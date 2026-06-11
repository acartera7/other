#include <iostream>
#include <vector>
#include <algorithm>

void print_array(const std::vector<int>& v) {
  for (const auto& i : v) {
    std::cout << i << " ";
  }
  std::cout << "\n";
}

int main(void) {

  std::vector<int> v = {4, 7 ,2 ,5 ,9 ,1 ,0};

  std::cout << "Initial vector: " << std::endl;
  print_array(v);
  
  std::cout << "Vector Heapified" << std::endl;
  std::make_heap(v.begin(), v.end());
  print_array(v);
  
  std::cout << "Top: " << v.front() << std::endl;
  
  std::cout << "Pushing 10" << std::endl;
  v.push_back(10);
  print_array(v);
  
  std::cout << "Pushing Heap" << std::endl;
  std::push_heap(v.begin(), v.end());

  std::cout << "New Top: " << v.front() << std::endl;


  std::vector<int> v2 = {14, 22 ,2 ,11 ,19 ,5 ,48, 10, 6, 12, 17, 35};
  std::cout << "New Vector: " << std::endl;
  print_array(v2);

  std::cout << "New Vector Heapified" << std::endl;
  std::make_heap(v2.begin(), v2.end());
  print_array(v2);
  
  std::cout << "Popping Top 5 Elements" << std::endl;
  
  for(int i=0; i<5; ++i) {
    
    std::cout << v2.front() << std::endl;
    std::pop_heap(v2.begin(), v2.end());
    v2.pop_back();
  }
  
  std::cout << "Resulting New Vector:" << std::endl;
  print_array(v2);
  
  return 0;
}