#include <thread>
#include <string>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <syncstream>

/*
=== Exercise 18 - Readers/Writers Problem ===
*/

using ms = std::chrono::milliseconds;

std::random_device rd; 
std::mt19937 gen(rd());

template<typename... Args>
void ts_print(Args&&... args) {
    std::osyncstream out(std::cout);
    (out << ... << args) << '\n';
}

// std::uniform_int_distribution distrib(300,400);
// int rnd = distrib(gen);
// std::this_thread::sleep_for(ms(rnd));

struct PhaseRWScheduler {

  std::mutex scheduler_mtx;
  std::condition_variable reader_cv;
  std::condition_variable writer_cv;

  const size_t total_readers, total_writers; 
  const size_t readers_batchsize, writers_batchsize; // turnstile, how many total read/writers allowed to pass before switch
  const size_t max_readers;   // maximum amount of concurrent readers

  size_t readers_left, writers_left;    // turnstile, how many read/writers left to pass
  size_t active_readers, active_writers;

  enum class PHASE {
    READ_PHASE,
    WRITE_PHASE
  };

  PHASE curr_phase = PHASE::READ_PHASE;

  PhaseRWScheduler(const size_t r_total, 
                   const size_t w_total, 
                   const size_t r_batch, 
                   const size_t w_batch, 
                   const size_t r_max) : 
      scheduler_mtx(), 
      reader_cv(), 
      writer_cv(), 
      total_readers(r_total), 
      total_writers(w_total), 
      readers_batchsize(r_batch), 
      writers_batchsize(w_batch), 
      max_readers(r_max),
      active_readers(0),
      active_writers(0) {
    
    // initialize for readers to start first
    readers_left = readers_batchsize;
    writers_left = readers_batchsize;
  }

  void startRead() {
    std::unique_lock<std::mutex> ulock(scheduler_mtx);

    reader_cv.wait(ulock, [&](){
      return curr_phase == PHASE::READ_PHASE
             && active_readers <= max_readers
             && readers_left > 0;
    });

    active_readers++;
    readers_left--;

    if(readers_left == 0) {
      curr_phase == PHASE::WRITE_PHASE;
      readers_left = readers_batchsize;
    }
  }

  void endRead() {
    std::unique_lock<std::mutex> ulock(scheduler_mtx);

    active_readers--;

  }

  void acquireWrite(){

  }

};

void tf_read(std::string (&data)[], PhaseRWScheduler& scheduler) {
  scheduler.startRead();


}

void tf_write() {

}

int main(void) {

  // overview:
  // READER PHASE
  // ------------
  // allow readers in
  // queue writers
  // count readers

  // when max count occurs:
  //     stop accepting readers
  //     wait for active readers to finish

  // WRITER PHASE
  // ------------
  // allow writers in one at a time
  // count writers
  // queue readers

  // when  occurs:
  //     stop accepting writers
  //     wait for current writer to finish

  const size_t TOTAL_READERS = 5, TOTAL_WRITERS = 3;
  const size_t READERS_BATCHSIZE = 5, WRITERS_BATCHSIZE = 10;
  const size_t MAX_CONCURRENT_READERS = 3;
  
  std::shared_mutex rw_mutex;

  PhaseRWScheduler rw_scheduler(TOTAL_READERS,TOTAL_WRITERS, READERS_BATCHSIZE,
    WRITERS_BATCHSIZE, MAX_CONCURRENT_READERS);

  std::vector<std::thread> reader_threads;
  std::vector<std::thread> writer_threads;

  std::string data[] = {"Lorem", "ipsum", "dolor", "sit", "amet", 
                        "consectetur", "adipiscing", "elit"};

  for(unsigned i=0; i<TOTAL_READERS; ++i)   {
    reader_threads.emplace_back(
      tf_read,
      std::ref(data),
      std::ref(rw_scheduler)
    );
  }

  for(unsigned i=0; i<TOTAL_WRITERS; ++i) {
    writer_threads.emplace_back(
      tf_write,
      std::ref(data),
      std::ref(rw_mutex), 
      std::ref(rw_scheduler)
    );
  }

  for(auto& t : writer_threads) {
    t.join();
  }

  for(auto& t : reader_threads) {
    t.join();
  }

  return 0;
}

