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

/* Exercise 21
A CountdownTimer class whose worker thread is a 
member function, and each thread uses thread_local state.
*/
 
using ms = std::chrono::milliseconds;

template<typename... Args>
void ts_print(Args&&... args) {
  std::osyncstream out(std::cout);
  (out << ... << args) << '\n';
}

void onFinished() {
  ts_print("Times Up!");
}

class CountdownTimer {
  std::thread worker;
  std::mutex mtx;
  std::condition_variable cv_ispaused;
  bool stop_flag;
  bool pause_flag;
  float duration;
  float current_time;
  std::atomic<bool> running;
  std::function<void()> callback;

  void worker_loop() {
    auto last = std::chrono::steady_clock::now();
    std::unique_lock<std::mutex> ulock(mtx);
    {
      while(!stop_flag && current_time > 0.f) {
        
        cv_ispaused.wait(ulock, [&]{return !pause_flag || stop_flag;});
        if(stop_flag)
          break;  
        

        ulock.unlock();
        std::this_thread::sleep_for(ms(10));
        ulock.lock();
          
        auto now = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration_cast<ms>(now - last);
        current_time-=float(dt.count()/1000.0f);
        last = std::chrono::steady_clock::now();
        
        if(current_time <= 0.0f){
          stop_flag = true;
          break;
        }
      }
    }
    if(callback)
      callback();

    running.store(false);
  }
    
public:
  CountdownTimer(float time = 10.0f, std::function<void()> cb = std::function<void()>() ) :
      worker(), mtx(), cv_ispaused(), 
      stop_flag(false), pause_flag(false),
      duration(time), current_time(0), 
      running(false), callback(cb)  {
    
  }

  ~CountdownTimer() {
    stop();
  }

  void stop(){
    if(running.load()) {
      {
        std::unique_lock<std::mutex> ulock(mtx);
        stop_flag = true;
      }
      cv_ispaused.notify_one();
      worker.join();
    }
  }

  void set_time(float time) {
    if(!running.load()) {
      duration = time;
    }
  };
  
  void pause() {
    if(running.load()){
      std::unique_lock<std::mutex> ulock(mtx);
      pause_flag = true;
    }
  }

  void unpause() {
    
    if(running.load()){
      {
        std::unique_lock<std::mutex> ulock(mtx);
        if(!pause_flag)
          return;
        pause_flag = false;
      }
      cv_ispaused.notify_one();
    }
  }

  void start() {
    if(!running.load()) {
      running.store(true);
      current_time = duration;
      worker = std::thread(&CountdownTimer::worker_loop, this);
    }
  }

  float get_current_time() {
    std::unique_lock<std::mutex> ulock(mtx);
    return current_time;
  }
  
  bool isRunning(){ return running.load(); }
};



int main(void) {
  
  float t = 10.0f;
  CountdownTimer timer(t);
  ts_print("Timer set for ", t, " seconds");
  
  
  ts_print("Starting Timer");
  timer.start();

  while(timer.isRunning()) {
    std::cout << std::fixed << std::setprecision(2) << timer.get_current_time() << '\r' ;
    std::this_thread::sleep_for(ms(10));
  }

  //ts_print("Press any key to exit...");
  //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  //std::cin.get(); 

  return 0;
}

