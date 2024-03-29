/* Group Members: Cecelia Shuai xshuai3, Gigi Wang ywang580 */ 
#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "guard.h"
#include "message.h"

MessageQueue::MessageQueue() {
  // initialize the mutex and the semaphore
  pthread_mutex_init(&m_lock, NULL);
  sem_init(&m_avail,0,0);
}

MessageQueue::~MessageQueue() {
  // destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
  // free message in the message queue
  Message * msg;
  while (m_messages.size() != 0){
    msg = m_messages.back();
    m_messages.pop_back();
    delete msg;
  }
}

void MessageQueue::enqueue(Message *msg) {
  // put the specified message on the queue
  {
    // use guard to protect message queues 
    Guard g(m_lock);
    m_messages.push_front(msg);
  }
  // notify any thread waiting for a message to be available by calling sem_post
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts;
  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;
  Message *msg;

  // call sem_timedwait to wait up to 1 second for a message
  // to be available, return nullptr if no message is available
  if (sem_timedwait(&m_avail,&ts)!= 0) {
    return nullptr;
  } 
  // remove the next message from the queue, return it
  {
    // use guard when dequeue the m_messages 
    Guard g(m_lock);
    msg = m_messages.back();
    m_messages.pop_back();
  }
  
  return msg;
}
