#include <thread>
#include <string>
#include <iostream>
#include <syncstream>
#include <vector>

/*
Exercise 3 — Passing Parameters to a Thread
*/

void t_printInfo(std::string str, int num) {
  std::osyncstream out(std::cout);
  out << "String: " << str << std::endl;
  out << "Int: " << num << std::endl;

  return;
}

int main(int argc, char* argv[]) {
  
  if(argc > 2) {
    std::string s = argv[1];
    int n = std::atoi(argv[2]);   
    

    std::thread t(t_printInfo,s,n);

    t.join();
      
  }

  return 0;
}