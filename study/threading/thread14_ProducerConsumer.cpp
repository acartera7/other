#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>
#include <iostream>
#include <queue>
#include <chrono>
#include <array>
#include <random>
#include <atomic>
#include <syncstream>

/*
=== Producer–Consumer ===
This problem is about two threads coordinating over a shared queue:
- Producer: generates items and pushes them into a queue
- Consumer: removes items and processes them

The challenge is that:
- The producer must wait if the queue is full
- The consumer must wait if the queue is empty
- Both must avoid data races
- Both must avoid busy-waiting
- Both must avoid deadlocks
*/

std::mutex t_print_mtx;

template<typename... Args>
void ts_print(Args&&... args) {
    std::lock_guard<std::mutex> lock(t_print_mtx);
    (std::cout << ... << args) << std::endl;
}

template <typename T, size_t MAX>
class TQueue {
private:

  mutable std::mutex mtx;
  std::condition_variable cv_notempty;
  std::condition_variable cv_notfull;
  std::queue<T> q;
  size_t max_capacity = MAX;
  bool closed = false;

public:

  TQueue() : q(), max_capacity(MAX) {}

  TQueue(const TQueue& other) : q(other.q), max_capacity(other.max_capacity) {}

  TQueue& operator=(const TQueue& other) {
    if (this != &other) {
      q = other.q;
      max_capacity = other.max_capacity;
    }
    return *this;
  }

  void push(const T& in) {
    {
      std::unique_lock<std::mutex> lock(mtx);

      if (q.size() >= MAX) {
        ts_print("Queue is full. Producer is waiting...");
        cv_notfull.wait(lock, [this] { return this->q.size() < this->max_capacity; });
      }
      q.push(in);
    }
    cv_notempty.notify_one();
  }

  T pop() {
    T value;
    {
      std::unique_lock<std::mutex> lock(mtx);
      if(q.empty()) {
        ts_print("Queue is empty. Consumer is waiting...");
        cv_notempty.wait(lock, [this] { return !this->q.empty() || closed; });
        if (q.empty() && closed) // in case consumer is waiting for finished producer
          return T{};
      }

      value = q.front();
      q.pop();
    }
    cv_notfull.notify_one();
    return value;
  }

  void close_queue() {
    std::lock_guard<std::mutex> lock(mtx);
    closed = true;
    cv_notempty.notify_all();
  }

  const T& back()   const { std::lock_guard<std::mutex> lock(mtx); return q.back(); }
  const T& front()  const { std::lock_guard<std::mutex> lock(mtx); return q.front(); }
  size_t size()     const { std::lock_guard<std::mutex> lock(mtx); return q.size(); }
  bool empty()      const { std::lock_guard<std::mutex> lock(mtx); return q.empty(); }
  bool full()       const { std::lock_guard<std::mutex> lock(mtx); return q.size() >= max_capacity; }
  size_t capacity() const { return max_capacity; }

};


template <typename T, size_t MAX, size_t N>
void tf_produce(std::array<T,N> arr, TQueue<T,MAX>& que, std::atomic<bool>& finished)  {

  for(auto item : arr) {
    ts_print("Pushing: ", item);
    que.push(item);
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 301 + 200));
  }
  ts_print("Producer thread finished producing.");
  que.close_queue();
  finished.store(true);
}

template <typename T, size_t MAX>
void tf_consume(TQueue<T,MAX>& que, std::atomic<bool>& finished) {

  while(!finished.load() || !que.empty()) {
    T value = que.pop();
    ts_print("Consuming: ", value);
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 201 + 100));
  }
  ts_print("Consumer thread finished consuming.");
}

int main(void) {
  
  std::array<std::string,50> a_s = {
    "apple", "shadow", "river", "galaxy", "rocket",
    "orange", "planet", "dragon", "silver", "window",
    "forest", "thunder", "castle", "banana", "ocean",
    "crystal", "pencil", "mountain", "tiger", "camera",
    "diamond", "sunrise", "falcon", "library", "storm",
    "keyboard", "whisper", "desert", "volcano", "saturn",
    "notebook", "phantom", "engine", "emerald", "pirate",
    "comet", "breeze", "lantern", "mirror", "python",
    "guitar", "bridge", "rocketship", "jungle", "wizard",
    "nebula", "anchor", "machine", "spectrum", "voyager"
  };
  
  std::array<int,50> a_i = {
    12, 45, 78, 23, 89,
    34, 67, 91, 56, 10,
    43, 76, 29, 84, 15,
    62, 38, 95, 71, 27,
    50, 99, 18, 64, 31,
    87, 41, 73, 22, 58,
    96, 14, 35, 80, 47,
    69, 25, 92, 53, 11,
    74, 39, 88, 16, 60,
    28, 97, 44, 72, 19
  };

    std::array<int,20> a_i2 = {
    12, 45, 78, 23, 89,
    34, 67, 91, 56, 10,
    43, 76, 29, 84, 15,
    62, 38, 95, 71, 27
  };
  
  std::array<float,100> a_f = {
    -0.98f, -0.75f, -0.62f, -0.44f, -0.31f,
    -0.15f, -0.05f,  0.02f,  0.14f,  0.27f,
    0.39f,  0.51f,  0.63f,  0.74f,  0.85f,
    0.91f,  0.99f,  0.88f,  0.76f,  0.64f,
    0.52f,  0.41f,  0.29f,  0.18f,  0.07f,
    -0.04f, -0.16f, -0.28f, -0.40f, -0.53f,
    -0.66f, -0.79f, -0.92f, -1.00f, -0.87f,
    -0.73f, -0.60f, -0.47f, -0.33f, -0.20f,
    -0.08f,  0.05f,  0.17f,  0.30f,  0.42f,
    0.55f,  0.68f,  0.80f,  0.93f,  1.00f,
    
    0.95f,  0.82f,  0.70f,  0.58f,  0.45f,
    0.32f,  0.19f,  0.06f, -0.07f, -0.21f,
    -0.35f, -0.48f, -0.61f, -0.74f, -0.86f,
    -0.97f, -0.89f, -0.77f, -0.65f, -0.54f,
    -0.43f, -0.30f, -0.18f, -0.06f,  0.08f,
    0.20f,  0.33f,  0.46f,  0.59f,  0.71f,
     0.83f,  0.94f,  1.00f,  0.90f,  0.78f,
     0.66f,  0.53f,  0.40f,  0.26f,  0.13f,
     0.00f, -0.12f, -0.25f, -0.38f, -0.50f,
     -0.63f, -0.76f, -0.88f, -0.96f, -1.00f
    };
    
    TQueue<int,5> queue;
    std::atomic<bool> finished = false;

    std::thread t_producer(tf_produce<int, 5, 20>, a_i2, std::ref(queue), std::ref(finished));
    std::thread t_consumer(tf_consume<int, 5>, std::ref(queue), std::ref(finished));

    t_producer.join();
    t_consumer.join();
    
    return 0;
  }