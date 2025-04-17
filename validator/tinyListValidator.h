/*
Copyright � 2025 Tripp Robins

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the �Software�), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TINY_LIST_VALIDATOR
#define TINY_LIST_VALIDATOR

#include <stdlib.h>
#include <assert.h>

#include "hashmap.h"

struct TINYLIST_CLEANUP_VALIDATOR {
    struct hashmap* addressHashMap;
} TLcleanupValidator = { NULL };
/**********************************************************************************/\

int TINYLIST_validator_voidptr_compare(const void* a, const void* b, void* udata) {
    return ((*(char**)a) - (*(char**)b));
}

uint64_t TINYLIST_validator_voidptr_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    return hashmap_xxhash3(item, sizeof(item), seed0, seed1);
}

void __TINY_LIST_INIT_CLEANUP_VALIDATOR__() {
    TLcleanupValidator.addressHashMap = hashmap_new(sizeof(void*), 0, 0, 0,
        TINYLIST_validator_voidptr_hash, TINYLIST_validator_voidptr_compare, NULL, NULL);
}

enum TINYLIST_RETURN_CODE __TINY_LIST_VALIDATE_CLEANUP__() {
    if (hashmap_count(TLcleanupValidator.addressHashMap) > 0) {
        printf("TINYLIST: MEMORY LEAK: Not all allocated list nodes have been freed, %d remains allocated!\nAllocated nodes:\n", hashmap_count(TLcleanupValidator.addressHashMap));
        size_t iter = 0;
        void* item;
        while (hashmap_iter(TLcleanupValidator.addressHashMap, &iter, &item)) {
            printf("\tnode @ memory address: %p\n", item);
        }
        assert(0 && "TINYLIST: MEMORY LEAK: Not all allocated list nodes have been freed!");
    }
    hashmap_free(TLcleanupValidator.addressHashMap);
    return TINYLIST_SUCCESS;
}


#ifdef __TINY_LIST_VERIFY_OPERATIONS__

#define __DEFINE_TINY_LIST_VERIFY_OPERATIONS__(NODE_TYPE)\
void validateNodeCreation_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* node)\
{\
    if (!node) {assert(0&&"TINY LIST: Failed to create and allocate node!");}\
}\
\
void validateGetNodeAtPosition_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* head, const uint32_t pos)\
{\
    uint64_t len = getLinkedListLength_##NODE_TYPE(head)-1;\
    if (pos > len) {\
    printf("TINY LIST: INVALID OPERATION (OUT OF BOUNDS): Cannot get a node at a position %d that is outside the list of length %d!\n", pos, len);\
    assert(0&&"TINY LIST: INVALID OPERATION (OUT OF BOUNDS)");\
    }\
}\
void validateListCleanup_##NODE_TYPE\
(LIST_NODE(NODE_TYPE)* node)\
{\
if (node==NULL) {printf("TINY LIST: ERROR: Attempted to delete NULL node!\n"); assert(0&&"TINY LIST: ERROR: Attempted to delete NULL node!");}\
}


#define VALIDATE_NODE_CREATION(NODE_TYPE, node) validateNodeCreation_##NODE_TYPE(node)
#define VALIDATE_GET_NODE_AT_POSITION(NODE_TYPE, node, pos) validateGetNodeAtPosition_##NODE_TYPE(node, pos)
#define VALIDATE_LIST_CLEANUP(NODE_TYPE, node) validateListCleanup_##NODE_TYPE(node)

#else

#define __DEFINE_TINY_LIST_VERIFY_OPERATIONS__(NODE_TYPE)

#define VALIDATE_NODE_CREATION(NODE_TYPE, node)  
#define VALIDATE_GET_NODE_AT_POSITION(NODE_TYPE, node, pos)  
#define VALIDATE_LIST_CLEANUP(NODE_TYPE, node)

#endif

#endif // !TINY_LIST_VALIDATOR
