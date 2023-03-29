
typedef struct node{
    char* data;
    struct node* next;

}Node;

typedef struct msgq{
    int capacity;
    //although not necessary for a linked list, we will be capping
    //the linked list out as per instruction.


    //we will create our queue using a linked list.
    qNode* head;

}Msgq;

struct msgq *msgq_init(int num_msgs);

int msgq_send(struct msgq *mq, char *msg);

char *msgq_recv(struct msgq *mq);

int msgq_len(struct msgq *mq);

void msgq_show(struct msgq *mq);

int msgq_empty(struct msgq *mq);

char * msgq_peek(struct msgq *mq);
