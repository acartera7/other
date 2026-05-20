#include <thread>
#include <string>
#include <iostream>

/*
Exercise 1 — Your First Thread
Goal: Build intuition for how a thread is created, started, and joined in C++.
*/

using namespace std;

void t_printHello(){
  cout << "Hello from the thread!" << endl;
}

int main(void) {
  
  thread t(t_printHello);

  t.join();

  return 0;
}