#include <string>
#include <iostream>
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
 * Complete the 'findSmallestMissingPositive' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY orderNumbers as parameter.
 */

int findSmallestMissingPositive(vector<int> orderNumbers) {
    int smallest = 1;
      int val = -1;
    for(unsigned i=0; i<orderNumbers.size(); ++i) { // passing loop
      val = orderNumbers[i];
      
      while(static_cast<unsigned>(val) != i+1 && val > 0) { // swapping loop
        
        if(static_cast<unsigned>(val) < orderNumbers.size()) {
          if(orderNumbers[val-1] == val) 
            break;

          orderNumbers[i] = orderNumbers[val-1];
          orderNumbers[val-1] = val;
          val = orderNumbers[i];
        } else  
          break;
      }
    }
    
    for(unsigned i=0; i<orderNumbers.size(); ++i) { //scanning loop
      if(orderNumbers[i] != (int)i+1) {
        break;
      }
      ++smallest;
    }
    return smallest;
}

int main()
{
    string orderNumbers_count_temp;
    getline(cin, orderNumbers_count_temp);

    int orderNumbers_count = stoi(ltrim(rtrim(orderNumbers_count_temp)));

    vector<int> orderNumbers(orderNumbers_count);

    for (int i = 0; i < orderNumbers_count; i++) {
        string orderNumbers_item_temp;
        getline(cin, orderNumbers_item_temp);

        int orderNumbers_item = stoi(ltrim(rtrim(orderNumbers_item_temp)));

        orderNumbers[i] = orderNumbers_item;
    }

    int result = findSmallestMissingPositive(orderNumbers);

    cout << result << "\n";

    return 0;
}

