#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <chrono>
#include <vector>
#include <cstdlib>

using namespace std;


const int N_PHIL = 5;
mutex forks[N_PHIL];
counting_semaphore<N_PHIL - 1> room(N_PHIL - 1);

void philosopher(int id) {
    while (true) {
        cout << "[Philosopher " << id << "] Thinking...\n";
        this_thread::sleep_for(chrono::milliseconds(500 + rand() % 500));

        room.acquire();
        forks[id].lock();
        forks[(id + 1) % N_PHIL].lock();

        cout << "[Philosopher " << id << "] Eating...\n";
        this_thread::sleep_for(chrono::milliseconds(500 + rand() % 500));

        forks[id].unlock();
        forks[(id + 1) % N_PHIL].unlock();
        room.release();
    }
}

void runDiningPhilosophers() {
    cout << "\n=== Dining Philosophers Problem ===\n";
    vector<thread> philosophers;
    for (int i = 0; i < N_PHIL; i++)
        philosophers.emplace_back(philosopher, i);

    for (auto &t : philosophers)
        t.join();
}


int readCount = 0;
mutex readCountMutex;
counting_semaphore<1> resource(1);
counting_semaphore<1> serviceQueue(1);

void reader(int id) {
    while (true) {
        serviceQueue.acquire();
        readCountMutex.lock();
        if (++readCount == 1)
            resource.acquire();
        readCountMutex.unlock();
        serviceQueue.release();

        cout << "[Reader " << id << "] Reading data...\n";
        this_thread::sleep_for(chrono::milliseconds(400 + rand() % 400));

        readCountMutex.lock();
        if (--readCount == 0)
            resource.release();
        readCountMutex.unlock();

        this_thread::sleep_for(chrono::milliseconds(400 + rand() % 600));
    }
}

void writer(int id) {
    while (true) {
        serviceQueue.acquire();
        resource.acquire();
        serviceQueue.release();

        cout << "[Writer " << id << "] Writing data...\n";
        this_thread::sleep_for(chrono::milliseconds(600 + rand() % 600));

        resource.release();
        this_thread::sleep_for(chrono::milliseconds(600 + rand() % 800));
    }
}

void runReadersWriters() {
    cout << "\n=== Readers-Writers Problem ===\n";
    vector<thread> readers, writers;

    for (int i = 0; i < 5; i++)
        readers.emplace_back(reader, i);
    for (int i = 0; i < 2; i++)
        writers.emplace_back(writer, i);

    for (auto &t : readers)
        t.join();
    for (auto &t : writers)
        t.join();
}



int main() {
    srand(time(nullptr));
    int choice;

    cout << "==============================\n";
    cout << "   Synchronization Problems\n";
    cout << "==============================\n";

    while (true) {
        cout << "\nChoose an option:\n";
        cout << "1. Dining Philosophers Problem\n";
        cout << "2. Readers-Writers Problem\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                runDiningPhilosophers();
                break;
            case 2:
                runReadersWriters();
                break;
            case 3:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}
