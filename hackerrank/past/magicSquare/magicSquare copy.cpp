#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

int formingMagicSquare(vector<vector<int>> s) {
        vector<vector<vector<int>>> magicSquares = {
        {{8,1,6},{3,5,7},{4,9,2}},
        {{6,1,8},{7,5,3},{2,9,4}},
        {{4,9,2},{3,5,7},{8,1,6}},
        {{2,9,4},{7,5,3},{6,1,8}},
        {{8,3,4},{1,5,9},{6,7,2}},
        {{4,3,8},{9,5,1},{2,7,6}},
        {{6,7,2},{1,5,9},{8,3,4}},
        {{2,7,6},{9,5,1},{4,3,8}}
    };

    int minCost = INT_MAX;

    for (const auto& magic : magicSquares) {
        int cost = 0;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cost += abs(s[i][j] - magic[i][j]);
            }
        }

        minCost = min(minCost, cost);
    }

    return minCost;
}   

int main(void) {
  string line;
  
  vector<string> input;
  vector<vector<int>> s;

  while(getline(cin, line)) {
    input.push_back(line);
  }
  stringstream ss;
  int x;
  for(string nline : input) {
    ss.clear();
    ss.str(nline);
    vector<int> v;
    for(unsigned i=0; i<input.size();++i) {
        ss >> x;
        v.push_back(x);
    }
    s.push_back(v);
  }
  // ss.clear();
  formingMagicSquare(s);

  return 0;
}