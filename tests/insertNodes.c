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

#include "get_time_µs.h";

#include <TinyList/tinyList.h>

DEFINE_LIST_NODE_TYPE(uint8_t)
#ifndef NDEBUG
#define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations*/
__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(uint8_t) /*Defines the verification functions*/
#endif
DEFINE_LINKED_LIST_OPERATIONS(uint8_t);


#define NODE_COUNT 200000

#define TEST_SAMPLES 150

// single-threaded
void runSTcreateListTest(double* timeInSec) {
	long long tBegin = getMicroseconds();

	__TINY_LIST_INIT_CLEANUP_VALIDATOR__();

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

	__TINY_LIST_VALIDATE_CLEANUP__();

	if (timeInSec) {
		*timeInSec = time_spent;
	}
}
// multi-threaded
void runMTcreateListTest(double* timeInSec) {

	__TINY_LIST_INIT_CLEANUP_VALIDATOR__();

	long long tBegin = getMicroseconds();

	const LIST_NODE(uint8_t)* l2Head = CREATE_LINKED_LIST(uint8_t, NODE_COUNT);

	LIST_NODE(uint8_t)* lastNode = l2Head;


	long long tEnd = getMicroseconds();
	double time_spent = (double)(tEnd - tBegin) / 1000000;
	
	DESTROY_LINKED_LIST(uint8_t, l2Head);

	__TINY_LIST_VALIDATE_CLEANUP__();

	if (timeInSec) {
		*timeInSec = time_spent;
	}
}

int main() {
	{
		double t = 0;
		for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
			double tt = 0.00L;
			runSTcreateListTest(&tt);
			t += tt;
		}
		printf("Single Thead - Avg Time spent: %.6f seconds - %d nodes allocated per test @ %d samples\n", t / TEST_SAMPLES, NODE_COUNT, TEST_SAMPLES);

		t = 0;
		for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
			double tt = 0.00L;
			runMTcreateListTest(&tt);
			t += tt;
		}
		printf("Multithreaded - %d Threads - Avg Time spent: %.6f seconds - %d nodes allocated per test @ %d samples\n", TINY_LIST_THREAD_COUNT, t / TEST_SAMPLES, NODE_COUNT, TEST_SAMPLES);
	}
}