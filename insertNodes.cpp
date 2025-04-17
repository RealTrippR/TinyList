#include <iostream>
#include <chrono>
#include <cstdint>
#include <list>
#include <thread>
#include <vector>
#include <mutex>

#define NODE_COUNT 200000
#define TEST_SAMPLES 150
#define THREAD_COUNT 4

// single-threaded
void runSTcreateListTest(double& timeInSec) {
    auto start = std::chrono::high_resolution_clock::now();

    std::list<uint8_t> linkedList;

    for (uint64_t i = 0; i < NODE_COUNT; ++i) {
        linkedList.push_back(static_cast<uint8_t>(i));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    timeInSec = elapsed.count();
}

// multithreaded
void runMTcreateListTest(double& timeInSec) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    std::vector<std::list<uint8_t>> threadLists(THREAD_COUNT);

    // Function to be executed by each thread
    auto createListInThread = [&](int threadIndex) {
        for (uint64_t i = 0; i < NODE_COUNT / THREAD_COUNT; ++i) {
            threadLists[threadIndex].push_back(static_cast<uint8_t>(i + threadIndex * (NODE_COUNT / THREAD_COUNT)));
        }
    };

    // Create threads
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.push_back(std::thread(createListInThread, i));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }


    for (uint8_t i = 1; i < threadLists.size(); ++i) {
        threadLists[0].splice(threadLists[0].end(), threadLists[1]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    timeInSec = elapsed.count();
}

int main() {
    {
        double t = 0;
        for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
            double tt = 0.00L;
            runSTcreateListTest(tt);
            t += tt;
        }
        std::cout << "Single Threaded - Avg Time spent: " << t / TEST_SAMPLES << " seconds - " << NODE_COUNT << " nodes allocated per test\n";
    }

    {
        double t = 0;
        for (uint16_t i = 0; i < TEST_SAMPLES; ++i) {
            double tt = 0.00L;
            runMTcreateListTest(tt);
            t += tt;
        }
        std::cout << "Multithreaded - Avg Time spent: " << t / TEST_SAMPLES << " seconds - " << NODE_COUNT << " nodes allocated per test\n";
    }

    return 0;
}
