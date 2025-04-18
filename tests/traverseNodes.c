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

#include <stdio.h>
#include <stdint.h>

#include "get_time_us.h";

// Order matters
//#define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations*/
#include <TinyList/tinyList.h>
//__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(uint32_t) /*Defines the verification functions*/
DEFINE_LIST_NODE_TYPE(uint32_t)
/*
inline struct listNode_uint32_t* createListNode_uint32_t() {
    struct listNode_uint32_t* node = (struct listNode_uint32_t*)__tiny_list__allocNode(sizeof(struct listNode_uint32_t)); ; memset(node, 0, sizeof(struct listNode_uint32_t)); return node;
}
void thread_create_nodesuint32_t(void* arg) {
    struct TINY_LIST_CREATE_NODES_THREAD_DATA* data = (struct TINY_LIST_CREATE_NODES_THREAD_DATA*)arg;
    struct listNode_uint32_t* lastNode = (struct listNode_uint32_t*)data->head; 
    for (uint_fast32_t i = 1; i < data->allocCount; ++i) {
        struct listNode_uint32_t* node = createListNode_uint32_t(); 
        insertListNode_uint32_t(lastNode, node);
        lastNode = node;
    } 
    lastNode->next = data->nextHead;
}
inline struct listNode_uint32_t* createLinkedListOfSizeuint32_t(uint32_t size) {
    const int MT_PERFORMANCE_THRESHOLD_COUNT = 4 + 100000;
    if (size > 0 && 4>1) {
        static struct cthreads_thread threads[4]; 
        static struct TINY_LIST_CREATE_NODES_THREAD_DATA thread_data[4];
        static struct cthreads_args winArgs[4];
        struct listNode_uint32_t* tmpHeadNodes[4];
        for (uint_fast8_t i = 0; i < 4; ++i) {
            tmpHeadNodes[i] = createListNode_uint32_t();
        } 
        uint_fast32_t chunkSize = size / 4; uint_fast32_t remainder = size % 4;
        for (uint_fast8_t i = 0; i < 4; ++i) {
            thread_data[i].thread_id = i; 
            thread_data[i].allocCount = chunkSize + ((uint32_t)(i < remainder));
            thread_data[i].head = tmpHeadNodes[i];
            if (i<4 - 1) {
                thread_data[i].nextHead = tmpHeadNodes[i + 1];
            }
            else {
                thread_data[i].nextHead = ((void*)0);
            } 
            winArgs[i].func = thread_create_nodesuint32_t;
            winArgs[i].data = &thread_data[i];
            cthreads_thread_create(&threads[i], ((void*)0), thread_create_nodesuint32_t, &thread_data[i], &(winArgs[i]));
        }
        for (uint_fast8_t i = 0; i < 4; ++i) {
            cthreads_thread_join(threads[i], ((void*)0));
        }
        return tmpHeadNodes[0];
    }
    else {
        struct listNode_uint32_t* head = createListNode_uint32_t(); struct listNode_uint32_t* lastNode = head; for (uint_fast16_t i = 1; i < size; ++i) {
            struct listNode_uint32_t* node = createListNode_uint32_t(); insertListNode_uint32_t(lastNode, node); lastNode = node;
        } return head;
    }
}inline void destroyLinkedList_uint32_t(struct listNode_uint32_t* node) {
    ; while (node != ((void*)0)) {
        const void* __restrict tmp = node->next; __tiny_list__deallocNode(node); node = (struct listNode_uint32_t*)tmp;
    }
}inline void reverseLinkedList_uint32_t(struct listNode_uint32_t** head) {
    struct listNode_uint32_t* curr = *head, * prev = ((void*)0), * next; while (curr != ((void*)0)) {
        next = curr->next; curr->next = prev; prev = curr; curr = next;
    } *head = prev;
}inline void insertListNode_uint32_t(struct listNode_uint32_t* prev, struct listNode_uint32_t* nodeToInsert) {
    struct listNode_uint32_t* oldNext = prev->next; prev->next = nodeToInsert; nodeToInsert->next = oldNext;
} inline uint32_t getPositionOfNode_uint32_t(struct listNode_uint32_t* head, const struct listNode_uint32_t* node) {
    uint32_t pos = 0u; struct listNode_uint32_t* cur = head; while (1) {
        if (cur == node) {
            return pos;
        } cur = cur->next; if (cur == ((void*)0)) {
            return 0u;
        } pos++;
    } return 0u;
}inline struct listNode_uint32_t* getNodeAtPosition_uint32_t(struct listNode_uint32_t* head, const uint32_t pos) {
    ; uint32_t curPos = 0u; struct listNode_uint32_t* cur = head; while (1) {
        if (curPos == pos) {
            return cur;
        } curPos++; _mm_prefetch((const char*)(cur->next), 1); cur = cur->next;
    } return ((void*)0);
}inline uint32_t getLinkedListLength_uint32_t(struct listNode_uint32_t* head) {
    uint32_t len = 0; struct listNode_uint32_t* cur = head; while (cur != ((void*)0)) {
        len++; cur = cur->next;
    } return len;
}inline void deleteNode_uint32_t(struct listNode_uint32_t* prev, uint32_t offsetFromPrev) {
    if (offsetFromPrev != 1) {
        prev = getNodeAtPosition_uint32_t(prev, offsetFromPrev - 1);
    } struct listNode_uint32_t* next = prev->next->next; __tiny_list__deallocNode(prev->next); prev->next = next;
};
*/
DEFINE_LINKED_LIST_OPERATIONS(uint32_t)

#define NODE_COUNT 250000
#define TARGET_NODE_POS NODE_COUNT-1
#define TEST_SAMPLES 75


void getElementAtIndexTest(double *timeInSec) {

	__TINY_LIST_INIT_CLEANUP_VALIDATOR__();

	LIST_NODE(uint32_t)* head = CREATE_LINKED_LIST(uint32_t, NODE_COUNT);

	LIST_NODE(uint32_t)* lastNode = head;
	for (uint64_t i = 0; i < NODE_COUNT; ++i) {
		lastNode->value = i;
		lastNode = lastNode->next;
	}

	long long tBegin = getMicroseconds();

	const LIST_NODE(uint32_t)* node = GET_NODE_AT_POSITON(uint32_t, head, TARGET_NODE_POS);

	long long tEnd = getMicroseconds();
	double time_spent = (double)(tEnd - tBegin) / 1000000;

	printf("Value at position %d: %d \n", TARGET_NODE_POS, node->value);

	DESTROY_LINKED_LIST(uint32_t, head);

	__TINY_LIST_VALIDATE_CLEANUP__();

	if (timeInSec) {
		*timeInSec = time_spent;
	}
}

int main() {
	double t = 0;
	for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
		double tt = 0.00L;
		getElementAtIndexTest(&tt);
		t += tt;
	}
	// Note that it may seem to take longer than this, but that's because the time returned by getElementAtIndexTest does not include the time it takes to create or destroy a list, only the time it takes to access elements within it.
	printf("Avg time spent finding the node at position %d in a list of %d elements: %f @ %d samples\n", TARGET_NODE_POS, NODE_COUNT, t / TEST_SAMPLES, TEST_SAMPLES);
}
