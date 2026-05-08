#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

template<typename T>
int signum(T t) {
  return (T(0) < t) - (t < T(0));
}

vector<int> getSums(vector<vector<int>> s){
  vector<int> result;
  int sum=0;
  for(unsigned j=0; j<s.size();++j) {
    for(unsigned k=0; k<s.size();++k) {
      sum += s[j][k];
    }
    result.push_back(sum);
    sum = 0;
  }
  for(unsigned j=0; j<s.size();++j) {
    for(unsigned k=0; k<s.size();++k) {
      sum += s[k][j];
    }
    result.push_back(sum);
    sum = 0;
  }
  for(unsigned i=0; i<s.size();++i) {
    sum += s[i][i];
  }
  result.push_back(sum);
  sum = 0;

  for(unsigned i=0; i<s.size();++i) {
    sum += s[i][(s.size()-1)-i];
  }
  result.push_back(sum);

  return result;
}

//changes a specified cell of a magic square with a value, updates it and returns the sums vector with the result of the change
void MSAssign(vector<vector<int>>& s, vector<int>& s_sums, std::pair<int,int> pos, int value) {
  /*
  i,j in 0..n-1

  row = i;
  column = n+j

  diag: 2n if i=j
  anti-diag: 2n+1 if i+j=n-1
  */

  int n = s.size();
  int i = pos.first, j = pos.second;
  
  int row = i;
  int col = n+j;

  //find the difference between the change
  int val_diff = value - s[i][j];

  s[i][j] = value; //change value
 
  //apply difference to sums
  s_sums[row] += val_diff;
  s_sums[col] += val_diff;

  if(i==j) {
    s_sums[2*n] += val_diff;
  }
  if(i+j == n-1) {
    s_sums[2*n+1] += val_diff;
  }
}

int formingMagicSquare(vector<vector<int>> s) {
    int cost = 0;
    /*M = n * ((n^2 + 1) / 2)*/
    int const CONSTANT = s.size() * ((s.size()*s.size() + 1) / 2);
    unsigned const N = s.size();

    //get the sums of the rows, colums, and diagonals
    auto s_sums = getSums(s);
    
    vector<int> trg_sums(s_sums.size(),CONSTANT); //target sums, a vector of the magic number CONSTANT
    
    vector<int> diff_sums(s_sums.size()); // difference between the actual and target sums
    transform(trg_sums.begin(), trg_sums.end(),s_sums.begin(), diff_sums.begin(), minus<int>());

    //for each cell, if its corresponding sums have a minimum shared difference then we can change that cell by that
    int minimum;
    for(unsigned i=0; i<s.size(); ++i) {
      minimum = numeric_limits<int>::max();
      
      for(unsigned j=0; j<s.size(); ++j) {

        minimum = min(minimum,diff_sums[i]);    //row affected
        minimum = min(minimum,diff_sums[N+j]);  //column affected

        if(i==j) 
          minimum = min(minimum,diff_sums[2*N]);  //main diagnol affected
        
        if(i+j==N-1) 
          minimum = min(minimum,diff_sums[2*N+1]);  //anti-diagonal affected
        
        if (minimum != 0) {
          MSAssign(s,s_sums,{i,j},s[i][j]+minimum);
          diff_sums[i]-=minimum;
          diff_sums[N+j]-=minimum;
          if(i==j) 
            diff_sums[2*N]-=minimum;
          
          if(i+j==N-1) 
            diff_sums[2*N+1]-=minimum;

        }
      }
    }

    cout << "test" << endl;

    return cost;
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