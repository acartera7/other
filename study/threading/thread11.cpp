#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <vector>

/*
Exercise 11 — Create a Thread‑Safe Counter Using std::mutex
*/

class Counter
{
private:
  unsigned value;
  std::mutex mtx;
  
public:
  Counter();
  void increment();
  unsigned get();
  ~Counter();
};

Counter::Counter() : value(0) {}

void Counter::increment() {
  std::lock_guard<std::mutex> lock(mtx);
  ++value;
};

unsigned Counter::get() { 
  std::lock_guard<std::mutex> lock(mtx);
  return value;
}

Counter::~Counter() {

}

void t_func(Counter& c) {
  for(int i=0; i<100000; ++i) {
    c.increment();
  }
}


int main(int argc, char* argv[]) {
  
  if(argc > 1) {

    unsigned N = std::atoi(argv[1]);

    std::vector<std::thread> threads;

    Counter counter;

    for(unsigned i=0; i<N; ++i) {
      threads.emplace_back(t_func, std::ref(counter));
    }

    for(auto& t : threads ) {
      t.join();
    }

    std::cout << counter.get() << std::endl;
    
  }
  return 0;
}