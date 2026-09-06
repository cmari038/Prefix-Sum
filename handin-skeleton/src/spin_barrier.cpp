#include <spin_barrier.h>
using namespace std;


/************************
 * Your code here...    *
 * or wherever you like *
 ************************/

 void spin_barrier::setCounter(int count) {
   counter = count;
 }

 void spin_barrier::loop() {
    //int finishedThreads = counter.load();
    while(counter < numThreads) {
      this_thread::yield();
    }
    reset(); 
 }

 void spin_barrier::reset() {
  counter = 0;
 }