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

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>
#include <stdint.h>

#include "get_time_us.h";

#ifndef NDEBUG
	#define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations, it's important that this comes before #include <TinyList/tinyList.h>*/
	
#endif // !NDEBUG

#include <TinyList/tinyList.h>
DEFINE_LIST_NODE_TYPE(uint8_t)
__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(uint8_t) /*Defines the verification functions*/
DEFINE_LINKED_LIST_OPERATIONS(uint8_t);


#define NODE_COUNT 150000

#define TEST_SAMPLES 25

// single-threaded
void runSTcreateListTest(double* timeInSec) {

	long long tBegin = getMicroseconds();

	LIST_NODE(uint8_t)* head = CREATE_LIST_NODE(uint8_t);

	LIST_NODE(uint8_t)* lastNode = head;

	for (uint64_t i = 0; i < NODE_COUNT; ++i) {
		LIST_NODE(uint8_t)* newNode = CREATE_LIST_NODE(uint8_t);
		INSERT_LIST_NODE(uint8_t, lastNode, newNode);
		lastNode = newNode;
	}

	long long tEnd = getMicroseconds();
	double time_spent = (double)(tEnd - tBegin) / 1000000;
	
	DESTROY_LINKED_LIST(uint8_t, head);


	if (timeInSec) {
		*timeInSec = time_spent;
	}
}
// multi-threaded
void runMTcreateListTest(double* timeInSec) {

	long long tBegin = getMicroseconds();

	LIST_NODE(uint8_t)* head = CREATE_LINKED_LIST(uint8_t, NODE_COUNT);

	long long tEnd = getMicroseconds();
	double time_spent = (double)(tEnd - tBegin) / 1000000;
	
	DESTROY_LINKED_LIST(uint8_t, head);


	if (timeInSec) {
		*timeInSec = time_spent;
	}
}

int main() {
#ifndef NDEBUG
	__TINY_LIST_INIT_CLEANUP_VALIDATOR__();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // !NDEBUG
	{
		double t = 0;
		for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
			double tt = 0.00L;
			runSTcreateListTest(&tt);
			t += tt;
		}
		printf("Single Theaded - Avg Time spent: %.6f seconds - %d nodes allocated per test @ %d samples\n", t / TEST_SAMPLES, NODE_COUNT, TEST_SAMPLES);

		t = 0;
		for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
			double tt = 0.00L;
			runMTcreateListTest(&tt);
			t += tt;
		}
		printf("Multithreaded - %d Threads - Avg Time spent: %.6f seconds - %d nodes allocated per test @ %f samples\n", TINY_LIST_THREAD_COUNT, t / TEST_SAMPLES, NODE_COUNT, TEST_SAMPLES);
	}
#ifndef NDEBUG
	__TINY_LIST_VALIDATE_CLEANUP__();
	_CrtDumpMemoryLeaks();
#endif // !NDEBUG

}