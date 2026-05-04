#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

bool isAlphabeticPalindrome(string code) {
    
    string result;
    
    copy_if(code.begin(), code.end(), back_inserter(result), [](char x){return isalpha(static_cast<unsigned char>(x));});
    
    transform(result.begin(), result.end(), result.begin(), ::tolower);

    string reversed(result.rbegin(), result.rend());

    if(result == reversed) {
        return true;
    }
    
    return false;
}

int main(int argc, char* argv[]) {
  if(argc < 2) {
    cerr << "Usage: " << argv[0] << " <string>" << endl;
    return 1;
  }
  string input = argv[1];
  if(isAlphabeticPalindrome(input)) {
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
  }
  return 0;
}