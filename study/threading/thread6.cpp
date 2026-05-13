#include <thread>
#include <string>
#include <iostream>
#include <syncstream>
#include <vector>
#include <chrono>

void print_vector(const std::vector<uint64_t>& v) {
  for(const auto& e : v) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
}

void t_fact(unsigned N, uint64_t& result) {  
  result = 1;
  if(N == 0)
    return;
  
  for(unsigned i=1; i<=N;++i) {
    result *= i;
  }
  
}

int main(int argc, char* argv[]) {
  
  if(argc > 1) {
    unsigned N = std::atoi(argv[1]); 
    if(N <= 0) return 0;

    std::vector<std::thread> threads;
    
    std::vector<uint64_t> output(N,0);
    
    for(unsigned i=1; i<=N; ++i) {
      threads.push_back(std::thread(t_fact, i,std::ref(output[i-1])));
    }

    for (auto& t : threads)
      t.join();

    print_vector(output);
    
  }

  return 0;
}