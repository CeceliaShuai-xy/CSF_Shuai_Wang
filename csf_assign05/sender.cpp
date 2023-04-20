#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

using std::string;
using std::stringstream;

void enter_loop(Connection &connection);

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];


  // TODO: connect to server
  Connection connection;
  connection.connect(server_hostname, server_port);

  // TODO: send slogin message
  Message sender_message = {TAG_SLOGIN, username};
  connection.send(sender_message);

  Message server_response;
  connection.receive(server_response);
  if (server_response.tag == TAG_ERR) {
    fprintf(stderr, "%s", server_response.data.c_str());
    exit(1);
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  enter_loop(connection);

  return 0;
}

void enter_loop(Connection &connection) {
  Message sender_message;
  Message server_response;
  while (true) {
    string input;
    std::getline(std::cin, input);
    stringstream ssinput(input);
    string token;
    ssinput >> token;
    
    if (token == "/join") {
      ssinput >> token;
      sender_message = {TAG_JOIN, token};
    } else if (token == "/leave") {
      sender_message = {TAG_LEAVE, ""};
    } else if (token == "/quit") {
      sender_message = {TAG_QUIT, ""};
    } else if (token[0]=='/'){
      fprintf(stderr, "%s\n", "invalid command");
      continue;
    } else{
      sender_message = {TAG_SENDALL, ssinput.str()};
    }

    if (!connection.send(sender_message)) {
      fprintf(stderr, "%s\n", "failure to send message.");
      exit(1);
    }

    if (!connection.receive(server_response)){
      fprintf(stderr, "%s\n", "failure to receive message.");
      exit(1);
    } else {
      if(server_response.tag == TAG_ERR) {
        fprintf(stderr, "%s", server_response.data.c_str());
      }
    }
    if (sender_message.tag == TAG_QUIT) {
      return;
    }
  }
}
