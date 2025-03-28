#include <stdlib.h>
#include <assert.h>

/**********************************************************************************/\

#ifdef __TINY_LIST_VERIFY_OPERATIONS__

#define DEFINE_TINY_LIST_VERIFY_OPERATIONS__(NODE_TYPE)\
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

#define VALIDATE_NODE_CREATION(NODE_TYPE, node) validateNodeCreation_##NODE_TYPE(node)
#define VALIDATE_GET_NODE_AT_POSITION(NODE_TYPE, node, pos) validateGetNodeAtPosition_##NODE_TYPE(node, pos)

#else

#define DEFINE_TINY_LIST_VERIFY_OPERATIONS__(NODE_TYPE)

#define VALIDATE_NODE_CREATION(NODE_TYPE, node)  
#define VALIDATE_GET_NODE_AT_POSITION(NODE_TYPE, node, pos)  

#endif