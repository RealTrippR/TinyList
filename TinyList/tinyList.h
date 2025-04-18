/*
Copyright © 2025 Tripp Robins

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the “Software”), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TINY_LIST_H
#define TINY_LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <TinyList/ext/cthread.h>

enum TINYLIST_RETURN_CODE {
    TINYLIST_FAIL,
    TINYLIST_SUCCESS
};

#define LIST_NODE(NODE_TYPE) struct listNode_##NODE_TYPE

#if defined(_MSC_VER)
#include <xmmintrin.h>
#define PREFETCH(addr) _mm_prefetch((const char*)(addr), _MM_HINT_T0)
#elif defined(__GNUC__) || defined(__clang__)
#define PREFETCH(addr) __builtin_prefetch((addr), 0, 1)
#else
#define PREFETCH(addr)
#endif

/**********************************************************************************/
/*                                MULTI-THREADING                                 */
#ifndef TINY_LIST_THREAD_COUNT 
    #define TINY_LIST_THREAD_COUNT 4
#endif

struct TINY_LIST_CREATE_NODES_THREAD_DATA {
    uint16_t thread_id;
    uint32_t allocCount;
    void* head;
    void* nextHead;
};

#define DEFINE_LIST_NODE_TYPE(NODE_TYPE)\
struct listNode_##NODE_TYPE \
{ \
    struct listNode_##NODE_TYPE* next;\
    NODE_TYPE value; \
}; \
/**********************************************************************************/\
/*                              FUNCTION PROTOTYPES:                              */\
LIST_NODE(NODE_TYPE)* createListNode_##NODE_TYPE();\
LIST_NODE(NODE_TYPE)* createLinkedListOfSize##NODE_TYPE(uint32_t size);\
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
    void* __restrict ptr = malloc(bytes);
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
void thread_create_nodes##NODE_TYPE\
(void* arg)\
{\
    struct  TINY_LIST_CREATE_NODES_THREAD_DATA* data = (struct TINY_LIST_CREATE_NODES_THREAD_DATA*)arg;\
    LIST_NODE(NODE_TYPE)* lastNode = (LIST_NODE(NODE_TYPE)*)data->head;\
    for (uint_fast32_t i = 1/*starts at head*/; i < data->allocCount; ++i)\
    {\
        LIST_NODE(NODE_TYPE)* node = CREATE_LIST_NODE(NODE_TYPE);\
        INSERT_LIST_NODE(NODE_TYPE, lastNode, node);\
        lastNode = node;\
    }\
    lastNode->next = data->nextHead;\
}\
inline LIST_NODE(NODE_TYPE)* createLinkedListOfSize##NODE_TYPE\
(uint32_t size)\
{\
    /*Multithreading performance gains only occur for large list, a optimal number seems to be over 100K elements */\
    const int MT_PERFORMANCE_THRESHOLD_COUNT = TINY_LIST_THREAD_COUNT+100000;  /*Note that the TINY_LIST_THREAD_COUNT is added to the value to ensure that lists with a <= the thread count will be created with only a single thread */\
    if (size > MT_PERFORMANCE_THRESHOLD_COUNT && TINY_LIST_THREAD_COUNT>1) {\
        static struct cthreads_thread threads[TINY_LIST_THREAD_COUNT]; \
        static struct TINY_LIST_CREATE_NODES_THREAD_DATA thread_data[TINY_LIST_THREAD_COUNT]; \
        static struct cthreads_args winArgs[TINY_LIST_THREAD_COUNT]; \
        LIST_NODE(NODE_TYPE)* tmpHeadNodes[TINY_LIST_THREAD_COUNT];\
        for (uint_fast8_t i = 0; i < TINY_LIST_THREAD_COUNT; ++i) {\
            tmpHeadNodes[i] = CREATE_LIST_NODE(NODE_TYPE);\
        }\
        \
        uint_fast32_t chunkSize = size / TINY_LIST_THREAD_COUNT;\
        uint_fast32_t remainder = size % TINY_LIST_THREAD_COUNT;\
        for (uint_fast8_t i = 0; i < TINY_LIST_THREAD_COUNT; ++i) {\
            /*Create a head for each thread. The heads will be joined back together once the creation of nodes is complete*/\
            thread_data[i].thread_id = i;\
            thread_data[i].allocCount = chunkSize + chunkSize + ((uint32_t)(i < remainder));\
            thread_data[i].head = &(tmpHeadNodes[i]);\
            \
            if (i<TINY_LIST_THREAD_COUNT-1) {\
                thread_data[i].nextHead = &(tmpHeadNodes[i+1]); \
            } \
            else \
            {thread_data[i].nextHead = NULL;}\
            \
            winArgs[i].func = thread_create_nodes##NODE_TYPE;\
            winArgs[i].data = &thread_data[i];\
            \
            cthreads_thread_create(&threads[i], NULL, thread_create_nodes##NODE_TYPE, &thread_data[i],  &(winArgs[i]));\
        }\
        \
        for (uint_fast8_t i = 0; i < TINY_LIST_THREAD_COUNT; ++i) {\
            cthreads_thread_join(threads[i], NULL);\
        }\
        for (uint_fast8_t i = 0; i < TINY_LIST_THREAD_COUNT; ++i) {\
            cthreads_thread_detach(threads[i]);\
        }\
        return tmpHeadNodes[0];\
    } else {\
        LIST_NODE(NODE_TYPE)* head = CREATE_LIST_NODE(NODE_TYPE);\
        LIST_NODE(NODE_TYPE)* lastNode = head;\
        for (uint_fast16_t i = 1/*starts at head*/; i < size; ++i) {\
            LIST_NODE(NODE_TYPE)* node = CREATE_LIST_NODE(NODE_TYPE);\
            INSERT_LIST_NODE(NODE_TYPE, lastNode, node);\
            lastNode=node;\
        }\
        return head;\
    }\
}\
\
inline void destroyLinkedList_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* node)\
{ \
    VALIDATE_LIST_CLEANUP(NODE_TYPE, node);\
    while (node != NULL) {\
        const void* __restrict tmp = node->next;\
        __tiny_list__deallocNode(node);\
        node=(LIST_NODE(NODE_TYPE)*)tmp;\
    }\
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
        PREFETCH( cur->next);\
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
/*Creates a linked list of a given size, and for very large lists it will use multithreading to speed up creation time.*/
#define CREATE_LINKED_LIST(NODE_TYPE, size) createLinkedListOfSize##NODE_TYPE\
(size)

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

#endif // !TINY_LIST_H