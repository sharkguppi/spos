// This is a typical textbook question for operation system courses although it appears to be poorly defined in first glance, it was originally formulated in 1965 by Edsger Dijkstra as a student exam exercise.
// You will not see this question in an interview but understanding it is the fundamental of cracking other multi-threading questions.

// Solution 0: Let philosophers eat one by one
// LeetCode accepts this solution but it is generally WRONG. Why?

// Using raw mutex is very unprofessional, we don’t know how pickLeftFork() pickRightFork() eat() putLeftFork() putRightFork() are implemented, if any of them throws exception, our mutex will never be unlocked! Always wrap mutex with lock_guard or unique_lock

// This approach destroys advantage of multithreading, only one thread can be executed at a time.

// class DiningPhilosophers {
// private:
//     mutex mtx; // 5 philosophers share 1 single mutex
// public:
//     DiningPhilosophers() { }
//     void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
//         mtx.lock();
//         pickLeftFork(); pickRightFork(); eat(); putLeftFork(); putRightFork();
//         mtx.unlock();
//     }
// };
// Solution 1: Lock the table then pick up both forks together
// Very similar to Solution 0, but we release locks immediately after picking up both forks, then eat() method can take as long as it requires without blocking other threads.
// However isn't a nice approach either, it has great performance impact as table size grows, imagin if we have 50 threads try to eat(), every single one need to lock()/unlock() the table for picking up both forks while other 49 threads will have to wait().

// class DiningPhilosophers {
// private:
//     mutex mtxFork[5];
//     mutex mtxTable;
    
// public:
//     DiningPhilosophers() { }
//     void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
//         int left = philosopher;
//         int right = (philosopher + 1) % 5;
		
//         unique_lock<mutex> lckTable(mtxTable);
//         unique_lock<mutex> lckForkLeft(mtxFork[left]);
//         unique_lock<mutex> lckForkRight(mtxFork[right]);
//         lckTable.unlock(); // after locking both forks, we can safely unlock table
        
//         pickLeftFork(); pickRightFork(); eat(); putRightFork(); putLeftFork();
//         // locks will be automatically released due to unique_lock RAII design
//     }
// };
// Solution 2: Change forks picking order
// Let even philosophers pick up left forks before right forks.
// Let odd philosophers pick up right forks before left forks.
// Before diving into final answer, lets see an easy to understand solution with raw mutex

// class DiningPhilosophers {
// private:
//     mutex mtx[5];
    
// public:
//     DiningPhilosophers() { }
//     void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
//         int left = philosopher;
//         int right = (philosopher + 1) % 5;
        
//         if(philosopher % 2 == 0){
//             mtx[right].lock(); // lock right before left
//             mtx[left].lock();
//             pickLeftFork(); pickRightFork();
//         }
// 		else{
//             mtx[left].lock(); // lock left before right
//             mtx[right].lock();
//             pickLeftFork(); pickRightFork();
//         }
        
//         eat(); putRightFork(); putLeftFork();
//         mtx[left].unlock();
//         mtx[right].unlock();
//     }
// };
// Then lets wrap up raw mutex with unique_lock.
// Be aware that unique_lock needs to be put outside of if statement, otherwise its scoop will be limited in if statement only

// class DiningPhilosophers {
// private:
//     mutex mtx[5];
    
// public:
//     DiningPhilosophers() { }
//     void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
//         int left = philosopher;
//         int right = (philosopher + 1) % 5;
        
//         unique_lock<mutex> lck1(mtx[left], defer_lock); // defer_lock: init lck1 without locking mtx
//         unique_lock<mutex> lck2(mtx[right], defer_lock);
        
//         if(philosopher % 2 == 0){
//             lck1.lock(); // do NOT use std::lock(lck1, lck2)
//             lck2.lock();
//             pickLeftFork(); pickRightFork();
//         }
// 		else{
//             lck2.lock();
//             lck1.lock();
//             pickLeftFork(); pickRightFork();
//         }
//         eat(); putRightFork(); putLeftFork();
//         // lck1 & lck2 are auto released after this line
//     }
// };
// Solution 3: Limit 4 philosophers to eat at same time
// Use Semaphore to limit max of 4 philosophers can eat at same time, at least one philosopher will get both forks.

// class Semaphore {
// private:
// 	int count;
//     mutex mtx;
//     condition_variable cv;
	
