#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <syncstream>
#include <vector>
#include <chrono>

/*
Exercise 7 — Synchronize Access to a Shared Variable Using std::mutex
*/

void t_func(long& num, std::mutex& mtx) {
  for(unsigned i=0; i<100000; ++i){
    std::lock_guard<std::mutex> lock(mtx);
    ++num;
  }
}

int main(int argc, char* argv[]) {
  
  if(argc > 1) {

    unsigned N = std::atoi(argv[1]);
    long num = 0;

    std::mutex mtx;
    std::vector<std::thread> threads;

    for(unsigned i=0; i<N; ++i) {
      threads.emplace_back(t_func, std::ref(num), std::ref(mtx));
    }

    for(auto& t : threads ) {
      t.join();
    }

    std::cout << num << std::endl;
    
  }
  return 0;
}