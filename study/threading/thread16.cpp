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
Exercise 16 - Run multiple asynchronous tasks and wait for results.
*/

using ms = std::chrono::milliseconds;

std::random_device rd; 
std::mt19937 gen(rd());

template<typename... Args>
void ts_print(Args&&... args) {
    std::osyncstream out(std::cout);
    (out << ... << args) << '\n';
}

std::pair<int,long long> bogo_compute(int beg = 0, int end = std::numeric_limits<int>::max()){
  if(beg < 0) return {};
  
  auto start_time = std::chrono::high_resolution_clock::now();

  std::uniform_int_distribution<> distrib(0, std::numeric_limits<int>::max());
  
  int r = -1; 
  do {
    r = distrib(gen);
    //std::cout << "trying: " << r << std::endl;
  } while(!(beg < r && r < end));

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<ms>(end_time - start_time);
  
  //ts_print("Computation Time: ",duration.count()," ms");

  return std::pair<int,long long>(r,duration.count());
}


int main(void) {

  ts_print("Launching Asyncs...");
  auto start = std::chrono::high_resolution_clock::now();
  
  std::vector<std::future<std::pair<int,long long>>> futures;

  for(int i=1; i<=3; i++ ) {
    ts_print("Bogo_compute with range: ", i*200, " - ",i*250);
    futures.push_back(std::async(std::launch::async, bogo_compute, i*200,i*250));
  }
  
  std::vector<std::pair<int,long long>> results;
  for (auto& f : futures) {
    results.push_back(f.get());
  }

  std::sort(results.begin(), results.end(), 
      [](std::pair<int,long long> p1, std::pair<int,long long> p2) { 
        return p1.second < p2.second;
      });
  
  for(auto[result, duration] : results) {
    ts_print("Task finished with result: ",result, " | Duration: ", duration );
  }
  //ts_print("Result: ",);
  
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<ms>(end - start);
  
  ts_print("Total computation Time: ",duration.count()," ms");

  return 0;
}

