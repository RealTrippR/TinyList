#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

enum TINYLIST_RETURN_CODE {
    TINYLIST_FAIL,
    TINYLIST_SUCCESS
};
/*
#ifdef __TINY_LIST_VERIFY_OPERATIONS__\
    if (!node) {\
        assert(0&&"TINY LIST: Failed to create and allocate node!");\
    }\
#endif\

#ifdef __TINY_LIST_VERIFY_OPERATIONS__\
    if (pos > getLinkedListLength_##NODE_TYPE(head)-1) {assert(0&&"TINY LIST: INVALID OPERATION (OUT OF BOUNDS): Cannot get a node at a position that is outside the list!");}\
#endif\

#ifdef __TINY_LIST_VERIFY_OPERATIONS__\
    if (offsetFromPrev==0) {assert(0&&"TINY LIST: cannot delete a node with an offset of 0 from the previousNode");}\
    if (offsetFromPrev > getLinkedListLength_##NODE_TYPE(prev)-1) {assert(0&&"TINY LIST: INVALID OPERATION (OUT OF BOUNDS): Cannot delete node with offset from the previous, it is outside of the list!");}\
#endif\
*/

#define LIST_NODE(NODE_TYPE) struct listNode_##NODE_TYPE\

#define DEFINE_LIST_NODE_TYPE(NODE_TYPE)\
struct listNode_##NODE_TYPE \
{ \
    struct listNode_##NODE_TYPE* next;\
    NODE_TYPE value; \
}; \
/**********************************************************************************/\
/*                              FUNCTION PROTOTYPES:                              */\
LIST_NODE(NODE_TYPE)* createListNode_##NODE_TYPE();\
void destroyLinkedList_##NODE_TYPE(LIST_NODE(NODE_TYPE)* node);\
void reverseLinkedList_##NODE_TYPE(LIST_NODE(NODE_TYPE)** head);\
void insertListNode_##NODE_TYPE(LIST_NODE(NODE_TYPE)* prev, LIST_NODE(NODE_TYPE)* nodeToInsert);\
uint32_t getPositionOfNode_##NODE_TYPE(LIST_NODE(NODE_TYPE)* head, const LIST_NODE(NODE_TYPE)* node);\
LIST_NODE(NODE_TYPE)* getNodeAtPosition_##NODE_TYPE(LIST_NODE(NODE_TYPE)* head, const uint32_t pos);\
uint32_t getLinkedListLength_##NODE_TYPE(LIST_NODE(NODE_TYPE)* head);\
void deleteNode_##NODE_TYPE(LIST_NODE(NODE_TYPE)* prev, uint32_t offsetFromPrev);\
/**********************************************************************************/\

#include "validator/tinylistValidator.h"

void __tiny_list__deallocNode(void* node) {

#ifdef __TINY_LIST_VERIFY_OPERATIONS__
    hashmap_delete(TLcleanupValidator.addressHashMap, (void**)(&node));
#endif
    free(node);
}

void* __tiny_list__allocNode(uint16_t bytes) {
    void* ptr = malloc(bytes);
#ifdef __TINY_LIST_VERIFY_OPERATIONS__
    hashmap_set(TLcleanupValidator.addressHashMap, (void**)(&ptr));
#endif
    return ptr;
}


