//something

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "qNode.h"

int payloadSize = 50;



/* qNode_init ( char* payload) returns a pointer to a qNode.
   * this function creates and initialises a qNode.
   * please free() the node you create when no longer in use.
   */
struct *qNode qNode_init(char* payload){

    struct qNode *theNode = malloc(sizeof(struct qNode));
    theNode->payload = malloc(sizeof(char)*payloadSize);
    strcpy(theNode->payload, payload);
    theNode->next = NULL;
    return theNode;
}


/* pointTo(p1,p2) returns an int. 1 for failure, and 0 for success.
 *p1 is the qNode that is is going to point at p2.
 *p2 is the qNode that will be pointed to by p1.
 */

int pointTo(struct qNode* p1, struct qNode* p2){

    p1->next = p2;

    if(p1->next == p2){
        //this would mean success.
        return 0;
    }

    else{
        //this would mean failure
        return 1;
    }
}


