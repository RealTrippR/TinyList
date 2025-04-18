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

DEFINE_LIST_NODE_TYPE(uint32_t)
#ifndef NDEBUG
#define __TINY_LIST_VERIFY_OPERATIONS__ /*Enables verification of TinyList operations*/
__DEFINE_TINY_LIST_VERIFY_OPERATIONS__(uint32_t) /*Defines the verification functions*/
#endif
DEFINE_LINKED_LIST_OPERATIONS(uint32_t);


#define NODE_COUNT 10000
#define TARGET_NODE_POS NODE_COUNT-1
#define TEST_SAMPLES 150


void getElementAtIndexTest(double *timeInSec) {

	__TINY_LIST_INIT_CLEANUP_VALIDATOR__();

	LIST_NODE(uint32_t)* head = CREATE_LINKED_LIST(uint32_t, NODE_COUNT);

	LIST_NODE(uint32_t)* lastNode = head;
	for (uint64_t i = 0; i < NODE_COUNT; ++i) {
		lastNode->value = i;
		lastNode = lastNode->next;
	}



	long long tBegin = getMicroseconds();

	const LIST_NODE(uint32_t)* node = GET_NODE_AT_POSITON(uint32_t, head, TARGET_NODE_POS);

	long long tEnd = getMicroseconds();
	double time_spent = (double)(tEnd - tBegin) / 1000000;

	printf("Value at position %d: %d \n", TARGET_NODE_POS, node->value);

	DESTROY_LINKED_LIST(uint32_t, head);

	__TINY_LIST_VALIDATE_CLEANUP__();

	if (timeInSec) {
		*timeInSec = time_spent;
	}
}

int main() {
	double t = 0;
	for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
		double tt = 0.00L;
		getElementAtIndexTest(&tt);
		t += tt;
	}
	// Note that it may seem to take longer than this, but that's because the time returned by getElementAtIndexTest does not include the time it takes to create or destroy a list, only the time it takes to access it.
	printf("Avg time spent finding the node at position %d in a list of %d elements: %f @ %d samples\n", TARGET_NODE_POS, NODE_COUNT, t / TEST_SAMPLES, TEST_SAMPLES);
}