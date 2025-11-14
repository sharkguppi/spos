#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;

void fifoPageReplacement(vector<int> pages, int frameSize) {
    unordered_set<int> frames;
    queue<int> indexQueue;

    int pageFaults = 0;

    for (int page : pages) {
        if (frames.find(page) == frames.end()) {
            if (frames.size() == frameSize) {
                int oldest = indexQueue.front();
                indexQueue.pop();
                frames.erase(oldest);
            }
            frames.insert(page);
            indexQueue.push(page);
            pageFaults++;
        }
        cout << "Page: " << page << " -> ";
        for (int f : frames) cout << f << " ";
        cout << "\n";
    }

    cout << "Total Page Faults (FIFO): " << pageFaults << "\n";
}

void lruPageReplacement(vector<int> pages, int frameSize) {
    unordered_set<int> frames;
    unordered_map<int, int> lastUsed;

    int pageFaults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];

        if (frames.find(page) == frames.end()) {
            if (frames.size() == frameSize) {
                int lru = pages[i];
                int leastRecent = i;
                for (int f : frames) {
                    if (lastUsed[f] < leastRecent) {
                        leastRecent = lastUsed[f];
                        lru = f;
                    }
                }
                frames.erase(lru);
            }
            frames.insert(page);
            pageFaults++;
        }
        lastUsed[page] = i;

        cout << "Page: " << page << " -> ";
        for (int f : frames) cout << f << " ";
        cout << "\n";
    }

    cout << "Total Page Faults (LRU): " << pageFaults << "\n";
}

void optimalPageReplacement(vector<int> pages, int frameSize) {
    unordered_set<int> frames;
    int pageFaults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];

        if (frames.find(page) == frames.end()) {
            if (frames.size() == frameSize) {
                int farthest = i + 1, toRemove = -1;

                for (int f : frames) {
                    int j;
                    for (j = i + 1; j < pages.size(); j++) {
                        if (pages[j] == f) break;
                    }
                    if (j > farthest) {
                        farthest = j;
                        toRemove = f;
                    } else if (j == pages.size()) {
                        toRemove = f;
                        break;
                    }
                }

                frames.erase(toRemove);
            }
            frames.insert(page);
            pageFaults++;
        }

        cout << "Page: " << page << " -> ";
        for (int f : frames) cout << f << " ";
        cout << "\n";
    }

    cout << "Total Page Faults (Optimal): " << pageFaults << "\n";
}

int main() {
    int frameSize, n, choice;
    vector<int> pages;

    cout << "Enter number of pages: ";
    cin >> n;
    pages.resize(n);

    cout << "Enter page reference string:\n";
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }
    cout << "Enter number of frames: ";
    cin >> frameSize;

    do {
        cout << "\nPage Replacement Algorithms\n";
        cout << "1. FIFO\n";
        cout << "2. LRU\n";
        cout << "3. Optimal\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: fifoPageReplacement(pages, frameSize); break;
            case 2: lruPageReplacement(pages, frameSize); break;
            case 3: optimalPageReplacement(pages, frameSize); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}