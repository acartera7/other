#include <thread>
#include <string>
#include <iostream>
#include <mutex>
#include <vector>
#include <future>
#include <random>
#include <chrono>
#include <syncstream>
#include <algorithm>

/*
=== Exercise 18 - Readers/Writers Problem ===
*/

using ms = std::chrono::milliseconds;

std::random_device rd; 
std::mt19937 gen(rd());

template<typename... Args>
void ts_print(Args&&... args) {
    std::osyncstream out(std::cout);
    (out << ... << args) << '\n';
}

int main(void) {


  return 0;
}

