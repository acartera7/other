#include <thread>
#include <string>
#include <iostream>
#include <syncstream>
#include <vector>
#include <chrono>


void t_printInfo(std::string id, std::string str) {  
  
  std::osyncstream out(std::cout);
  out << "Thread " << id << ": " << str << std::endl;
  out << "Sleeping for 1000ms" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

int main(int argc, char* argv[]) {
  
  if(argc > 2) {
    std::string s_a = argv[1];
    std::string s_b = argv[2];   

    std::thread t_a(t_printInfo,"A",s_a);
    std::thread t_b(t_printInfo,"B",s_b);

    t_a.join();
    std::cout << "Main: Thread A has been joined" << std::endl;

    t_b.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  }

  return 0;
}