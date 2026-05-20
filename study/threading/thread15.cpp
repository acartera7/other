#include <thread>
#include <string>
#include <iostream>
#include <mutex>
#include <vector>
#include <future>
#include <random>
#include <chrono>
#include <syncstream>

/*
Exercise 15 - Use std::async and std::future to get result from a thread.
*/

using ms = std::chrono::milliseconds;

std::random_device rd; 
std::mt19937 gen(rd());

template<typename... Args>
void ts_print(Args&&... args) {
    std::osyncstream out(std::cout);
    (out << ... << args) << '\n';
}

int bogo_compute(int beg = 0, int end = std::numeric_limits<int>::max()){
  if(beg < 0) return -1;

  std::uniform_int_distribution<> distrib(0, std::numeric_limits<int>::max());
  
  int r = -1; 
  do {
    r = distrib(gen);
    //std::cout << "trying: " << r << std::endl;
  } while(!(beg < r && r < end));
  return r;
}

int slow_add(int a, int b) {
  
  std::uniform_int_distribution<> distrib(500, 2000);
  
  std::this_thread::sleep_for(ms(distrib(gen)));
  ts_print("Task finished: ", a+b);
  
  return a+b;
}

void sum_range(const std::vector<int>& data,
    std::promise<int> result_promise) {
  int sum = 0;
  for(int x : data) {
    sum += x;
  }
  result_promise.set_value(sum);
}

int parallel_sum(const std::vector<int>& data) {
  int i_half = data.size()/2;
  std::vector v1(data.begin(),data.begin()+i_half);
  std::vector v2(data.begin()+i_half,data.end());

  std::promise<int> p1;
  std::promise<int> p2;
  std::future<int> f1 = p1.get_future();
  std::future<int> f2 = p2.get_future();

  std::thread t1(sum_range, std::ref(v1), std::move(p1));
  std::thread t2(sum_range, std::ref(v2), std::move(p2));

  int r1 = f1.get();
  int r2 = f2.get();

  t1.join();
  t2.join();
  
  return r1+r2;
}

int main(void) {

  std::future fut = std::async(bogo_compute, 200,300);

  ts_print("Starting Single Computation...");
  
  auto start = std::chrono::high_resolution_clock::now();
  
  //fut.wait_for(std::chrono::milliseconds(500));
  
  ts_print("Result: ",fut.get());
  
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<ms>(end - start);
  
  ts_print("Computation Time: ",duration.count()," ms");

  std::vector<std::future<int>> asyncs;

  ts_print("Starting multiple computations...");
  for (int i=1; i<=5; ++i) {
    asyncs.emplace_back(std::async(slow_add, i*5, i*2));
  }

  std::vector<int> results;
  for(auto& fut : asyncs) {
    results.push_back(fut.get());
  }

  ts_print("Multiple computations finished.");

  for(int x: results) {
    ts_print("Result: ", x);
  }

  std::vector<int> v = {1,2,3,4,5,6,7,8};

  int result = parallel_sum(v);

  ts_print("Parallel sum: ", result);

  return 0;
}