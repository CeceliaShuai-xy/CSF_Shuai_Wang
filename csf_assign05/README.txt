Group Members:
Cecelia Shuai (xshuai3), Gigi Wang (ywang580).

Contribution:
MS1: Both members worked in person together and contributed equally to 
all the funcitons implemented.
MS2: Both members worked in person together and contributed equally to 
all the funcitons implemented, including program designm, program testing,
program debugging.

Synchronization Report: /* Explain how you implemented thread synchronization in the server*/




[NORA'S VERSION!!!]
Our critical sections are in room.cpp, message_queue.cpp as well as the find_or_create_room function in server.cpp.
We determined these critical sections by identifying if their code are using shared data. Specifially, multiple receiver / sender
access the room to get the room members / be added or removed from the room. Multiple senders will access the message_queue
to enqueue their messages, and there may be many users that find or create a room at the same time. For room.cpp, we only use 
mutex because we just have to ensure that no two programs are accessing it at the same time. For message_queue.cpp,
we used semaphore besides mutex. It is because semaphore allows the sender to notify the receiver to dequeue the
message when it is enqueued by the sender. Mutex is used to ensure no two senders are updating the semaphore at the
same time. We ensure sychronization hazards (deadlocks) are not introduced by calling the Guard object to lock / unlock the mutex.
This allows the lock to be released when it is out of scope without explicitly calling pthread_mutex_unlock, which prevents
deadlocks induced by interuptions and simplifies coding. 
