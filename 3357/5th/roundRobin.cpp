#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

int main() {
    int n, timeQuantum;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter arrival time of process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter burst time of process " << i + 1 << ": ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
    }

    cout << "Enter time quantum: ";
    cin >> timeQuantum;

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrivalTime < b.arrivalTime;
    });

    queue<int> readyQueue;
    vector<bool> inQueue(n, false);
    int currentTime = 0, completed = 0;
    vector<pair<int, int>> ganttChart; // (Process ID, Start Time)

    // Add processes that have arrived at time 0
    for (int i = 0; i < n; ++i) {
        if (processes[i].arrivalTime <= currentTime) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {
        if (readyQueue.empty()) {
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && processes[i].arrivalTime <= currentTime) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int index = readyQueue.front();
        readyQueue.pop();

        ganttChart.push_back({processes[index].id, currentTime});

        int execTime = min(timeQuantum, processes[index].remainingTime);
        currentTime += execTime;
        processes[index].remainingTime -= execTime;

        // Check new arrivals during execution
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (processes[index].remainingTime == 0) {
            processes[index].completionTime = currentTime;
            processes[index].turnaroundTime = processes[index].completionTime - processes[index].arrivalTime;
            processes[index].waitingTime = processes[index].turnaroundTime - processes[index].burstTime;
            completed++;
        } else {
            readyQueue.push(index);
        }
    }

    // Display Process Table
    cout << "\nProcess Table:\n";
    cout << left << setw(10) << "Process"
         << setw(10) << "AT"
         << setw(10) << "BT"
         << setw(15) << "CT"
         << setw(15) << "TAT"
         << setw(15) << "WT" << endl;

    double totalWT = 0;

    for (const auto &p : processes) {
        cout << left << "P" << setw(9) << p.id
             << setw(10) << p.arrivalTime
             << setw(10) << p.burstTime
             << setw(15) << p.completionTime
             << setw(15) << p.turnaroundTime
             << setw(15) << p.waitingTime << endl;

        totalWT += p.waitingTime;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << totalWT / n << endl;

    // Gantt Chart
    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); ++i) {
        cout << "|  P" << ganttChart[i].first << "  ";
    }
    cout << "|\n";

    // Timeline under Gantt Chart
    int timeMarker = 0;
    cout << "0";
    for (size_t i = 0; i < ganttChart.size(); ++i) {
        int idx = ganttChart[i].first - 1;
        int execTime;
        if (i == ganttChart.size() - 1) {
            execTime = processes[idx].completionTime - timeMarker;
        } else {
            execTime = min(timeQuantum, processes[idx].burstTime); // Approximation
        }
        timeMarker += execTime;
        cout << setw(6) << timeMarker;
    }

    cout << endl;

    return 0;
}
