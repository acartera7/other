#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

int yeardays[12]   = {31,28,31,30,31,30,31,31,30,31,30,31};
int zel_months[12] = {13,14,3,4,5,6,7,8,9,10,11,12};
string days_of_week[7] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "No Input" << endl;
    return 0; 
  }
  string result = "Error";

  string date = argv[1];

  int month = stoi(date.substr(0,2));
  int day = stoi(date.substr(2,2));
  int year = stoi(date.substr(4,4));


  /*
  Zeller's Congruence:
  h = ( q + floor(13(m+1)/5) + K + floor(K/4) + floor(J/4) - 2J ) mod 7
  */
  int q = day;
  int m = zel_months[month-1];
  if (month == 1 || month == 2) {
    year -= 1;
  }
  int K = year % 100; 
  int J = int(year / 100);
  int h = (q + (13*(m+1))/5 + K + K/4 + J/4 - 2*J) % 7;

  cout << days_of_week[h] << endl;
  //cout << month << " " << day << " " << year << " " << endl;
  return 0;
}
