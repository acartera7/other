#include <thread>
#include <string>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <chrono>
#include <syncstream>
#include <processthreadsapi.h>

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

struct PhaseRWScheduler {

  std::mutex scheduler_mtx;
  std::condition_variable reader_cv;
  std::condition_variable writer_cv;

  size_t total_readers, total_writers; 
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
    writers_left = 0;
  }

  void startRead(int id) {
    std::unique_lock<std::mutex> ulock(scheduler_mtx);

    reader_cv.wait(ulock, [&](){
      bool result = curr_phase == PHASE::READ_PHASE
             && active_readers < max_readers
             && readers_left > 0;
      if(!result)
        ts_print("Reader ",id," (", GetCurrentThreadId(),") waits. ", 
          "Current phase: ", curr_phase==PHASE::READ_PHASE ? "READ | " : "WRITE | ",
          "Active readers: ", active_readers, " (Max: ", max_readers, ") | ",
          "Readers left: ", readers_left);
      return result;
    });
    
    active_readers++;
    readers_left--;
    ts_print("Reader ",id," (", GetCurrentThreadId(),") acquiring read. Active readers: ", active_readers, " (Max: ", max_readers, ") ", "Readers left: ", readers_left);

  }

  void startWrite(int id) {
    std::unique_lock<std::mutex> ulock(scheduler_mtx);

    writer_cv.wait(ulock, [&](){
      bool result = curr_phase == PHASE::WRITE_PHASE
            && active_writers == 0
            && writers_left > 0;
      if(!result)
        ts_print("Writer ",id," (", GetCurrentThreadId(),") waits. ", 
          "Current phase: ", curr_phase==PHASE::READ_PHASE ? "READ | " : "WRITE | ",
          "Writers left: ", writers_left);
      return result;
    });

    active_writers++;
    writers_left--;
    ts_print("Writer ",id," (", GetCurrentThreadId(),") acquiring write. Active writers: ", active_writers, " Writers left: ", writers_left);

  }

  void endRead() {
    bool lastreader = false;
    {
      std::unique_lock<std::mutex> ulock(scheduler_mtx); 
      active_readers--;
      //last reader, start writers
      if(active_readers == 0 && readers_left == 0) {
        if(total_writers) {
          ts_print("\n>>>>>>>>>> Readers turnstile exhausted. Switching to Writers... <<<<<<<<<<\n");
          curr_phase = PHASE::WRITE_PHASE;
          writers_left = writers_batchsize;
          lastreader = true;
        } else {
          readers_left = readers_batchsize;
        }
      }
    }
    if(lastreader) {
      writer_cv.notify_all();
      return;
    }
    if(curr_phase == PHASE::READ_PHASE)
      reader_cv.notify_one();
  }

  void endWrite() {
    bool lastwriter = false;
    {
      std::unique_lock<std::mutex> ulock(scheduler_mtx); 
      active_writers--;
      //last writer, start readers
      if(active_writers == 0 && writers_left == 0) {
        if(total_readers) {
          ts_print("\n>>>>>>>>>> Writers turnstile exhausted. Switching to Readers... <<<<<<<<<<\n");
          curr_phase = PHASE::READ_PHASE;
          readers_left = readers_batchsize;
          lastwriter = true;
        } else {
          writers_left = writers_batchsize;
        }
      }
    }
    if(lastwriter) {  
      reader_cv.notify_all();
      return;
    }
    if(curr_phase==PHASE::WRITE_PHASE) {
      writer_cv.notify_one();
    }
  }

  void exitRead() {
    bool lastreader = false;
    {
      std::unique_lock<std::mutex> ulock(scheduler_mtx); 
      total_readers--;
      
      if(total_readers==0) {
        if(total_writers==0) {
          ts_print("\n>>>>>>>>>> All readers and writers exited. <<<<<<<<<<\n");
          return;
        }
        ts_print("\n>>>>>>>>>> All readers exited. Switching to Readers... <<<<<<<<<<\n");
        curr_phase = PHASE::WRITE_PHASE;
        writers_left = writers_batchsize;
        lastreader=true;
      }
    }
    if(lastreader) {  
      writer_cv.notify_all();
    }
  }
  
  void exitWrite() {
    bool lastwriter = false;
    {
      std::unique_lock<std::mutex> ulock(scheduler_mtx); 
      total_writers--;
      
      if(total_writers==0) {
        if(total_readers==0) {
          ts_print("\n>>>>>>>>>> All readers and writers exited. <<<<<<<<<<\n");
          return;
        }
        ts_print("\n>>>>>>>>>> All writers exited. Switching to Readers... <<<<<<<<<<\n");
        curr_phase = PHASE::READ_PHASE;
        readers_left = readers_batchsize;
        lastwriter = true;
      }
    }
    if(lastwriter) {  
      reader_cv.notify_all();
    }
  }
};


