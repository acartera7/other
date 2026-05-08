#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>

using namespace std;

vector<string> split(const string &);

static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
    return s;
}

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) {return !std::isspace(c);}).base(), s.end());
    return s;
}

/*
 * Complete the 'insertionSort1' function below.
 *
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER_ARRAY arr
 */

template <typename T>
void printArray(vector<T> v) {
  for (T x : v) {
    cout << x << " ";
  }
  cout << endl;
}

int get_digit(int num, int i_digit) {
  return int(num / pow(10, i_digit)) % 10;
}

void radixSort(int n, vector<int> arr) {
  if(arr.empty()) return;
  
  //find max digits
  int biggest = *max_element(arr.begin(), arr.end());
  unsigned digits = unsigned(log10(biggest))+1;
  
  
  vector<int> out_arr(arr.size(), 0);

  for(unsigned i=0; i<digits; ++i) { // per digit pass
    int val=0;
    int sum=-1; // account for index in summing
    int count[10] = {0}; 

    for(size_t j=0; j<arr.size(); ++j) { // count digits
      val = arr[j];
      ++count[get_digit(val,i)]; 
    }
    for(size_t j=0; j<10; ++j) { // rollings summation
      count[j] += sum;
      sum = count[j];
    }

    for(int j=int(arr.size()-1); j>=0; --j) { // place into out_array
      out_arr[count[get_digit(arr[j],i)]--] = arr[j]; // <.< 
    }
    
    arr = out_arr;
    out_arr = vector<int>(arr.size(), 0);
    printArray(arr);
  }
    
}

int main()
{
    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    radixSort(n, arr);

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
