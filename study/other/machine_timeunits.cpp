#include <string>
#include <iostream>
#include <vector>
#include <algorithm>  
#include <numeric>  

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
 * Complete the 'calculateMinimumTimeUnits' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY tasks
 *  2. INTEGER m
 *  3. INTEGER k
 */


bool checkCooldown(vector<vector<int>> machine_history, int m_index, int task, int time, int cooldown) {
  for(int i=1; i<cooldown; ++i) {
    if(time-i >= 0) {
      if(machine_history[m_index][time-i] < 0) //on cooldown
        continue;

      if(machine_history[m_index][time-i] == task) {
        return false;
      }
    } else {
      return true;
    }
  }
  return true;
}

int calculateMinimumTimeUnits(vector<int> tasks, int m, int k) {
  
  int time = 0;
  if(tasks.empty()) return time;
  for(int i = 0; i < m; ++i) {}
  vector<vector<int>> machine_history;

  if(((tasks.size() / m) * k) < 4) {
    machine_history = vector<vector<int>>(m,vector<int>(4,-1));
  } else {
    //max time heuristic = (n_tasks / m) * k (if all tasks needed a cooldown)
    machine_history = vector<vector<int>>(m,vector<int>((tasks.size() / m) * k,-1)); 

  }
  vector<int> machine_order(m);
  iota(machine_order.begin(), machine_order.end(), 0);
  do {

    vector<int> machine_last = {};
    
    //the machine who ran the highest number task should go first to detect collisions down the line
    if (time > 0) {
      for (int i = 0; i < m; ++i) {
        machine_last.push_back(machine_history[i][time-1]);
      }
      sort(machine_order.begin(), machine_order.end(),[=](int i1, int i2) {return machine_last[i1] > machine_last[i2];});
      
    }
    
    for(int m_index : machine_order) {
      int task = -1;
      
      //scroll to the first task that doesn't have a collision, if none just pick the first
      if(!machine_last.empty()) {
        for(int i=0; i< tasks.size(); ++i) {
          if(machine_last[m_index] != tasks[i]) {
            task = tasks[i];
            tasks.erase(tasks.begin()+i);
            break;
          }
        }
      }
        
      if(task == -1) {
        //attempt to assign the first task

        if(!machine_last.empty()){
          //put on cooldown
          if(checkCooldown(machine_history,m_index, tasks[0], time, k)) {
            machine_history[m_index][time] = tasks[0];
          } else {
            continue;
          }
        }
        machine_history[m_index][time] = tasks[0];
        tasks.erase(tasks.begin());
        
      } else {
        machine_history[m_index][time] = task;
        
      }
      if(tasks.empty()) break;
      
    }
    ++time;
  } while(!tasks.empty());  
  
  return time;
}

int main()
{
  string tasks_count_temp;
  getline(cin, tasks_count_temp);

  int tasks_count = stoi(ltrim(rtrim(tasks_count_temp)));

  vector<int> tasks(tasks_count);

  for (int i = 0; i < tasks_count; i++) {
    string tasks_item_temp;
    getline(cin, tasks_item_temp);

    int tasks_item = stoi(ltrim(rtrim(tasks_item_temp)));

    tasks[i] = tasks_item;
  }

  string m_temp;
  getline(cin, m_temp);

  int m = stoi(ltrim(rtrim(m_temp)));

  string k_temp;
  getline(cin, k_temp);

  int k = stoi(ltrim(rtrim(k_temp)));

  int result = calculateMinimumTimeUnits(tasks, m, k);

  cout << result << "\n";

  return 0;
}