void tf_read(int id, std::vector<std::string>& data, PhaseRWScheduler& scheduler) {
  for (size_t i=0; i<5; ++i) {

    scheduler.startRead(id);

    
    ts_print("Reader ",id," (", GetCurrentThreadId(),") starts to read");
    for(size_t j=0; j<data.size(); ++j) {
      //std::this_thread::sleep_for(ms(50));
      ts_print("Reader ",id," (", GetCurrentThreadId(),") reads: ", data[j], " (",j+1,"/",data.size() ,")");
    }
    scheduler.endRead();

    //std::uniform_int_distribution distrib(100,200);
    //std::this_thread::sleep_for(ms(distrib(gen)));
  
  }
  ts_print("\n>>>>>>>>>> Reader ",id," (", GetCurrentThreadId(),") finished. Exiting... <<<<<<<<<<\n");
  scheduler.exitRead();
  
}

void tf_write(int id, std::vector<std::string>& data, PhaseRWScheduler& scheduler) {
  for(int i=0; i<15; ++i) {
    
    scheduler.startWrite(id);

    ts_print("Writer ",id," (", GetCurrentThreadId(),") starts to write");

    //random word, at a random place gets a random letter...
    for(int j=1; j<=3; ++j) {

      std::uniform_int_distribution word_dist(0,int(data.size()-1)); 
      std::string& rnd_word = data[word_dist(gen)];
      std::string org_word = rnd_word;

      std::uniform_int_distribution pos_dist(0, int(rnd_word.size()-1));
      size_t rnd_pos = pos_dist(gen);
      
      std::uniform_int_distribution letter_dist('a', 'z');
      rnd_word[rnd_pos] = letter_dist(gen);

      ts_print("Writer ",id," (", GetCurrentThreadId(),") replaced \"",org_word,"\" with \"",rnd_word,"\" (",j,"/",3,")");
      //std::this_thread::sleep_for(ms(50));
    }
    scheduler.endWrite();

    //std::uniform_int_distribution distrib(100,200);
    //std::this_thread::sleep_for(ms(distrib(gen)));

  }
  ts_print("\n>>>>>>>>>> Writer ",id," (", GetCurrentThreadId(),") finished. Exiting... <<<<<<<<<<\n");
  scheduler.exitWrite();
}

int main(void) {

  const size_t TOTAL_READERS = 5, TOTAL_WRITERS = 3;
  const size_t READERS_BATCHSIZE = 5, WRITERS_BATCHSIZE = 10;
  const size_t MAX_CONCURRENT_READERS = 3;
  
  PhaseRWScheduler rw_scheduler(TOTAL_READERS,TOTAL_WRITERS, READERS_BATCHSIZE,
    WRITERS_BATCHSIZE, MAX_CONCURRENT_READERS);

  std::vector<std::thread> reader_threads;
  std::vector<std::thread> writer_threads;

  std::vector<std::string> data = {"Lorem", "ipsum", "dolor", "sit", "amet", 
                                    "consectetur", "adipiscing", "elit"};

  for(unsigned i=0; i<TOTAL_READERS; ++i)   {
    reader_threads.emplace_back(
      tf_read,
      i,
      std::ref(data),
      std::ref(rw_scheduler)
    );
  }

  for(unsigned i=0; i<TOTAL_WRITERS; ++i) {
    writer_threads.emplace_back(
      tf_write,
      i,
      std::ref(data),
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

