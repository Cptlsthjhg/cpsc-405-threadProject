#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "zemaphore.h"
#include "msgq.h"

// SEE Labs/GdbLldbLab for more information on lldb - lowlevel debugger

struct msgq *mq;

//
// Main threads
//
char *messages[] = { "msg1", "msg2", "hellomsg", "gustymsg" };

//static arrays for consumers.
int consumerArray[75];
//hopefully, we should only need to declare this once.
//this is because each pthread should have its own static section.
//this means each pthread will have its own static array.


//sends 50 messages.
void *producer(void*arg){
    int selection = (int) arg;
    //this is how we get the user to select
    //the messages that are generated. not sure what we're choosing from.

    char *dogs[] = { "retriever","pug","poodle","pomeranian","husky"};
    char *cats[] = { "tabby","calico","black","toroise shell","neon green"};

    //each of these have 5 different messages.
    //we will send 50 by sending all of these messages 10 times.

    int maxStrings = 6;
    char** messages = NULL;
    if(selection == 1){messages=dogs;}
    if(selection == 2) {messages=cats;}

    for (int j = 0 ; j < 10 ; j++){
        for (int i = 0; i < sizeof(messages)/sizeof(char*); i++) {
            printf("sending: %s\n", messages[i]);
            msgq_send(mq, messages[i]);
        }
    }
    //10*5 = 50. Therefore, we should be able to send 50 messages.
    printf("This is a producer thread. All finished sending messages. Goodbye.\n");
    return NULL;

}

//
void *consumer(void *arg){
    int me = (int)arg;
    int count=0;

    //this makes it so that thread 5 runs an extra time.
    if(me==5){count--;}


    //we intend to have two consumers (100/3 = 33 (remainder of 1) ). Therefore, we should read 33 things.
    //One extra read will be completed by one of the threads to remove that remaining message.
    while(count<33){
        char* m = msgq_recv(mq);
        printf("This is the consumer thread %d. we received %s.\n",me,m);
        free(m);
        count++;
    }
    printf("This is consumer thread %d. All finished gathering results.\n",me);
    return NULL;
}


// sends msgs in messages[]
void *promtAndSend(void *arg) {
    for (int i = 0; i < sizeof(messages)/sizeof(char*); i++) {
        char response[80];
        printf("Send? ");
        scanf("%s", response);
        if (response[0] == 'y' || response[0] == 'Y') {
            printf("sending: %s\n", messages[i]);
            msgq_send(mq, messages[i]);
        }
    }
    return NULL;
}

// consume messges in msgq
void *recvMsgs(void *arg) {
    sleep(5);
    int msg_count = msgq_len(mq);
    printf("mq msg_count: %d\n", msg_count);
    for (int i = 0; i < msg_count; i++) {
        char *m = msgq_recv(mq);
        printf("recvMsgs: %s\n", m);
        //free(m);
    }
    return NULL;
}

void *passiton(void *arg) {
    int me = (int) arg;
    while (1) {
        sleep(1);
        printf("passiton%d initial msgq_len: %d\n", me, msgq_len(mq));
        char *m = msgq_recv(mq);
        printf("passiton%d: %p %p %s\n", me, &m, m, m);
        printf("passiton%d after recv msgq_len: %d\n", me, msgq_len(mq));
        msgq_send(mq, m);
        printf("passiton%d after send msgq_len: %d\n", me, msgq_len(mq));
        free(m);
    }
    return NULL;
}

#define MSGQLEN 4

int main(int argc, char *argv[]) {
    pthread_t p1, p2, p3, p4, p5;
    mq = msgq_init(MSGQLEN);
    char test = '4';
    if (argc == 2)
        test = argv[1][0];
    switch (test) {
        case '1':
            printf("test fill and empty msgq\n");
            pthread_create(&p1, NULL, promtAndSend, NULL);
            pthread_join(p1, NULL);
            printf("msgq_show() after filling for test 1:\n");
            msgq_show(mq);
            pthread_create(&p2, NULL, recvMsgs, NULL);
            pthread_join(p2, NULL);
            printf("msgq_show() after all consumed by test 1:\n");
            msgq_show(mq);
            break;
        case '2':
            printf("test fill msgs and pass it on\n");
            pthread_create(&p1, NULL, promtAndSend, NULL);
            pthread_join(p1, NULL);
            printf("msgq_show() after filling for test 2:\n");
            msgq_show(mq);
            pthread_create(&p1, NULL, passiton, (void *)1);
            pthread_create(&p2, NULL, passiton, (void *)2);
            pthread_join(p1, NULL);
            pthread_join(p2, NULL);
            break;
        case '3':
            printf("This test case shows blocking and unblocking for cases where the q is full, and cases where the q is empty. \n");
            pthread_create(&p1, NULL, promtAndSend, NULL);
            pthread_join(p1, NULL);


            //this should block until p2 reads out.
            pthread_create(&p1, NULL, promtAndSend, NULL);
            pthread_create(&p2, NULL, recvMsgs,NULL);

            pthread_join(p1, NULL);
            pthread_join(p2, NULL);

            pthread_create(&p2, NULL, recvMsgs,NULL);
            pthread_join(p2, NULL);
            break;

        case '4':
            printf("This test case shows us our handmade producer and consumer methods.\n");
            printf("This test case will send 50 messages 2 times from threads (100 messages), then it will receive 33  messages twice from two threads.\n");
            pthread_create(&p1, NULL, producer, (void *)1);
            pthread_create(&p2, NULL, producer, (void *)2);
            pthread_create(&p3, NULL, consumer, (void *)3);
            pthread_create(&p4, NULL, consumer, (void *)4);
            pthread_create(&p5, NULL, consumer, (void *)5);
            pthread_join(p3,NULL);
            pthread_join(p4,NULL);
            pthread_join(p5,NULL);

            pthread_join(p1, NULL);
            pthread_join(p2, NULL);
            
        default:
            printf("invalid test selection!\n");
            break;
    }
    return 0;
}

