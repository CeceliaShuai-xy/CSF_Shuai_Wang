/* Group Members: Cecelia Shuai xshuai3, Gigi Wang ywang580 */ 
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"
#include "client_util.h"
#include "csapp.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

struct ConnInfo{
  Connection* connection;
  Server *server;

  ConnInfo(Connection* connection, Server* server) {
    this->connection = connection;
    this->server = server;
  }

  ~ConnInfo() {
    delete connection;
  }
};

namespace {




void *worker(void *arg) {
  pthread_detach(pthread_self());

  //       use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  ConnInfo* info = static_cast<ConnInfo *>(arg);
  Server* server = info->server;
  Message message;
  Connection* connection = info->connection;
  
  if(!connection->receive(message)) {
    // if not received message, send error message
    connection->send(Message(TAG_ERR,"Cannot receive the message"));
    delete info; // free the connect info
  }

  // if message is received properly, send ok 
  // and communicate with client based on the message tag
  
  // create a new user with empty username 
  std::string user_name;
  User* user = new User("");

  // if the tag is valid login (r or s login), check the username validity
  if(message.tag == TAG_SLOGIN) {
    user_name = trim(message.data);
    if (!server->checkUsernameValid(user_name)){
      // if the username is invalid, send error message and delete the user 
      connection->send(Message(TAG_ERR, "Invalid username"));
      // free the user if early return
      delete user;
      return nullptr;
    }
    user->username = user_name;
    connection->send(Message(TAG_OK, "You successfully log in"));
    // chat with sender
    server->chat_with_sender(connection, info->server,user->username);
  } else if (message.tag == TAG_RLOGIN) {
    user_name = trim(message.data);
    if (!server->checkUsernameValid(user_name)){
      // if the username is invalid, send error message and delete the user 
      connection->send(Message(TAG_ERR, "Invalid username"));
      delete user;
      return nullptr;
    }
    user->username  = user_name;
    connection->send(Message(TAG_OK, "You successfully log in"));
    server->chat_with_receiver(connection, info->server,user);
  } else {
    // if tag is invalid (not login), send error message
    connection->send(Message(TAG_ERR, "You need to log in first"));
  }

  //delete info;
  delete user;
  return nullptr;
}
}

// helper function to communicate with the sender
void Server::chat_with_sender(Connection* connection, Server* server, std::string username) {
  Message msg;
  Room* room = nullptr;
  // always true, waiting for senders' messages
  while (1) {
    // if unable to receive message, send error message
    if(!connection->receive(msg)){
      connection->send(Message(TAG_ERR,"invalid message"));
    } else {
    // if successfully received the message, check the message tag
      if (msg.tag == TAG_JOIN) {
        // if join
        if (!checkMessageValidLength(msg)) { 
          // check if message is over MAX_LEN
          connection->send(Message(TAG_ERR, "invalid message"));
          return;
        }
        // find or create the room with the room name 
        room = server->find_or_create_room(trim(msg.data));
        connection->send(Message(TAG_OK, "You successfully join the room"));
      } else if (msg.tag == TAG_LEAVE) {
        // If leave the room
        if (room == nullptr) {
          // report error if user is not yet in the room
          connection->send(Message(TAG_ERR,"Not in a room yet"));
        } else {
          // leave the room -> room is invalidated
          room = nullptr;
          connection->send(Message(TAG_OK, "You successfully leave the room"));
        }
      } else if (msg.tag == TAG_SENDALL) {
        // if sendall
        if (room == nullptr) {
          // report error if user is not yet in the room
          connection->send(Message(TAG_ERR,"Not in a room yet"));
        } else {
          // check if message is over MAX_LEN
          if (!checkMessageValidLength(msg)) { 
            connection->send(Message(TAG_ERR, "invalid message"));
            return;
          }
          // if the message is valid, broadcast the message to all receivers in the room by adding
          // the message to the message queue
          room->broadcast_message(username, trim(msg.data));
          connection->send(Message(TAG_OK, "You successfully send the message to the entire room"));
        }

      } else if (msg.tag == TAG_QUIT) {
        // if user quit, exit the loop
        connection->send(Message(TAG_OK, "You successfully quit the program"));
        return;
      } else {
        // invalid tag
        connection->send(Message(TAG_ERR,"invalid message"));
      }
    }
    
  }

}

// helper function to check if message is valid in length
bool Server::checkMessageValidLength(Message &msg) {
  if (msg.data.find("\n") == std::string::npos) {
    return false;
  }
  return true;
}

// helper function to check if username is valid in format
bool Server::checkUsernameValid(const std::string& username) {
  if (username.length() == 0) {
    return false;
  }
  for (char c : username) {
    if (!std::isalnum(c)) { 
        return false;
    }
  }
  return true;
}


// helper function to communicate with the receiver
void Server::chat_with_receiver(Connection* connection, Server* server, User* user) {
  Message join_message;
  if (!connection->receive(join_message)) {
    // send error when unable to receive message
    connection->send(Message(TAG_ERR,"Unable to receive the join message"));
    return;
  }
  if (join_message.tag != TAG_JOIN) {
    // if the first message is not join, remind to join after login
    if (!checkMessageValidLength(join_message)) { // check if message is over MAX_LEN
      connection->send(Message(TAG_ERR, "invalid join message"));
      return;
    }
    connection->send(Message(TAG_ERR,"Have to join after logging in"));
    return;
  }
  // find (create) the room to join
  Room* room = server->find_or_create_room(trim(join_message.data));
  // add the member to the room
  room->add_member(user);
  connection->send(Message(TAG_OK, "You successfully join the room"));

  // if the user quits or the connection is invalid
  if (connection->get_last_result() == Connection::EOF_OR_ERROR) {
    // remove user from the room members
    room->remove_member(user);
    return;
  }

// once in the room, wait for the sender's message from the message queue 
  while (true) {
    Message* message = user->mqueue.dequeue();
    if(message != nullptr) {
      // has available message, send the message
      connection->send(*message);
      delete message;
    }

    if (connection->get_last_result() == Connection::EOF_OR_ERROR) {
      // if the connection fails, break the while loop
      break;
    }
  }
  // if out of the loop (user quits), remove the receiver from the room
  room->remove_member(user);

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  // destroy mutex
  pthread_mutex_destroy(&m_lock);
  //free room:
  for(std::map<std::string, Room *>::iterator it=m_rooms.begin(); it!=m_rooms.end(); ++it)
  {
      delete it->second;
  }
}

bool Server::listen() {
  //       use open_listenfd to create the server socket, return true
  //       if successful, false if not
  int fd = open_listenfd(std::to_string(m_port).c_str());

  if (fd < 0) {
    // unable to create the server socket
    return false;
  }
  m_ssock = fd;
  return true;
}

void Server::handle_client_requests() {
  //       infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while (1) {
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
      fprintf(stderr, "%s\n","Connot accept client connection");
      exit(1);
    }

    Connection* connection = new Connection(clientfd);
    ConnInfo* info = new ConnInfo(connection, this);
    pthread_t thr_id;
    if (pthread_create(&thr_id, NULL, worker, info) != 0) {
      fprintf(stderr, "%s\n","Connot create thread");
      connection->send(Message(TAG_ERR,"can't create thread"));
      delete info;
      exit(1);
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Guard g(m_lock);
  if (m_rooms.find(room_name) == m_rooms.end()) {
    m_rooms[room_name] = new Room(room_name);
  }
  return m_rooms.at(room_name);
}
