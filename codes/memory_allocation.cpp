#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric> // Required for std::accumulate

using namespace std;

struct Block {
    int id;
    int size;
    bool allocated;
};

struct Process {
    int id;
    int size;
    int blockAllocated; // Will store the block ID
};

// This function is no longer needed as we will pass copies to the functions.
// void resetAllocation(vector<Block>& blocks, vector<Process>& processes) { ... }

void printAllocation(const vector<Process>& processes, const vector<Block>& blocks) {
    cout << "\n+---------+---------------+------------------+-------------------------+\n";
    cout << "| Process | Process Size  | Block Allocated  | Internal Fragmentation  |\n";
    cout << "+---------+---------------+------------------+-------------------------+\n";

    int totalInternalFragmentation = 0;

    for (const auto& process : processes) {
        cout << "| P" << left << setw(6) << process.id
             << "| " << left << setw(14) << process.size;
        if (process.blockAllocated != -1) {
            // Find the block that was allocated to this process
            const Block* allocatedBlock = nullptr;
            for(const auto& block : blocks) {
                if(block.id == process.blockAllocated) {
                    allocatedBlock = &block;
                    break;
                }
            }
            int internalFragmentation = allocatedBlock->size - process.size;
            totalInternalFragmentation += internalFragmentation;
            cout << "| Block " << left << setw(11) << process.blockAllocated
                 << "| " << left << setw(24) << internalFragmentation << "|\n";
        } else {
            cout << "| Not Allocated" << left << setw(5) << ""
                 << "| " << left << setw(24) << "N/A" << "|\n";
        }
    }
    cout << "+---------+---------------+------------------+-------------------------+\n";

    int totalExternalFragmentation = 0;
    cout << "\nUnused Blocks (External Fragmentation): ";
    bool first = true;
    for (const auto& block : blocks) {
        if (!block.allocated) {
            if(!first) cout << ", ";
            cout << "Block " << block.id << " (" << block.size << ")";
            totalExternalFragmentation += block.size;
            first = false;
        }
    }
    if(first) cout << "None";
    cout << "\n";

    cout << "\nTotal Internal Fragmentation: " << totalInternalFragmentation << "\n";
    cout << "Total External Fragmentation: " << totalExternalFragmentation << "\n";
}

void firstFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& process : processes) {
        for (auto& block : blocks) {
            if (!block.allocated && block.size >= process.size) {
                process.blockAllocated = block.id;
                block.allocated = true; // The entire block is now marked as used
                break; // Move to the next process
            }
        }
    }
    cout << "\n--- First Fit Allocation ---";
    printAllocation(processes, blocks);
}

void bestFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& process : processes) {
        int bestIdx = -1;
        for (int j = 0; j < blocks.size(); j++) {
            if (!blocks[j].allocated && blocks[j].size >= process.size) {
                if (bestIdx == -1 || blocks[j].size < blocks[bestIdx].size)
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            process.blockAllocated = blocks[bestIdx].id;
            blocks[bestIdx].allocated = true; // The entire block is now marked as used
        }
    }
    cout << "\n--- Best Fit Allocation ---";
    printAllocation(processes, blocks);
}

void worstFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& process : processes) {
        int worstIdx = -1;
        for (int j = 0; j < blocks.size(); j++) {
            if (!blocks[j].allocated && blocks[j].size >= process.size) {
                if (worstIdx == -1 || blocks[j].size > blocks[worstIdx].size)
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            process.blockAllocated = blocks[worstIdx].id;
            blocks[worstIdx].allocated = true; // The entire block is now marked as used
        }
    }
    cout << "\n--- Worst Fit Allocation ---";
    printAllocation(processes, blocks);
}

void nextFit(vector<Block> blocks, vector<Process> processes) {
    int lastPos = 0;
    for (auto& process : processes) {
        int count = 0;
        while (count < blocks.size()) {
            if (!blocks[lastPos].allocated && blocks[lastPos].size >= process.size) {
                process.blockAllocated = blocks[lastPos].id;
                blocks[lastPos].allocated = true; // The entire block is now marked as used
                lastPos = (lastPos + 1) % blocks.size();
                goto next_process; // Break outer loop and go to next process
            }
            lastPos = (lastPos + 1) % blocks.size();
            count++;
        }
        next_process:;
    }
    cout << "\n--- Next Fit Allocation ---";
    printAllocation(processes, blocks);
}

int main() {
    vector<Block> blocks;
    vector<Process> processes;
    int nBlocks, nProcesses;

    cout << "Enter number of memory blocks: ";
    cin >> nBlocks;
    blocks.resize(nBlocks);
    for (int i = 0; i < nBlocks; i++) {
        cout << "Enter size of Block " << i + 1 << ": ";
        cin >> blocks[i].size;
        blocks[i].id = i + 1;
        blocks[i].allocated = false;
    }

    cout << "\nEnter number of processes: ";
    cin >> nProcesses;
    processes.resize(nProcesses);
    for (int i = 0; i < nProcesses; i++) {
        cout << "Enter size of Process " << i + 1 << ": ";
        cin >> processes[i].size;
        processes[i].id = i + 1;
        processes[i].blockAllocated = -1;
    }

    int choice;
    do {
        cout << "\n\n===== Memory Allocation Menu =====\n";
        cout << "1. First Fit\n";
        cout << "2. Best Fit\n";
        cout << "3. Worst Fit\n";
        cout << "4. Next Fit\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            // Each function receives a fresh copy of the original blocks and processes
            case 1: firstFit(blocks, processes); break;
            case 2: bestFit(blocks, processes); break;
            case 3: worstFit(blocks, processes); break;
            case 4: nextFit(blocks, processes); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
