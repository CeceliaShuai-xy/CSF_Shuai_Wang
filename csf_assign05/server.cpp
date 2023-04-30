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

  // TODO: use a static cast to convert arg from a void* to
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

  // if message is received properly, send ok and communicate with client
  std::string user_name;
  if(message.tag == TAG_SLOGIN) {
    user_name = message.data;
    connection->send(Message(TAG_OK, "You successfully log in"));
    server->chat_with_sender(connection, info->server,user_name);
  } else if (message.tag == TAG_RLOGIN) {
    user_name = message.data;
    connection->send(Message(TAG_OK, "You successfully log in"));
    server->chat_with_receiver(connection, info->server,user_name);
  } else {
    connection->send(Message(TAG_ERR, "You need to log in first"));
  }

  delete info;
  return nullptr;
}
}

void Server::chat_with_sender(Connection* connection, Server* server, std::string username) {
  Message msg;
  Room* room = nullptr;
  while (1) {
    if(!connection->receive(msg)){
      connection->send(Message(TAG_ERR,"invalid message"));
    } else {
      if (msg.tag == TAG_JOIN) {
        // if join
        room = server->find_or_create_room(rtrim(msg.data));
        connection->send(Message(TAG_OK, "You successfully join the room"));

      } else if (msg.tag == TAG_LEAVE) {
        // If leave the room
        if (room == nullptr) {
          connection->send(Message(TAG_ERR,"Not in a room yet"));
        } else {
          // leave the room -> room is invalie
          room = nullptr;
          connection->send(Message(TAG_OK, "You successfully leave the room"));
        }
      } else if (msg.tag == TAG_SENDALL) {
        if (room == nullptr) {
          connection->send(Message(TAG_ERR,"Not in a room yet"));
        } else {
          room->broadcast_message(username, rtrim(msg.data));
          connection->send(Message(TAG_OK, "You successfully send the message to the entire room"));
        }

      } else if (msg.tag == TAG_QUIT) {
        connection->send(Message(TAG_OK, "You successfully quit the program"));
        return;
      } else {
        // invalid tag
        connection->send(Message(TAG_ERR,"invalid message"));
      }
    }
    
  }

}

void Server::chat_with_receiver(Connection* connection, Server* server, std::string username) {

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  int fd = open_listenfd(std::to_string(m_port).c_str());

  if (fd < 0) {
    return false;
  }
  m_ssock = fd;
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
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
