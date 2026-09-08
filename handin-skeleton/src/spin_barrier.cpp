#include <spin_barrier.h>
using namespace std;


/************************
 * Your code here...    *
 * or wherever you like *
 ************************/

 /*void spin_barrier::setCounter(int count) {
   counter = count;
 }*/

 void spin_barrier::loop() {
    //int finishedThreads = counter.load();
    int local_counter = counter.fetch_add(1);
    int local_go = go.load();
    if(local_counter + 1 == numThreads) {
      reset();
      go.store(1-go.load());
    }
    else {
      while(local_go == go.load()) {
          this_thread::yield();
      }
    }
 }

 void spin_barrier::reset() {
  counter.store(0);
 }