#include <stdio.h>
#include <stdint.h>
#include "listNode.h"

DEFINE_LIST_NODE_TYPE(uint8_t)

int main() {
    LIST_NODE(uint8_t)* head = CREATE_LIST_NODE(uint8_t);

    LIST_NODE(uint8_t)* lastNode = head;
    for (uint8_t i = 1; i <= 19; ++i) {
        LIST_NODE(uint8_t)* node = CREATE_LIST_NODE(uint8_t);
        node->value = i;
        INSERT_LIST_NODE(uint8_t, lastNode, node);
        lastNode=node;
    }
    
    LIST_NODE(uint8_t)* atPos = GET_NODE_AT_POSITON(uint8_t, head, 5, UINT32_MAX);
    printf("Linked list length: %d\n", GET_LIST_LENGTH(uint8_t, head));

    REVERSE_LINKED_LIST(uint8_t, &head);

    DESTROY_LINKED_LIST(uint8_t, head);
}