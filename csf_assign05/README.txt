Group Members:
Cecelia Shuai (xshuai3), Gigi Wang (ywang580).

Contribution:
MS1: Both members worked in person together and contributed equally to 
all the funcitons implemented.
MS2: Both members worked in person together and contributed equally to 
all the funcitons implemented, including program designm, program testing,
program debugging.

Synchronization Report: /* Explain how you implemented thread synchronization in the server*/

Our critical sections are in server.cpp, room.cpp, and message_queue.cpp. We determined our critical sessions by 
paying attention to where the shared data structures are being accessed/modified by different threads (clients).
When multiple clinets are accessing/modifying those data structure concurrently, unexpected behavior will occur if
sychronization is not done properly.

In general, we use Guard whenever we use mutex to ensure that all locked mutexes are unlock when the scope ends. This
practices reduces the chances of deadlocks that come from forgetting to unlock the mutex.

In server.cpp:
We use mutex to prevent race conditions when multiple clients are joining or creating new rooms concurrently. We
lock the mutex in find_or_create_room so that if two senders try to create the same room concurrently, they'll not
add the same room twice to our RoomMap. We use Guard so that the mutex is unlock automatically after the room is 
created/find, which prevents deadlock.

In room.cpp:
We use mutex to prevent race conditions in add_member, remove_member, and broadcast_message. add_member and remove_member
both modify UserSet members, which is a data structure that share across all the receivers in the room. broadcast_message 
also modify UserSet members, specifially it modifies each user in the Userset. If we didn't lock the mutex, then message 
will be lost when two senders calling broadcast_message concurrently. Again, we use Guard to prevent deadlock.

In message_queue.cpp:
We use mutex to prevent race conditions in enqueue and dequeue. enqueue and dequeue both modify deque<Message *> m_messages
data structure which is shared across sender and receiver. We want to allow sychronization when the sender and receiver both 
want to change the deque data structure at the same time. We also introduce a semaphore m_avail to allow us to control multiple 
messages with multiple operations enqueue/dequeue. The semaphore automatically tells the receiver to sleep when there's no message
in the queue, and wake it up when the sender put messages in the queue. To prevent deadlock from the interactions between mutex
and semaphore, we ensure that we call sem_wait before we lock the mutex in dequeue, and we unlock the mutex before we call 
sem_post in enqueue.

