
#include <iostream>

class myClass {
public:
	myClass() {
        static uint16_t c = 0;
		std::cout << c << " - Constructor called\n";
        c++;
	}
	~myClass() {
        static uint16_t c = 0;
        std::cout << c << " - Destructor called\n";
        c++;
	}
	char val = 0;
};


#include <stdio.h>
#include <stdint.h>

#include <time.h>

#define TINY_LIST_USE_CPP_NEW_AND_FREE
#ifndef NDEBUG
    #define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations, it's important that this comes before #include <TinyList/tinyList.h>*/
#endif // !NDEBUG
#include <TinyList/tinyList.h>
DEFINE_LIST_NODE_TYPE(myClass)
__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(myClass) /*Defines the verification functions*/
DEFINE_LINKED_LIST_OPERATIONS(myClass);

int main() {
    const clock_t tBegin = clock();
    __TINY_LIST_INIT_CLEANUP_VALIDATOR__();

    LIST_NODE(myClass)* head = CREATE_LIST_NODE(myClass);

    LIST_NODE(myClass)* lastNode = head;
    for (uint16_t i = 1; i <= 5; ++i) {
        LIST_NODE(myClass)* node = CREATE_LIST_NODE(myClass);
        node->value.val = i;
        INSERT_LIST_NODE(myClass, lastNode, node);
        lastNode = node;
    }

    DESTROY_LINKED_LIST(myClass, head);

    __TINY_LIST_VALIDATE_CLEANUP__();

    const clock_t tEnd = clock();
    double time_spent = (double)(tEnd - tBegin) / CLOCKS_PER_SEC;
    printf("Time spent: %.5f seconds\n", time_spent);

}