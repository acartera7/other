#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <syncstream>
#include <vector>
#include <chrono>


void t_func(long& num) {
  for(unsigned i=0; i<100000; ++i){
    ++num;
  }
}

int main(int argc, char* argv[]) {
  
  if(argc > 1) {

    unsigned N = std::atoi(argv[1]);
    long num = 0;

    std::vector<std::thread> threads;

    for(unsigned i=0; i<N; ++i) {
      threads.emplace_back(t_func, std::ref(num));
    }

    for(auto& t : threads ) {
      t.join();
    }

    std::cout << num << std::endl;
    
  }
  return 0;
}