#define DEFINE_LINKED_LIST_OPERATIONS(NODE_TYPE)\
\
inline LIST_NODE(NODE_TYPE)* createListNode_##NODE_TYPE\
()\
{\
    LIST_NODE(NODE_TYPE)* node = (LIST_NODE(NODE_TYPE)*)__tiny_list__allocNode(sizeof(LIST_NODE(NODE_TYPE)));\
    VALIDATE_NODE_CREATION(NODE_TYPE,node);\
    memset(node, 0, sizeof(LIST_NODE(NODE_TYPE)));\
    return node;\
}\
\
inline void destroyLinkedList_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* node)\
{ \
    VALIDATE_LIST_CLEANUP(NODE_TYPE, node);\
    LIST_NODE(NODE_TYPE)* next = node->next;\
    __tiny_list__deallocNode(node);\
    if (next == NULL) { return; }\
    destroyLinkedList_##NODE_TYPE(next);\
}\
\
inline void reverseLinkedList_##NODE_TYPE\
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
inline void insertListNode_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* prev, LIST_NODE(NODE_TYPE)* nodeToInsert)\
{\
    LIST_NODE(NODE_TYPE)* oldNext = prev->next;\
    prev->next = nodeToInsert;\
    nodeToInsert->next=oldNext;\
}\
/*returns the position of the node in the list. If the node not a descendant of the head it will return -1*/ \
inline uint32_t getPositionOfNode_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* head, const LIST_NODE(NODE_TYPE)* node)\
{\
    uint32_t pos = 0u;\
    LIST_NODE(NODE_TYPE)* cur = head;\
    while (1) {\
        if (cur==node){return pos;}\
        cur = cur->next;\
        if (cur==NULL) {return 0u;} /*The node was not found in the list.*/\
        pos++;\
    }\
    return 0u;\
}\
\
inline LIST_NODE(NODE_TYPE)* getNodeAtPosition_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* head, const uint32_t pos)\
{\
    VALIDATE_GET_NODE_AT_POSITION(NODE_TYPE,head,pos);\
    uint32_t curPos = 0u;\
    LIST_NODE(NODE_TYPE)* cur = head;\
    while (1) {\
        if (curPos==pos) {\
            return cur;\
        }\
        curPos++;\
        cur = cur->next;\
    }\
    return NULL;\
}\
\
inline uint32_t getLinkedListLength_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* head)\
{\
    uint32_t len = 0;\
    LIST_NODE(NODE_TYPE)* cur = head;\
    while (cur!=NULL) {\
        len++;\
        cur = cur->next;\
    }\
    return len;\
}\
inline void deleteNode_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* prev, uint32_t offsetFromPrev)\
{\
    if (offsetFromPrev!=1) {prev = getNodeAtPosition_##NODE_TYPE(prev, offsetFromPrev-1);} /*Get offsetFromPrev-1*/ \
    LIST_NODE(NODE_TYPE)* next = prev->next->next; /*store the node after the node to delete*/ \
    __tiny_list__deallocNode(prev->next);\
    prev->next = next;\
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Allocates and returns a 0-initialized node*/
#define CREATE_LIST_NODE(NODE_TYPE) createListNode_##NODE_TYPE\
()
/*This recursively destroys every node in the list, starting at the head*/
#define DESTROY_LINKED_LIST(NODE_TYPE, node) destroyLinkedList_##NODE_TYPE\
(node)
/*Reverses the linked list, starting at the head*/
#define REVERSE_LINKED_LIST(NODE_TYPE, head) reverseLinkedList_##NODE_TYPE\
(head)
/*Inserts a node after prev. (prev->next will equal node, and node->next will reference the node that prev->next used to point to)*/
#define INSERT_LIST_NODE(NODE_TYPE, prev, node) insertListNode_##NODE_TYPE\
(prev, node)
/*Returns the node at the position if it exists, if it does not exist it will return NULL*/
#define GET_NODE_AT_POSITON(NODE_TYPE, head, pos) getNodeAtPosition_##NODE_TYPE\
(head,pos)
/*returns the position of the node in the list. If the node not a descendant of the head it will return -1*/ \
#define GET_POSITION_OF_NODE(NODE_TYPE, head, node) getPositionOfNode_##NODE_TYPE\
(head,node)
/*Returns the length of the linked list, starting from the head*/
#define GET_LIST_LENGTH(NODE_TYPE, head) getLinkedListLength_##NODE_TYPE\
(head)
/*Deallocates the node offsetFromPrevious nodes after prev*/
#define DELETE_NODE(NODE_TYPE, prev, offsetFromPrevious) deleteNode_##NODE_TYPE\
(prev, offsetFromPrevious)
