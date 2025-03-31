## Tiny List ##
Tiny list is a simple C-macro based implementation of a linked list.
It supports basic operations, such as node insertion and deletion, and linked list reversing, searching, and length retrieval.

## Example ##


## Operations ##
```
## Defines the node struct for this type
DEFINE_LIST_NODE_TYPE(nodeType)

## Defines the linked list operations for this type
DEFINE_LINKED_LIST_OPERATIONS(nodeType)

## Allocates and returns a 0-initialized node
CREATE_LIST_NODE(NODE_TYPE)()

## This recursively destroys every node in the list, starting at the head
DESTROY_LINKED_LIST(NODE_TYPE, node)

## Reverses the linked list, starting at the head
REVERSE_LINKED_LIST(NODE_TYPE, head)

## Inserts a node after prev. (prev->next will equal node, and node->next will reference the node that prev->next used to point to)
INSERT_LIST_NODE(NODE_TYPE, prev, node)

## Returns the node at the position if it exists, if it does not exist it will return NULL
GET_NODE_AT_POSITON(NODE_TYPE, head, pos)

## Returns the position of the node in the list. If the node not a descendant of the head it will return -1
GET_POSITION_OF_NODE(NODE_TYPE, head, node)

## Returns the length of the linked list, starting from the head
GET_LIST_LENGTH(NODE_TYPE, head)

Deallocates the node offsetFromPrevious nodes after prev
## DELETE_NODE(NODE_TYPE, prev, offsetFromPrevious)
```

## Validation ##
Tiny list has optional protection againt invalid operations and memory leaks.

```
## Enables verification of TinyList operations
#define __TINY_LIST_VERIFY_OPERATIONS__

## Defines the verification functions
__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(uint8_t) 
```
