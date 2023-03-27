#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "msgq.h"



/*initialises message queue and returns a pointer to a struct msgq.
*num_msgs is the maximum number of messages that may be
*in the message queue. the returned pointer is used in the
*in the other API functions
*/
struct msgq *msgq_init(int num_msgs){



}




/*places msg on message queue mq.
*mq is returned from msgq_init. 
*msgq_send makes a copy of msg on the heap.
*if mq has num_msgs in it; msgq_send blocks until there
* is room for the message (msg).
*msgq_send returns 1 for success and -1 for failure.
*/
int msgq_send(struct msgq *mq, char *msg){}




/*msgq_recv returns a message from mq. mq is obtained from msgq_init.
*The returned message on the heap. The funtion that receives the
*message can 'free()' it when it is no longer needed.
*/
char *msgq_recv(struct msgq *mq){}




/*returns the number of messages on mq.
*mq is returned from msgq_init(). Due to the nature of threads
*and interrupts, the length returned may be incorrect by the time
*it is used.
*/
int msgq_len(struct msgq *mq){}




/*displays all of the messages in mq to stdout.
*mq is returned from msgq_init.
*/
void msgq_show(struct msgq *mq){}