// public:
//     Semaphore(int n = 0) : count(n) { }
//     void Set(int n) { count = n; }
//     void Signal() {
//         unique_lock<mutex> lck(mtx);
//         ++count;
//         cv.notify_one();
//     }
//     void Wait() {
//         unique_lock<mutex> lck(mtx);
//         cv.wait(lck, [&](){ return count > 0; }); // standard format of wait() lambda expression
//         --count;
//     }
// };

// class DiningPhilosophers {
// private:
//     mutex mtx[5];
//     Semaphore sem;
    
// public:
//     DiningPhilosophers() { sem.Set(4); }
//     void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
//         int left = philosopher;
//         int right = (philosopher + 1) % 5;
        
//         sem.Wait(); // if there are 4 philosophers eating, thread will wait here

//         unique_lock<mutex> lckLeft(mtx[left]);
//         unique_lock<mutex> lckRight(mtx[right]);
        
//         pickLeftFork(); pickRightFork(); eat(); putRightFork(); putLeftFork();
        
//         lckLeft.unlock();
//         lckRight.unlock();

//         sem.Signal(); // finish eating, release 1 spot for other philosophers
//     }
// };
// why bother writing our own Semaphore class? Why don't we use #include "semaphore.h" like other solutions?
// Because semaphore.h is only available for Linux, while CreateSemaphore() is only available for windows, in real life writing following code breaks portability and readability.

// #ifdef UNIX
// 	// code for Linux
// #else
// 	// code for Windows
// #endif
// Is there a way to solve above problem without writing our own Semaphore class?
// Yes, we can use c++ 20 <semaphore> header, sadly LeetCode seems don't have c++ 20 support so following code won't compile :(

// #include <thread>
// #include <chrono>
// #include <semaphore> // ERROR: compiler unable to find this header

// class DiningPhilosophers {
// private:
//     mutex mtx[5];
//     counting_semaphore<4> sem(4);
    
// public:
//     DiningPhilosophers() { }
//     void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
//         int left = philosopher;
//         int right = (philosopher + 1) % 5;
        
//         sem.acquire(); // if there are 4 philosophers eating, thread will wait here

//         unique_lock<mutex> lckLeft(mtx[left]);
//         unique_lock<mutex> lckRight(mtx[right]);
        
//         pickLeftFork(); pickRightFork(); eat(); putRightFork(); putLeftFork();
        
//         lckLeft.unlock();
//         lckRight.unlock();

//         sem.release(); // finish eating, release 1 spot for other philosophers
//     }
// };
// Last Solution for fun: ON LINER

// class DiningPhilosophers {
// private:
//     mutex mtx[5];
// public:
//     void wantsToEat(int philosopher, function<void()> pickLeftFork, function<void()> pickRightFork, function<void()> eat, function<void()> putLeftFork, function<void()> putRightFork) {
//         scoped_lock lck(mtx[philosopher], mtx[(philosopher + 1) % 5]);
//         pickLeftFork(); pickRightFork(); eat(); putLeftFork(); putRightFork();
//     }
// };
// Interview Follow Ups:

// For solution 2, can we use lock(lck1, lck2), instead of lck1.lock(); lck2.lock();? Why?
// For solution 3, can we call lckRight.unlock() before lckLeft.unlock()? Why?
// For solution 3, can we call cv.notify_all() instead of cv.notify_one()? Why?
// For last solution, one liner gets TLEed sometime, explain why this approach is slow?

// class DiningPhilosophers {
// public:
//     condition_variable cv;
//     mutex m;
//     vector<bool> fork;
//     DiningPhilosophers() {
//         for(int i = 0;i<5;i++) {
//             fork.push_back(true);
//         }
//     }

//     void wantsToEat(int i,
//                     function<void()> pickLeftFork,
//                     function<void()> pickRightFork,
//                     function<void()> eat,
//                     function<void()> putLeftFork,
//                     function<void()> putRightFork) {
        
//         unique_lock<mutex> lock(m);
// 		while(fork[i] == false || fork[(i+1)%5] == false) {
//             cv.wait(lock);
//         }

//         pickLeftFork();
//         pickRightFork();

//         fork[i]=false ;
//         fork[(i+1)%5]=false ;

//         eat();

//         fork[i]=true ;
//         fork[(i+1)%5]=true ;
        
//         putLeftFork();
//         putRightFork();
        
//         cv.notify_all();
//     }
// };
// https://leetcode.com/problems/the-dining-philosophers/submissions/1792042837/