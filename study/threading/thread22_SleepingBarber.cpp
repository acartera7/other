#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>
#include <iostream>
#include <queue>
#include <chrono>
#include <array>
#include <random>
#include <atomic>
#include <syncstream>

/*
=== Sleeping Barber ===

*/

std::mutex t_print_mtx;

template<typename... Args>
void ts_print(Args&&... args) {
    std::lock_guard<std::mutex> lock(t_print_mtx);
    (std::cout << ... << args) << std::endl;
}

int main(void) {
    
    return 0;
  }