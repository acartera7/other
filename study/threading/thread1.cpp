  #include <thread>
  #include <string>
  #include <iostream>

  using namespace std;

  void t_printHello(){
    cout << "Hello from the thread!" << endl;
  }

  int main(void) {
    
    thread t(t_printHello);

    t.join();

    return 0;
  }