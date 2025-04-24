#include <stdio.h>
#include <stdint.h>

#include <time.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifndef NDEBUG
    #define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations, it's important that this comes before #include <TinyList/tinyList.h>*/
#endif // !NDEBUG
#include <TinyList/tinyList.h>
DEFINE_LIST_NODE_TYPE(uint8_t)
__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(uint8_t) /*Defines the verification functions*/
DEFINE_LINKED_LIST_OPERATIONS(uint8_t);

int main() {
#ifndef NDEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // !NDEBUG

    const clock_t tBegin = clock();


    LIST_NODE(uint8_t)* head = CREATE_LIST_NODE(uint8_t);

    LIST_NODE(uint8_t)* lastNode = head;
    for (uint8_t i = 1; i <= 25; ++i) {
        LIST_NODE(uint8_t)* node = CREATE_LIST_NODE(uint8_t);
        node->value = i;
        INSERT_LIST_NODE(uint8_t, lastNode, node);
        lastNode = node;
    }

    LIST_NODE(uint8_t)* atPos = GET_NODE_AT_POSITON(uint8_t, head, 5);
    printf("Linked list length: %d\n", GET_LIST_LENGTH(uint8_t, head));

    REVERSE_LINKED_LIST(uint8_t, &head);

    DELETE_NODE(uint8_t, head, 1u /*pos of the node to delete*/);

    DESTROY_LINKED_LIST(uint8_t, head);

    const clock_t tEnd = clock();
    double time_spent = (double)(tEnd - tBegin) / CLOCKS_PER_SEC;
	printf("Time spent: %.5f seconds\n", time_spent);

#ifndef NDEBUG
    _CrtDumpMemoryLeaks();
#endif // !NDEBUG
}