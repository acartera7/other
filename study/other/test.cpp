#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  

using namespace std;

static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
    return s;
}

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) {return !std::isspace(c);}).base(), s.end());
    return s;
}

/*
 * Complete the 'repeatedString' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. STRING s
 *  2. LONG_INTEGER n
 */

long repeatedString(string s, long long n) {
  unsigned a_count = count(s.begin(), s.end(), 'a');
  return ((n/s.size()) / a_count) + count(s.begin(), s.begin()+(n%s.size()), 'a');
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    string n_temp;
    getline(cin, n_temp);

    long long n = 1000000000000;

    long result = repeatedString(s, n);

    fout << result << "\n";

    fout.close();

    return 0;
}

