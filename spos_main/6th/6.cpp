#include <iostream>
#include <vector>
#include <limits>
using namespace std;

struct Block {
    int size;
    bool occupied;
    int processID;

    Block(int s) : size(s), occupied(false), processID(-1) {}
};

void resetMemory(vector<Block>& memory) {
    for (auto& block : memory) {
        block.occupied = false;
        block.processID = -1;
    }
}

void printMemory(const vector<Block>& memory, const vector<int>& processes, const string& title) {
    cout << "\n--- " << title << " ---\n";
    for (int i = 0; i < memory.size(); ++i) {
        cout << "Block " << i << " (Size: " << memory[i].size << "): ";
        if (memory[i].occupied)
            cout << "Occupied by Process " << memory[i].processID
                 << " (Size: " << processes[memory[i].processID] << ")\n";
        else
            cout << "Free\n";
    }
}

void firstFit(vector<Block> memory, const vector<int>& processes) {
    for (int i = 0; i < processes.size(); ++i) {
        bool allocated = false;
        for (auto& block : memory) {
            if (!block.occupied && block.size >= processes[i]) {
                block.occupied = true;
                block.processID = i;
                allocated = true;
                break;
            }
        }
        if (!allocated)
            cout << "Process " << i << " (Size: " << processes[i] << ") => Not Allocated\n";
    }
    printMemory(memory, processes, "First Fit Allocation");
}

void nextFit(vector<Block> memory, const vector<int>& processes) {
    int lastIndex = 0;
    for (int i = 0; i < processes.size(); ++i) {
        bool allocated = false;
        int count = 0;
        int index = lastIndex;

        while (count < memory.size()) {
            if (!memory[index].occupied && memory[index].size >= processes[i]) {
                memory[index].occupied = true;
                memory[index].processID = i;
                lastIndex = index;
                allocated = true;
                break;
            }
            index = (index + 1) % memory.size();
            count++;
        }

        if (!allocated)
            cout << "Process " << i << " (Size: " << processes[i] << ") => Not Allocated\n";
    }
    printMemory(memory, processes, "Next Fit Allocation");
}

void bestFit(vector<Block> memory, const vector<int>& processes) {
    for (int i = 0; i < processes.size(); ++i) {
        int bestIndex = -1;
        int minDiff = numeric_limits<int>::max();

        for (int j = 0; j < memory.size(); ++j) {
            if (!memory[j].occupied && memory[j].size >= processes[i]) {
                int diff = memory[j].size - processes[i];
                if (diff < minDiff) {
                    minDiff = diff;
                    bestIndex = j;
                }
            }
        }

        if (bestIndex != -1) {
            memory[bestIndex].occupied = true;
            memory[bestIndex].processID = i;
        } else {
            cout << "Process " << i << " (Size: " << processes[i] << ") => Not Allocated\n";
        }
    }
    printMemory(memory, processes, "Best Fit Allocation");
}

void worstFit(vector<Block> memory, const vector<int>& processes) {
    for (int i = 0; i < processes.size(); ++i) {
        int worstIndex = -1;
        int maxDiff = -1;

        for (int j = 0; j < memory.size(); ++j) {
            if (!memory[j].occupied && memory[j].size >= processes[i]) {
                int diff = memory[j].size - processes[i];
                if (diff > maxDiff) {
                    maxDiff = diff;
                    worstIndex = j;
                }
            }
        }

        if (worstIndex != -1) {
            memory[worstIndex].occupied = true;
            memory[worstIndex].processID = i;
        } else {
            cout << "Process " << i << " (Size: " << processes[i] << ") => Not Allocated\n";
        }
    }
    printMemory(memory, processes, "Worst Fit Allocation");
}

int main() {
    vector<Block> memory = { Block(100), Block(500), Block(200), Block(300), Block(600) };
    vector<int> processes = { 212, 417, 112, 426 };

    firstFit(memory, processes);

    resetMemory(memory);
    nextFit(memory, processes);

    resetMemory(memory);
    bestFit(memory, processes);

    resetMemory(memory);
    worstFit(memory, processes);

    return 0;
}
