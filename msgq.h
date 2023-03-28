
typedef struct msgq{

    int capacity;
    //capacity is used as the maximum value for the queue.

    char ** stringQueue;
    //this is a pointer to an UNITIALISED ARRAY.
    //this will hold our strings.
    //the pointer will need to be initialised to a char** (malloc)

}msgq;

struct msgq *msgq_init(int num_msgs);

int msgq_send(struct msgq *mq, char *msg);

char *msgq_recv(struct msgq *mq);

int msgq_len(struct msgq *mq);

void msgq_show(struct msgq *mq);
