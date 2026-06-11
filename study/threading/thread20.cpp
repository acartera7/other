#include <thread>
#include <string>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>
#include <random>
#include <chrono>
#include <syncstream>
#include <algorithm>
#include <numeric>

/* Exercise 20
Atomic vs Mutex Counter Benchmark 
Build a small benchmarking program that:
- launches N threads
- each thread increments a shared counter M times
- using three different counter types
- measures and prints the time for each
- verifies correctness (final count must be N × M)
*/

using ms = std::chrono::milliseconds;

template<typename... Args>
void ts_print(Args&&... args) {
  std::osyncstream out(std::cout);
  (out << ... << args) << '\n';
}

struct alignas(64) PaddedInt {
  std::atomic<int> value;
  char padding[64 - sizeof(int)];
};


void tf_atomic_count(std::atomic<int>& data, size_t count) {
  for(size_t i=0; i<count;++i) {
    data.fetch_add(1,std::memory_order_relaxed);
  }
}

void tf_mutex_count(std::mutex& mtx, int& data, size_t count) {
  for(int i=0; i< count; ++i){
    std::unique_lock<std::mutex> ulock(mtx);
    data++;
  }
}

void tf_falsesharing_counter(std::atomic<int>& data, size_t count) {
  for(int i=0; i<count; ++i) {
    data++;
  }
}

void tf_padded_counter(PaddedInt& data, size_t count) {
  for(int i=0; i<count; ++i) {
    data.value++;
  }
}

int main(void) {

  std::vector<std::thread> threads;
  
  size_t N = 5;
  size_t M = 1000000;
  std::atomic<int> counter;

  /* ===== Atmoic Counter ===== */
  auto start = std::chrono::high_resolution_clock::now();

  for(size_t i=0; i<N; ++i) {
    threads.emplace_back(
      tf_atomic_count,
      std::ref(counter),
      M
    );
  }

  for(auto& t : threads) {
    t.join();
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<ms>(end-start).count();

  
  ts_print("Atomic Counter:");
  ts_print("Time: ", duration, "ms");
  ts_print("Final: ", counter, " (expected 5000000)");
  
  threads.clear();
  counter.store(0);
  
  /* ===== Mutex Counter ===== */

  std::mutex mtx;
  int integral_counter = 0;

  start = std::chrono::high_resolution_clock::now();

  for(size_t i=0; i<N; ++i) {
    threads.emplace_back(
      tf_mutex_count,
      std::ref(mtx),
      std::ref(integral_counter),
      M
    );
  }

  for(auto& t : threads) {
    t.join();
  }

  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<ms>(end-start).count();
  
  ts_print("Mutex Counter:");
  ts_print("Time: ", duration, "ms");
  ts_print("Final: ", integral_counter, " (expected 5000000)");
  
  threads.clear();

  /* ===== FalseSharing Counter ===== */

  std::vector<std::atomic<int>> counters(N);
  for (auto& c : counters) {
    c.store(0, std::memory_order_relaxed);
  }

  start = std::chrono::high_resolution_clock::now();

  for(size_t i=0; i<N; ++i) {
    threads.emplace_back(
      tf_falsesharing_counter,
      std::ref(counters[i]),
      M
    );
  }

  for(auto& t : threads) {
    t.join();
  }

  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<ms>(end-start).count();
  
  int result = std::accumulate(counters.begin(),counters.end(),0);

  ts_print("False Sharing Counter:");
  ts_print("Time: ", duration, "ms");
  ts_print("Final: ", result, " (expected 5000000)");
  
  threads.clear();
  result = 0;
  
  /* ===== Padded Counter ===== */

  std::vector<PaddedInt> padded_counters(N);
  for (PaddedInt& c : padded_counters) {
    c.value.store(0, std::memory_order_relaxed);
  }

  start = std::chrono::high_resolution_clock::now();

  for(size_t i=0; i<N; ++i) {
    threads.emplace_back(
      tf_padded_counter,
      std::ref(padded_counters[i]),
      M
    );
  }

  for(auto& t : threads) {
    t.join();
  }

  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<ms>(end-start).count();
  
  for(PaddedInt& x : padded_counters){
    result += x.value;
  }

  ts_print("Padded Counter:");
  ts_print("Time: ", duration, "ms");
  ts_print("Final: ", result, " (expected 5000000)");
  
  threads.clear();

  return 0;
}

