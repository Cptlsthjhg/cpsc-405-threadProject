#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "zemaphore.h"
#include "msgq.h"


//see line 18.
//#define MAXSTRINGLENGTH 100

/*initialises message queue and returns a pointer to a struct msgq.
 *num_msgs is the maximum number of messages that may be
 *in the message queue. the returned pointer is used in the
 *in the other API functions
 */

int const MAXSTRINGLENGTH = 100;

struct msgq *msgq_init(int num_msgs){

    struct msgq* theQ  = (struct msgq * ) malloc(sizeof(struct msgq));
    theQ->capacity = num_msgs;
    theQ->size = 0;
    theQ->head = NULL;

    struct __zem_t * zf = (struct __zem_t *) malloc(sizeof(struct __zem_t));
    struct __zem_t * ze = (struct __zem_t *) malloc(sizeof(struct __zem_t));
    zem_init(ze , 0);
    theQ->zf = zf;
    theQ->ze = ze;
    return theQ;

}




/*places msg on message queue mq.
 *mq is returned from msgq_init. 
 *msgq_send makes a copy of msg on the heap.
 *if mq has num_msgs in it; msgq_send blocks until there
 * is room for the message (msg).
 *msgq_send returns 1 for success and -1 for failure.
 */
int msgq_send(struct msgq *mq, char *msg){
    char* msgCopy = (char*)malloc(sizeof(char) * MAXSTRINGLENGTH );
    strcpy(msgCopy , msg);


    if(mq->size==mq->capacity){
        //block until there is room for the message.
        printf("MQ WAITING BECAUSE FULL\n"); 
        zem_wait(mq->zf);
    }
    else{
        //in this case, we can just add the message.
        struct node * newNode = (Node*)malloc(sizeof(Node));
        newNode->data=msgCopy;
        newNode->next = NULL;

        //printf("DEBUG: MSGCOPY: %s \n",newNode->data); 

        if(msgq_empty(mq)==0){
            //if the lsit was empty, set the new node as the head.
            printf("MQ POSTING BECAUSE EMPTY (NOW FILLING)\n"); 
            mq->head = newNode;
            zem_post(mq->ze);
        }

        else{
            //then we just go to the last node.
            Node* current = mq->head;
            while(current->next != NULL){
                current = current->next;
            }

            //now that we are at the last node, we throw the new one on the end.
            current->next = newNode;
        }
        //then we have to increase the size of the list.
        mq->size = mq->size + 1;
    }
return 1;
}


/*msgq_recv returns a message from mq. mq is obtained from msgq_init.
 *The returned message on the heap. The funtion that receives the
 *message can 'free()' it when it is no longer needed.
 */
char *msgq_recv(struct msgq *mq){

    //Dequeue

    if(msgq_empty(mq)==0)
    { 
        printf("MQ WAITING BECAUSE EMPTY (WAITING FOR FILL)\n"); 
        zem_wait(mq->ze);
    }


    //first we obtain the node at the head position
    struct node* localNode = mq->head;
    //then we extract the data from the node
    char* payload = (char*)malloc(sizeof(char) * MAXSTRINGLENGTH );
    //if(localNode == NULL){
      //  perror("ENCOUNTERED NULL WITH MSGQ NOT EMPTY.");
        //exit -1;
    //}
    strcpy(payload, localNode->data);
    //we need to change the head to next before deletion.
    mq->head = localNode->next;
    //then we free the node (deleting it)
    free(localNode);
    //post if the q was full.
    if(mq->size==mq->capacity){
        printf("MQ POSTING BECAUSE FULL (EMPTYING NOW)\n"); 
        zem_post(mq->zf);
    }
    mq->size = mq->size - 1;
    //return the payload (data)

    return payload;

}




/*returns the number of messages on mq.
 *mq is returned from msgq_init(). Due to the nature of threads
 *and interrupts, the length returned may be incorrect by the time
 *it is used.
 */
int msgq_len(struct msgq *mq){
    return mq->size;
}




/*displays all of the messages in mq to stdout.
 *mq is returned from msgq_init.
 */
void msgq_show(struct msgq *mq){

    struct node* localNode = mq->head;
    char* payload = (char*)malloc(sizeof(char) * MAXSTRINGLENGTH );

    if(localNode == NULL){
    perror("HEAD EMPTY. RETURNING FROM MSGQ_SHOW.");
    return;}

    if(localNode->data != NULL){
    strcpy(payload, localNode->data);
    printf("contents: %s\n",payload);

    while(localNode->next!=NULL){
        localNode = localNode->next;
        strcpy(payload, localNode->data);
        printf("contents: %s\n",payload);

    }

    printf("read all the way through msgq ( msgq_show() ) \n"); 
}
else{perror("msgq_show encountered empty mq.");}
}
/*
 *
 */
int msgq_empty(struct msgq *mq){

    if(mq->head==NULL){
        //this is the case where the q is empty:
        return 0;
    }
    else{return 1;}
}



/*
 *
 */
char * msgq_peek(struct msgq *mq){
    struct node* localNode = mq->head;
    char* payload = (char*)malloc(sizeof(char) * MAXSTRINGLENGTH);
    strcpy(payload, localNode->data);
    return payload;
}
