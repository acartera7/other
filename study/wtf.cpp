#include <iostream>
#include <deque>
#include <string>
#include <algorithm>

using namespace std;

void print(const deque<int>& wines) {
  for(int i : wines) {
    cout << i << " ";
  }
  cout << endl;
}


int main(void) {

  int sum = 0;
  deque<int> wines = {2,3,5,1,4};

  for(int i = 1; i<=5; ++i) {
    print(wines);
    
    if(wines.front() <  wines.back()) {
      sum += wines.front()*i;
      cout << "wine removed: " << wines.front() << "*" << i << endl;
      wines.pop_front();
    } else {
      sum += wines.back()*i;
      cout << "wine removed: " << wines.back() << "*" << i << endl;

      wines.pop_back();
    }
    cout << "sum: " << sum << "\n\n";
  }
  

  return 0;
}