#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <limits> // Required for numeric_limits

using namespace std;

// Function to display the current state of frames
void displayFrames(const vector<int>& frames) {
    cout << "[";
    for (size_t i = 0; i < frames.size(); ++i) {
        cout << frames[i] << (i == frames.size() - 1 ? "" : ", ");
    }
    cout << "]\n";
}

// Function to simulate FIFO page replacement
int fifo(const vector<int>& pages, int frames, int& hits) {
    unordered_set<int> s;
    queue<int> indexes;
    int page_faults = 0;

    cout << "\n--- FIFO Simulation ---\n";
    vector<int> current_frames;

    for (int page : pages) {
        cout << "Accessing page " << page << ": ";
        if (s.find(page) != s.end()) {
            hits++;
            cout << "Hit.  ";
        } else {
            page_faults++;
            cout << "Miss. ";
            if (s.size() < frames) {
                s.insert(page);
                indexes.push(page);
            } else {
                int val = indexes.front();
                indexes.pop();
                s.erase(val);
                s.insert(page);
                indexes.push(page);
            }
        }
        
        // Update current_frames for display
        current_frames.clear();
        queue<int> temp_q = indexes;
        while(!temp_q.empty()){
            current_frames.push_back(temp_q.front());
            temp_q.pop();
        }
        displayFrames(current_frames);
    }
    return page_faults;
}

// Function to simulate LRU page replacement
int lru(const vector<int>& pages, int frames, int& hits) {
    unordered_set<int> s;
    unordered_map<int, int> indexes;
    int page_faults = 0;

    cout << "\n--- LRU Simulation ---\n";
    for (int i = 0; i < (int)pages.size(); ++i) {
        int page = pages[i];
        cout << "Accessing page " << page << ": ";

        if (s.find(page) != s.end()) {
            hits++;
            cout << "Hit.  ";
        } else {
            page_faults++;
            cout << "Miss. ";
            if (s.size() < frames) {
                s.insert(page);
            } else {
                int lru_page = -1, min_index = i;
                for (auto p : s) {
                    if (indexes[p] < min_index) {
                        min_index = indexes[p];
                        lru_page = p;
                    }
                }
                s.erase(lru_page);
                s.insert(page);
            }
        }
        indexes[page] = i; // Update the last used time for the page

        // Display current frames
        vector<int> frames_vec(s.begin(), s.end());
        sort(frames_vec.begin(), frames_vec.end());
        displayFrames(frames_vec);
    }
    return page_faults;
}

// Function to simulate Optimal page replacement
int optimal(const vector<int>& pages, int frames, int& hits) {
    unordered_set<int> s;
    int page_faults = 0;

    cout << "\n--- Optimal Simulation ---\n";
    for (int i = 0; i < (int)pages.size(); ++i) {
        int page = pages[i];
        cout << "Accessing page " << page << ": ";

        if (s.find(page) != s.end()) {
            hits++;
            cout << "Hit.  ";
        } else {
            page_faults++;
            cout << "Miss. ";
            if (s.size() < frames) {
                s.insert(page);
            } else {
                int farthest = -1, page_to_replace = -1;
                for (int p_in_frame : s) {
                    int future_pos = -1;
                    for (int j = i + 1; j < (int)pages.size(); ++j) {
                        if (pages[j] == p_in_frame) {
                            future_pos = j;
                            break;
                        }
                    }
                    if (future_pos == -1) { // Page not found in the future, best to replace
                        page_to_replace = p_in_frame;
                        break;
                    }
                    if (future_pos > farthest) {
                        farthest = future_pos;
                        page_to_replace = p_in_frame;
                    }
                }
                s.erase(page_to_replace);
                s.insert(page);
            }
        }
        
        // Display current frames
        vector<int> frames_vec(s.begin(), s.end());
        sort(frames_vec.begin(), frames_vec.end());
        displayFrames(frames_vec);
    }
    return page_faults;
}

int main() {
    int frames, choice;
    vector<int> pages;
    int n;

    cout << "Enter number of frames: ";
    cin >> frames;

    cout << "Enter number of pages in reference string: ";
    cin >> n;

    cout << "Enter reference string (space separated): ";
    pages.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> pages[i];
    }

    do {
        cout << "\nPage Replacement Algorithms Menu:\n";
        cout << "1. FIFO\n2. LRU\n3. Optimal\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Input validation to prevent infinite loop
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            choice = 0; // Reset choice to continue loop safely
            continue;
        }

        int hits = 0;
        int faults = 0;

        switch (choice) {
            case 1: {
                faults = fifo(pages, frames, hits);
                cout << "\nTotal page misses (FIFO): " << faults << "\n";
                cout << "Total page hits (FIFO): " << hits << "\n";
                break;
            }
            case 2: {
                faults = lru(pages, frames, hits);
                cout << "\nTotal page misses (LRU): " << faults << "\n";
                cout << "Total page hits (LRU): " << hits << "\n";
                break;
            }
            case 3: {
                faults = optimal(pages, frames, hits);
                cout << "\nTotal page misses (Optimal): " << faults << "\n";
                cout << "Total page hits (Optimal): " << hits << "\n";
                break;
            }
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
