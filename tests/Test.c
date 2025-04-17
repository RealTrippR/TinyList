#include <stdio.h>
#include <stdint.h>

#include <time.h>
#include "../tinyList.h"
DEFINE_LIST_NODE_TYPE(uint8_t)
#ifndef NDEBUG
#define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations*/
__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(uint8_t) /*Defines the verification functions*/
#endif
DEFINE_LINKED_LIST_OPERATIONS(uint8_t);


int main() {
    const clock_t tBegin = clock();
#ifndef NDEBUG
    __TINY_LIST_INIT_CLEANUP_VALIDATOR__();
#endif // !NDEBUG

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

#ifndef NDEBUG
    __TINY_LIST_VALIDATE_CLEANUP__();
#endif
    const clock_t tEnd = clock();
    double time_spent = (double)(tEnd - tBegin) / CLOCKS_PER_SEC;
	printf("Time spent: %.5f seconds\n", time_spent);

}