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

/*
Exercise 19 - Build a Minimal Thread Pool
*/

using ms = std::chrono::milliseconds;

template<typename... Args>
void ts_print(Args&&... args) {
  std::osyncstream out(std::cout);
  (out << ... << args) << '\n';
}

void compute_50(int id) {
  ts_print("Thread ", id, " computing for 50ms");
  std::this_thread::sleep_for(ms(50));
  return;
} 
void compute_100(int id){
  ts_print("Thread ", id, " computing for 100ms");
  std::this_thread::sleep_for(ms(100));
  return;
}  
void compute_200(int id) {
  ts_print("Thread ", id, " computing for 200ms");
  std::this_thread::sleep_for(ms(200));
  return;
} 
void compute_500(int id) {
  ts_print("Thread ", id, " computing for 500ms");
  std::this_thread::sleep_for(ms(500));
  return;
}

class ThreadPool {
private:
  std::mutex mtx;
  std::condition_variable cv;
  
  std::vector<std::thread> threads;
  std::queue<std::function<void(int)>> task_queue;

  bool stop;

  void tf_execute(int id) {
    while(true){

      std::function<void(int)> task;
      {
        std::unique_lock<std::mutex> ulock(mtx);
        
        cv.wait(ulock,[&]{return !task_queue.empty() || stop;});
        
        // don't exit until all tasks have at least one dedicated thread
        if (stop && task_queue.size() < threads.size()) { 
          ts_print("Thread ", id, " exiting.");
          return;
        }
        
        task = task_queue.front();
        task_queue.pop();
      }
      
      task(id);
      
    }
  }

public:
  ThreadPool(size_t n = std::thread::hardware_concurrency()) : 
      mtx(), cv(), threads(), task_queue(), stop(false) {
    for(int i=0; i<n; ++i) {
      threads.emplace_back(&ThreadPool::tf_execute, this, i);
    }
  }

  void submit(std::function<void(int)> f) {
    if(stop) {
      ts_print("Submission failed -- ThreadPool is shutting down.");
      return;
    }
    {
      std::unique_lock<std::mutex> ulock(mtx);
      task_queue.push(f);
    }
    ts_print("Task Submitted -- Task Queue size: ", task_queue.size());
    
    cv.notify_one();
  }

  void shutdown() {
    {
      std::unique_lock<std::mutex> ulock(mtx);
      stop = true;
    }
    cv.notify_all();
    ts_print("===== Shutdown Activated =====");
    for(auto& t : threads) {
      t.join();
    }
  }
};

int main(void) {
  size_t N = 4;

  void (*task_array[4])(int) = {compute_50,compute_100,compute_200,compute_500};
  
  std::random_device rd; 
  std::mt19937 gen(rd());

  std::uniform_int_distribution dist1(0,3);
  std::uniform_int_distribution dist2(30,50);
  ThreadPool tp(N);
  ts_print("Initializing Thread Pool with ", N, " threads.");
  
  for(int i=0; i<50; ++i){    
    tp.submit(task_array[dist1(gen)]);
    std::this_thread::sleep_for(ms(dist2(gen))); 
  } 
  
  tp.shutdown();  //NOTE: BLOCKING

  return 0;
}

