#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
  return s;
}

static inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) {return !std::isspace(c);}).base(), s.end());
  return s;
}
vector<string> split(const string &);

/*
 * Complete the 'countSwaps' function below.
 *
 * The function accepts INTEGER_ARRAY a as parameter.
 */

void countSwaps(vector<int> a) {
  unsigned numSwaps = 0;
  for(unsigned i=0; i<a.size(); ++i) {
    for(unsigned j=0; j<a.size()-1; ++j) {
      if(a[j] > a[j+1]) {
        swap(a[j], a[j+1]);
        ++numSwaps;
      }
    }
  }
  cout << "Array is sorted in " << numSwaps << " swaps." << endl;
  cout << "First Element: " << a[0] << endl;
  cout << "Last Element: " << a[a.size()-1] << endl;
}

int main()
{
  string n_temp;
  getline(cin, n_temp);

  int n = stoi(ltrim(rtrim(n_temp)));

  string a_temp_temp;
  getline(cin, a_temp_temp);

  vector<string> a_temp = split(rtrim(a_temp_temp));

  vector<int> a(n);

  for (int i = 0; i < n; i++) {
    int a_item = stoi(a_temp[i]);

    a[i] = a_item;
  }

  countSwaps(a);

  return 0;
}


vector<string> split(const string &str) {
  vector<string> tokens;

  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = str.find(" ", start)) != string::npos) {
    tokens.push_back(str.substr(start, end - start));

    start = end + 1;
  }

  tokens.push_back(str.substr(start));

  return tokens;
}
