#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

#include <string>

using std::string;

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  string port_str = std::to_string(port);
  int fd = open_clientfd(hostname.c_str(), port_str.c_str());
  if (fd < 0) {
    fprintf(stderr, "%s\n", "Could not connect to server");
    exit(1);
  }
  m_fd = fd;
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

Connection::~Connection() {
    this->close();
}

bool Connection::is_open() const {
  if (m_last_result == EOF_OR_ERROR) {
    return false;
  }
  return true;
}

void Connection::close() {
  // TODO: close the connection if it is open
  if (this->is_open()) {
    ::close(m_fd);
    m_fd = -1;
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  assert(m_last_result == SUCCESS);
  assert(is_open());
  string message = msg.concat_message();
  ssize_t s = rio_writen(m_fd, message.c_str(), message.length());
  if (s != (ssize_t)message.length()) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  m_last_result = SUCCESS;
  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  assert(m_last_result == SUCCESS);
  assert(is_open());

  char buf[Message::MAX_LEN + 1]; //create buffer
  ssize_t s = rio_readlineb(&m_fdbuf, buf, Message::MAX_LEN);
  if (s < 1) {
    msg.tag = TAG_EMPTY;
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  string message(buf);
  int colon_pos = message.find(":");
  msg.tag = message.substr(0, colon_pos);
  msg.data = message.substr(colon_pos + 1, message.length());

  //invalid if : doesn't exist
  if(colon_pos < 0) {
    m_last_result = INVALID_MSG;
    return false;
  }

  if (msg.tag != TAG_ERR && msg.tag != TAG_OK && 
  msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN &&
  msg.tag != TAG_JOIN && msg.tag != TAG_LEAVE && msg.tag != TAG_SENDALL
  && msg.tag != TAG_QUIT && msg.tag != TAG_DELIVERY) {
    m_last_result = INVALID_MSG;
    return false;
  }

  m_last_result = SUCCESS;
  return true;
}
