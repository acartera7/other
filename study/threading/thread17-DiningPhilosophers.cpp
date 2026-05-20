#include <thread>
#include <string>
#include <iostream>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>
#include <syncstream>

using ms = std::chrono::milliseconds;

const size_t NUM_PHILOSOPHERS = 5;

template<typename... Args>
void ts_print(Args&&... args) {
    std::osyncstream out(std::cout);
    (out << ... << args) << '\n';
}

class Philosopher {
private:
  int id;
  int bites_left;
  std::mutex& right_fork;
  std::mutex& left_fork;
  std::mt19937 gen;

public:
  Philosopher(int _id, std::mutex& r_fork, std::mutex& l_fork, unsigned appetite) : 
      id(_id), bites_left(appetite),
      right_fork(r_fork), left_fork(l_fork),
      gen(std::random_device{}()) {
  }
  // -----------------------------
  // for each philosopher i
  // right fork = i
  // left fork = (i+1) % 5
  // -----------------------------

  void put_down_left_fork() { 
    ts_print("Philosopher: ", id, " puts down fork: ", (id + 1) % NUM_PHILOSOPHERS);
    left_fork.unlock();
  }
  void put_down_right_fork() { 
    ts_print("Philosopher: ", id, " puts down fork: ", id);
    right_fork.unlock();
  }

  void eat() {
    std::uniform_int_distribution distrib(200,300);
    int rnd = distrib(gen);
    --bites_left;
    ts_print("Philosopher: ", id, " eats for ", rnd, "ms - ", bites_left, " bites left");
    std::this_thread::sleep_for(ms(rnd));
  }

  void think() {
    std::uniform_int_distribution distrib(300,400);
    int rnd = distrib(gen);
    ts_print("Philosopher: ", id, " thinks for ", rnd, "ms");
    std::this_thread::sleep_for(ms(rnd));
  }

  unsigned get_bites_left() const { return bites_left; }

  void ordered_dine() { //deadlock prevention by ordering 

    // define lower/higher forks
    std::mutex *lower_fork, *higher_fork;
    std::string lower,higher;
    int lower_forkID, higher_forkID;

    if(id < (id+1)%NUM_PHILOSOPHERS) {
      lower_fork = &right_fork;
      higher_fork = &left_fork;
      lower = "right";
      higher = "left";
      lower_forkID = id;
      higher_forkID = (id+1)%NUM_PHILOSOPHERS;
    } else {
      lower_fork = &left_fork;
      higher_fork = &right_fork;
      lower = "left";
      higher = "right";
      lower_forkID = (id+1)%NUM_PHILOSOPHERS;
      higher_forkID = id;
    }

    while(true) {
      
      think();

      //pick up lower fork
      lower_fork->lock();
      ts_print("Philosopher: ", id, " picks up ", lower ," fork: ", lower_forkID);

      //pick up higher fork
      higher_fork->lock();
      ts_print("Philosopher: ", id, " picks up ", higher ," fork: ", higher_forkID);

      eat();

      put_down_left_fork();
      put_down_right_fork();
      
      if(bites_left <= 0) {
        ts_print("Philosopher: ", id, " is satisified with the meal, proceeds to praise Aphrodite...");
        break;
      }
    }
  }

  void courteous_dine() { //deadlock prevention by resource relinquishing
    while(true) {
      
      think();

      // Pick up right fork
      right_fork.lock();
      ts_print("Philosopher: ", id, " picks up right fork: ", id);
      
      // BLOCKING
      if(left_fork.try_lock()) {
        ts_print("Philosopher: ", id, " picks up left fork: ", (id + 1) % NUM_PHILOSOPHERS);
      } else {
        put_down_right_fork();
        ts_print("Philosopher: ", id, " waits on left fork: ", (id + 1) % NUM_PHILOSOPHERS);
        std::this_thread::sleep_for(ms(100));
        continue;
      }
    
      eat();

      put_down_left_fork();
      put_down_right_fork();
      
      if(bites_left <= 0) {
        ts_print("Philosopher: ", id, " is satisified with the meal, proceeds to praise Aphrodite...");
        break;
      }
    }
  }
};

void tf_dine(Philosopher&& philosopher) {
  philosopher.courteous_dine();
}

int main(void) {

  std::vector<std::thread> threads;
  std::vector<std::mutex> forks(5);

  for(int i=0; i<NUM_PHILOSOPHERS; ++i) {
    threads.emplace_back(
      tf_dine,
      std::move(Philosopher(i,std::ref(forks[i]), std::ref(forks[(i+1)%NUM_PHILOSOPHERS]), 5))
    );
  }

  for(auto& t : threads) {
    t.join();
  }

  return 0;
}

