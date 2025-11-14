#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <condition_variable>

// Use a single mutex for cout to prevent garbled output from multiple threads
std::mutex print_mutex;

// =================================================================================
// CUSTOM SEMAPHORE (C++11 compatible)
// =================================================================================

class CustomSemaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;

public:
    CustomSemaphore(int initial_count = 0) : count(initial_count) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until count > 0
        cv.wait(lock, [this](){ return count > 0; });
        --count;
    }

    void release() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one(); // Notify one waiting thread
    }
};


// =================================================================================
// UTILITY FUNCTIONS
// =================================================================================

void print(const std::string& message) {
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << message << std::endl;
}

// =================================================================================
// DINING PHILOSOPHERS PROBLEM
// =================================================================================

namespace DP {
    // --- Solution 1: Asymmetric (Odd/Even) Fork Pickup Order (Prevents Deadlock) ---
    void philosopher_asymmetric(int id, int num_philosophers, int meals_to_eat, std::vector<std::mutex>& forks) {
        for (int meal = 0; meal < meals_to_eat; ++meal) {
            print("Philosopher " + std::to_string(id) + " is thinking.");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            int left_fork = id;
            int right_fork = (id + 1) % num_philosophers;

            // Odd/Even rule to break circular wait
            if (id % 2 == 0) {
                forks[left_fork].lock();
                print("Philosopher " + std::to_string(id) + " picked up left fork (" + std::to_string(left_fork) + ").");
                forks[right_fork].lock();
                print("Philosopher " + std::to_string(id) + " picked up right fork (" + std::to_string(right_fork) + ").");
            } else {
                forks[right_fork].lock();
                print("Philosopher " + std::to_string(id) + " picked up right fork (" + std::to_string(right_fork) + ").");
                forks[left_fork].lock();
                print("Philosopher " + std::to_string(id) + " picked up left fork (" + std::to_string(left_fork) + ").");
            }

            print("Philosopher " + std::to_string(id) + " is eating meal " + std::to_string(meal + 1) + ".");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            forks[left_fork].unlock();
            forks[right_fork].unlock();
            print("Philosopher " + std::to_string(id) + " put down both forks.");
        }
    }

    // --- Solution 2: Semaphore (Limit Philosophers at the Table) ---
    void philosopher_semaphore(int id, int num_philosophers, int meals_to_eat, std::vector<std::mutex>& forks, CustomSemaphore& table) {
        for (int meal = 0; meal < meals_to_eat; ++meal) {
            print("Philosopher " + std::to_string(id) + " is thinking.");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            table.acquire(); // Wait for a seat at the table
            print("Philosopher " + std::to_string(id) + " sat at the table.");

            int left_fork = id;
            int right_fork = (id + 1) % num_philosophers;

            forks[left_fork].lock();
            print("Philosopher " + std::to_string(id) + " picked up left fork (" + std::to_string(left_fork) + ").");
            forks[right_fork].lock();
            print("Philosopher " + std::to_string(id) + " picked up right fork (" + std::to_string(right_fork) + ").");

            print("Philosopher " + std::to_string(id) + " is eating meal " + std::to_string(meal + 1) + ".");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            forks[left_fork].unlock();
            forks[right_fork].unlock();
            print("Philosopher " + std::to_string(id) + " put down both forks.");

            table.release(); // Leave the table
            print("Philosopher " + std::to_string(id) + " left the table.");
        }
    }
    
    // --- Solution 3: Deadlock-Free Multiple Lock (C++11 std::lock) ---
    void philosopher_scoped_lock(int id, int num_philosophers, int meals_to_eat, std::vector<std::mutex>& forks) {
        for (int meal = 0; meal < meals_to_eat; ++meal) {
            print("Philosopher " + std::to_string(id) + " is thinking.");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            int left_fork = id;
            int right_fork = (id + 1) % num_philosophers;

            // C++11 equivalent of std::scoped_lock
            {
                // Lock both mutexes simultaneously to prevent deadlock
                std::lock(forks[left_fork], forks[right_fork]);

                // Use lock_guard with adopt_lock to manage the already-locked mutexes
                std::lock_guard<std::mutex> lock_left(forks[left_fork], std::adopt_lock);
                std::lock_guard<std::mutex> lock_right(forks[right_fork], std::adopt_lock);

                print("Philosopher " + std::to_string(id) + " picked up both forks.");
                print("Philosopher " + std::to_string(id) + " is eating meal " + std::to_string(meal + 1) + ".");
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                print("Philosopher " + std::to_string(id) + " is about to put down forks.");
            } // lock_guards go out of scope and automatically release the locks
            print("Philosopher " + std::to_string(id) + " put down both forks.");
        }
    }


