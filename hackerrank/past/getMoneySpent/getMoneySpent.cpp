#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

template <typename T>
int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

int getMoneySpent(vector<int> keyboards, vector<int> drives, int b) {
    int money_spent = 0;
    sort(keyboards.begin(), keyboards.end());
    sort(drives.begin(), drives.end());
    
    for(unsigned i=0; i<keyboards.size(); ++i) {
        for(unsigned j=0; j<drives.size(); ++j) {
            int curr_money = keyboards[i]+drives[j];
            if(curr_money <= b) {
              money_spent = curr_money > money_spent ? curr_money : money_spent;
            } 
        }
    }
    return money_spent;
}


int main(void) {
  string line;
  
  getline(cin, line);
  stringstream ss(line);
  int b, n, m;
  ss >> b >> n >> m;

  getline(cin, line);
  ss.clear();
  ss.str(line);

  int x;
  vector<int> keyboards, drives;
  for(int i=0; i<n; ++i) {
    ss >> x;
    keyboards.push_back(x);
  }

  getline(cin, line);
  ss.clear();
  ss.str(line);

  for(int i=0; i<m; ++i) {
    ss >> x;
    drives.push_back(x);
  }

  int moneyspent = getMoneySpent(keyboards, drives, b);
  cout << moneyspent << endl;
  return 0;
}