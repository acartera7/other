#include <iostream>
#include <string>
#include <cassert>

using namespace std;
int monthdays[12]   = {31,28,31,30,31,30,31,31,30,31,30,31};
string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr << "too little arguments" << endl;
    return 0; 
  }

  int day = atoi(argv[1]);

  int year = atoi(argv[2]);

  if(year == 1918 && day > 31 ) {
    monthdays[1]-=13; // Feb 1-13 do not exist

  } else if (year <= 1917) {

    if (year % 4 == 0) {
      // julian leap year
      monthdays[1]++; // Feb 29 
    }
  } else {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      // gregorian leap year
      monthdays[1]++; // Feb 29
    }
  }

  

  int month;
  for(int i=0; i<12; ++i ) {
    int days = monthdays[i];
    if (day - days > 0){
      day -= days;

    } else {
      month = i;
      break;
    }
  }

  assert(day != 0);

  string suff = "th";

  if(year == 1918 && day > 31 ) {
    day+=13;
  }

  switch (day % 10) {
    case 1:
      if(day != 11)
        suff = "st";
      break;
    case 2:
      if(day != 12)
        suff = "nd";
      break;
    case 3:
      if(day != 13)
        suff = "rd";
      break;
    default:
      break;
  }

  cout << months[month] + " " << to_string(day) + suff << endl;
  return 0;
}
