#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <algorithm>

struct Student
{
  std::string name;
  int score;
};


int main() {
  std::vector<Student> students {
    {"Alice", 80},
    {"Bob", 72},
    {"Charlie", 95},
    {"David", 61},
    {"Eve", 99},
    {"Frank", 88},
    {"Grace", 67},
    {"Henry",91},
    {"Ian",84},
    {"Jack",78}
  };

  std::vector<std::vector<int>> nums{
    {1,2},
    {3,4,5},
    {},
    {6},
    {7,8},
    {9,10,11,12},
    {13},
    {14,15}
  };

  std::vector<std::vector<std::string>> words{
    {"The", "quick"},
    {"brown", "fox"},
    {"jumps"},
    {"over", "the", "lazy", "dog"}
  };

  auto vw_topStudents = std::views::filter(students,[](const Student& x){return x.score >= 80;});
  
  auto vw_topSNames = std::views::transform(vw_topStudents, [](const Student& x){return x.name;} );

  for(const std::string& s : vw_topSNames) {
   std::cout << s << std::endl; 
  }

  auto names = 
    students 
    | std::views::filter([](const Student& x){return x.score >= 80;})
    | std::views::filter([](const Student& x){return x.name.size() > 3;})
    | std::views::transform([](const Student& x){return x.name;});   

  std::ranges::sort(students, {}, &Student::score);

  auto last3Over80 = 
   students 
   | std::views::filter([](const Student& x){
     return x.score >= 80;
   })
   | std::views::reverse
   | std::views::take(3)
   | std::views::transform(&Student::name)
   | std::views::reverse;

  auto even = [](int i){return 0==i%2;};

  auto flat = 
    nums  
    | std::views::join
    | std::views::filter(even);

  for(const int& x : flat) {
    std::cout << x << '\n'; 
  }

  
  return 0;
}