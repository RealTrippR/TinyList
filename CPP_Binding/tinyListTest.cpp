#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

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

namespace tl {
    #define TINY_LIST_USE_CPP
    #ifndef NDEBUG
        #define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations, it's important that this comes before #include <TinyList/tinyList.h>*/
    #endif // !NDEBUG
    #include <TinyList/tinyList.h>
    DEFINE_LIST_NODE_TYPE(myClass)
    __DEFINE_TINY_LIST_VERIFY_OPERATIONS__(myClass) /*Defines the verification functions*/
    DEFINE_LINKED_LIST_OPERATIONS(myClass);
}

int main() {
#ifndef NDEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // !NDEBUG
    const clock_t tBegin = clock();
    tl::LIST_NODE(myClass)* head = tl::CREATE_LIST_NODE(myClass);

    tl::LIST_NODE(myClass)* lastNode = head;
    for (uint16_t i = 1; i <= 5; ++i) {
        tl::LIST_NODE(myClass)* node = tl::CREATE_LIST_NODE(myClass);
        node->value.val = i;
        INSERT_LIST_NODE(myClass, lastNode, node);
        lastNode = node;
    }

    tl::DESTROY_LINKED_LIST(myClass, head);

    const clock_t tEnd = clock();
    double time_spent = (double)(tEnd - tBegin) / CLOCKS_PER_SEC;
    printf("Time spent: %.5f seconds\n", time_spent);
#ifndef NDEBUG
    _CrtDumpMemoryLeaks();
#endif // !NDEBUG
}