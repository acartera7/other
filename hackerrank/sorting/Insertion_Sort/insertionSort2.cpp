#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

template <typename T>
void printArray(vector<T> v) {
    for (T x : v) {
        cout << x << " ";
    }
    cout << endl;
}

void insertionSort2(int n, vector<int> arr) {
    if(arr.empty()) return;
    
    int val;
    for(int i=1; i<n; ++i) { // skip the first element
        val = arr[i];
        for(int j=i, k=j-1; j>=0 && k>=-1; --j, --k) {
            if(j == 0) {
                arr[j] = val;
                break;
            }
            if(arr[k] > val) {
                arr[j] = arr[k];
            } else {
                arr[j] = val;
                break;
            }
        }
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

    insertionSort2(n, arr);

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
