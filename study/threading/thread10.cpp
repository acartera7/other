#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <vector>


void t_func(long& num1, long& num2, std::mutex& mtx) {
  std::unique_lock<std::mutex> ulock(mtx);
  
  ++num1;
  ulock.unlock();

  for(unsigned i=0; i<100000; ++i){
    ++num2;
  }

  ulock.lock();
  ++num1;
  ulock.unlock();

}

int main(int argc, char* argv[]) {
  
  if(argc > 1) {

    unsigned N = std::atoi(argv[1]);
    long num1 = 0;
    long num2 = 0;

    std::mutex mtx;
    std::vector<std::thread> threads;

    for(unsigned i=0; i<N; ++i) {
      threads.emplace_back(t_func, std::ref(num1),std::ref(num2), std::ref(mtx));
    }

    for(auto& t : threads ) {
      t.join();
    }

    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    
  }
  return 0;
}