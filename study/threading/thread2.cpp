#include <thread>
#include <string>
#include <iostream>
#include <syncstream>
#include <vector>

/*
Exercise 2 — Multiple Threads Printing Their IDs
Goal: Understand how threads run independently and how to retrieve their IDs.
*/

void t_printInfo(std::string payload) {
  
  std::osyncstream(std::cout) << "Thread id: " << std::this_thread::get_id() << std::endl;
  std::osyncstream(std::cout) << "Payload: " << payload << std::endl;
  return;
}

int main(int argc, char* argv[]) {
  
  if(argc > 1) {
    int n = std::atoi(argv[1]);   
    
    std::vector<std::thread> threads;

    for(int i=1; i<=n; ++i) {
      threads.push_back(std::thread(t_printInfo,"Hello world, I am thread # " + std::to_string(i) + "!\n"));
    }
    
    for (auto& t : threads)
      t.join();
    //while(!threads.empty()) {
    //  for(unsigned i=threads.size()-1; i>=0; --i) {
    //    if(threads[i].joinable()) {
    //      threads[i].join();
    //      threads.erase(threads.begin()+i);
    //      break;
    //    }
    //  }
    //}
  }

  return 0;
}