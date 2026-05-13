#include <thread>
#include <string>
#include <iostream>
#include <syncstream>
#include <vector>
#include <chrono>


void t_sum(size_t n,int* arr, int& sum) {  
  for(size_t i=0; i<n; ++i) {
    sum += arr[i];
  }
}

int main(int argc, char* argv[]) {
  
  int arr[10] = {5,7,8,3,5,1,8,5,9,0};
  int sum =0;
  std::thread t(t_sum,10, arr, std::ref(sum));

  t.join();
  std::cout << "Main: Thread Joined." << std::endl;
  std::cout << "Main: Sum = " << sum << std::endl;

  return 0;
}