    void run() {
        int choice, num_philosophers, meals_to_eat;
        std::cout << "\n--- Dining Philosophers Problem ---\n";
        std::cout << "Enter number of philosophers: ";
        std::cin >> num_philosophers;
        std::cout << "Enter number of meals each philosopher should eat: ";
        std::cin >> meals_to_eat;

        std::cout << "\nSelect a solution:\n";
        std::cout << "1. Asymmetric Fork Pickup (Odd/Even Rule)\n";
        std::cout << "2. Semaphore (Limit Philosophers at Table)\n";
        std::cout << "3. Deadlock-Free Multiple Lock (std::lock)\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        std::vector<std::thread> philosophers;
        std::vector<std::mutex> forks(num_philosophers);

        switch (choice) {
            case 1: {
                print("\nStarting simulation with Asymmetric solution...");
                for (int i = 0; i < num_philosophers; ++i) {
                    philosophers.emplace_back(philosopher_asymmetric, i, num_philosophers, meals_to_eat, std::ref(forks));
                }
                break;
            }
            case 2: {
                print("\nStarting simulation with Semaphore solution...");
                // Allow N-1 philosophers at the table to prevent deadlock
                CustomSemaphore table(num_philosophers - 1);
                for (int i = 0; i < num_philosophers; ++i) {
                    philosophers.emplace_back(philosopher_semaphore, i, num_philosophers, meals_to_eat, std::ref(forks), std::ref(table));
                }
                break;
            }
            case 3: {
                print("\nStarting simulation with Scoped Lock solution...");
                 for (int i = 0; i < num_philosophers; ++i) {
                    philosophers.emplace_back(philosopher_scoped_lock, i, num_philosophers, meals_to_eat, std::ref(forks));
                }
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
                return;
        }

        for (auto& p : philosophers) {
            p.join();
        }
        print("\nAll philosophers have finished eating.");
    }
}

// =================================================================================
// READER-WRITER PROBLEM
// =================================================================================

namespace RW {
    std::mutex read_count_mutex;
    CustomSemaphore write_sem(1); // Controls write access
    int read_count = 0;

    void reader(int id) {
        print("Reader " + std::to_string(id) + " wants to read.");
        
        // Acquire lock to increment read_count
        read_count_mutex.lock();
        read_count++;
        if (read_count == 1) {
            write_sem.acquire(); // First reader blocks writers
        }
        read_count_mutex.unlock();

        // Reading section
        print("Reader " + std::to_string(id) + " is reading.");
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        print("Reader " + std::to_string(id) + " finished reading.");

        // Acquire lock to decrement read_count
        read_count_mutex.lock();
        read_count--;
        if (read_count == 0) {
            write_sem.release(); // Last reader unblocks writers
        }
        read_count_mutex.unlock();
    }

    void writer(int id) {
        print("Writer " + std::to_string(id) + " wants to write.");
        write_sem.acquire(); // Acquire exclusive lock for writing

        // Writing section
        print("Writer " + std::to_string(id) + " is writing.");
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        print("Writer " + std::to_string(id) + " finished writing.");

        write_sem.release(); // Release lock
    }

    void run() {
        int num_ops;
        std::cout << "\n--- Reader-Writer Problem ---\n";
        std::cout << "Enter total number of operations (reads/writes): ";
        std::cin >> num_ops;

        std::vector<std::thread> threads;
        int reader_id = 1, writer_id = 1;

        std::cout << "Enter the sequence of operations (1 for Reader, 0 for Writer):\n";
        for (int i = 0; i < num_ops; ++i) {
            int op_choice;
            std::cout << "Operation " << i + 1 << ": ";
            std::cin >> op_choice;
            if (op_choice == 1) {
                threads.emplace_back(reader, reader_id++);
            } else if (op_choice == 0) {
                threads.emplace_back(writer, writer_id++);
            } else {
                std::cout << "Invalid input, skipping.\n";
            }
        }

        print("\nStarting simulation...");
        for (auto& t : threads) {
            t.join();
        }
        print("\nAll operations completed.");
    }
}

// =================================================================================
// MAIN MENU
// =================================================================================

int main() {
    int choice;
    while (true) {
        std::cout << "\n\n========= Concurrency Problems Menu =========\n";
        std::cout << "1. Dining Philosophers Problem\n";
        std::cout << "2. Reader-Writer Problem\n";
        std::cout << "3. Exit\n";
        std::cout << "===========================================\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                DP::run();
                break;
            case 2:
                RW::run();
                break;
            case 3:
                std::cout << "Exiting.\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}