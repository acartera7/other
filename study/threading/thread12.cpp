#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>
#include <iostream>
#include <vector>

void print_vector(const std::vector<unsigned>& v) {
  for(unsigned i : v) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}

void tf_odd(unsigned N, std::mutex& mtx,std::vector<unsigned>& output, std::condition_variable& cv, bool& odd_turn) {
  
  for(unsigned i=1; i<=N; i+=2){
    {
      std::unique_lock<std::mutex> ulock(mtx);
      cv.wait(ulock, [&odd_turn]{return odd_turn;});
      output.push_back(i);
      odd_turn = false;
    }
    cv.notify_one();
  }
}

void tf_even(unsigned N, std::mutex& mtx, std::vector<unsigned>& output, std::condition_variable& cv, bool& odd_turn) {
  
  for(unsigned i=0; i<=N; i+=2){
    {
      std::unique_lock<std::mutex> ulock(mtx);
      cv.wait(ulock, [&odd_turn]{return !odd_turn;});
      output.push_back(i);
      odd_turn = true;
    }
    cv.notify_one();
  }
}

int main(int argc, char* argv[]) {
  
  if(argc > 1) {

    unsigned N = std::atoi(argv[1]);

    //std::vector<std::thread> threads;
    
    std::mutex mtx;
    std::condition_variable cv;
    bool odd_turn = false;
    std::vector<unsigned> output;

    std::thread t_odd(tf_odd, N, std::ref(mtx),std::ref(output), std::ref(cv), std::ref(odd_turn));
    std::thread t_even(tf_even, N, std::ref(mtx),std::ref(output), std::ref(cv), std::ref(odd_turn));

    t_odd.join();
    t_even.join();

    print_vector(output);
  }
  return 0;
}