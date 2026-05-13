#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>

struct alignas(64) Padded {
  uint64_t value = 0;
};

void tf_sum(size_t beg, size_t end, std::vector<int>& v, uint64_t& out) {
  for(size_t i=beg; i<end; ++i) {
    out += v[i];
  }
}

int main(int argc, char* argv[]) {

  if(argc < 2) return 0;

  int N = 100000000;
  std::vector<int> v(N,1);  // vector to calculate
  
  size_t T = atoi(argv[1]); // number of multi threads
  unsigned range = N / T;   // division of work
  std::vector<std::thread> multi_threads;
  
  
  uint64_t output1 = 0;     // outputs
  std::vector<Padded> partial_sums(T,Padded(0));
  
  
  /////////  ONE THREAD COMPUTATION ////////////////////////////////////////////
  auto start = std::chrono::high_resolution_clock::now();
  std::thread single_thread(tf_sum, 0,N, std::ref(v), std::ref(output1));
  single_thread.join();
  auto end = std::chrono::high_resolution_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  ///////  END ONE THREAD COMPUTATION //////////////////////////////////////////

  std::cout << "Single-threaded computation of " << N << std::endl;
  std::cout << "Resulting sum: " << output1 << std::endl;
  std::cout << "Took " << ms << " milliseconds." << std::endl;
  
  std::cout << std::endl;

  ////////  MULTI THREAD COMPUTATION ///////////////////////////////////////////
  start = std::chrono::high_resolution_clock::now();
  for(size_t i=0; i<T; ++i) {
    multi_threads.emplace_back(tf_sum, i*range,(i+1)*range, std::ref(v), std::ref(partial_sums[i].value));
  }
  for(auto& t : multi_threads) 
    t.join();
  end = std::chrono::high_resolution_clock::now();
  ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  //////  END MULTI THREAD COMPUTATION /////////////////////////////////////////

  uint64_t output2;
  for(Padded s : partial_sums) {
    output2 += s.value;
  }

  std::cout << "Multi-threaded computation of " << N << "  with " << T << " threads." << std::endl;
  std::cout << "Resulting sum: " << output2 << std::endl;
  std::cout << "Thread partial sums:" << std::endl;
  for(Padded s : partial_sums) {
    std::cout << s.value << std::endl;
  }
  std::cout << "Took " << ms << " milliseconds." << std::endl;
  
  return 0;
}