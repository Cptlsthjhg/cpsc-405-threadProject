
typedef struct qNode{

    struct * qNode next;
    char * payload;

}qNode;


struct *qNode qNode_init(int payloadSize);

int pointTo(struct qNode* p1, struct qNode*);
