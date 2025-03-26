#include <stdlib.h>
#include <string.h>

#define LIST_NODE(NODE_TYPE) struct listNode_##NODE_TYPE

#define DEFINE_LIST_NODE_TYPE(NODE_TYPE) \
struct listNode_##NODE_TYPE \
{ \
    struct listNode_##NODE_TYPE* next;\
    NODE_TYPE value; \
}; \
\
LIST_NODE(NODE_TYPE)* createListNode_##NODE_TYPE\
()\
{\
    LIST_NODE(NODE_TYPE)* node = (LIST_NODE(NODE_TYPE)*)malloc(sizeof(LIST_NODE(NODE_TYPE)));\
    memset(node, 0, sizeof(LIST_NODE(NODE_TYPE)));\
    return node;\
}\
\
void destroyLinkedList_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* node)\
{ \
    if (NULL==node) {return;}\
    destroyLinkedList_##NODE_TYPE(node->next);\
    free(node);\
}\
\
void reverseLinkedList_##HEAD_TYPE\
(LIST_NODE(NODE_TYPE)** head)\
{\
    LIST_NODE(NODE_TYPE)* curr = *head, *prev = NULL, *next;\
    while (curr != NULL) {\
          next = curr->next;\
          /*Reverse current node's next pointer*/\
          curr->next = prev;\
          /*Move pointers one position ahead*/\
          prev = curr;\
          curr = next;\
    }\
    *head = prev; /*Update the head of the linked list*/\
}\
\
void insertListNode_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* prev, LIST_NODE(NODE_TYPE)* nodeToInsert)\
{\
    LIST_NODE(NODE_TYPE)* oldNext = prev->next;\
    prev->next = nodeToInsert;\
    nodeToInsert->next=oldNext;\
}\
\
int64_t getPositionOfNode_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* head, const LIST_NODE(NODE_TYPE)* node, const uint32_t maxTraversals)\
{\
    uint32_t pos = 0u;\
    LIST_NODE(NODE_TYPE)* cur = head;\
    while (pos < maxTraversals) {\
        if (cur==node){return pos;}\
        cur = cur->next;\
        pos++;\
    }\
    return -1;\
}\
\
LIST_NODE(NODE_TYPE)* getNodeAtPosition_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* head, const uint32_t pos, const uint32_t maxTraversals)\
{\
    uint32_t curPos = 0u;\
    LIST_NODE(NODE_TYPE)* cur = head;\
    while (curPos < maxTraversals) {\
        if (curPos==pos) {\
            return cur;\
        }\
        curPos++;\
        cur = cur->next;\
    }\
    return NULL;\
}\
\
uint32_t getLinkedListLength_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* head)\
{\
    uint32_t len = 0;\
    LIST_NODE(NODE_TYPE)* cur = head;\
    while (cur!=NULL) {\
        len++;\
        cur = cur->next;\
    }\
    return len;\
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CREATE_LIST_NODE(NODE_TYPE) createListNode_##NODE_TYPE\
()

#define DESTROY_LINKED_LIST(NODE_TYPE, node) destroyLinkedList_##NODE_TYPE\
(node)

#define REVERSE_LINKED_LIST(NODE_TYPE, head) reverseLinkedList_##HEAD_TYPE\
(head)

#define INSERT_LIST_NODE(NODE_TYPE, prev, node) insertListNode_##NODE_TYPE\
(prev, node)

#define GET_NODE_AT_POSITON(NODE_TYPE, head, pos, maxTraversals) getNodeAtPosition_##NODE_TYPE\
(head,pos,maxTraversals)

#define GET_POSITION_OF_NODE(NODE_TYPE, head, node, maxTraversals) getPositionOfNode_##NODE_TYPE\
(head,node,maxTraversals)

#define GET_LIST_LENGTH(NODE_TYPE, head) getLinkedListLength_##NODE_TYPE\
(head)