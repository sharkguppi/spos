#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

// ======================================================
// Shared Process Structure
// ======================================================
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

// ======================================================
// Helper: Print Gantt Chart
// ======================================================
void printGanttChart(const vector<pair<int, int>>& gantt, const vector<Process>& processes) {
    cout << "\nGantt Chart:\n";
    cout << "-------------------------------------------------\n";
    for (auto &g : gantt)
        cout << "|  P" << g.first << "  ";
    cout << "|\n-------------------------------------------------\n";
    cout << gantt.front().second;
    for (size_t i = 1; i <= gantt.size(); ++i) {
        int nextTime = (i < gantt.size()) ? gantt[i].second : processes[gantt[i - 1].first - 1].completionTime;
        cout << setw(6) << nextTime;
    }
    cout << "\n";
}

// ======================================================
// FCFS Scheduling
// ======================================================
void fcfs() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Enter Arrival Time and Burst Time for Process " << p[i].id << ": ";
        cin >> p[i].arrivalTime >> p[i].burstTime;
    }

    sort(p.begin(), p.end(), [](const Process &a, const Process &b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    float totalWT = 0, totalTAT = 0;
    vector<pair<int, int>> gantt;

    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &proc : p) {
        if (currentTime < proc.arrivalTime)
            currentTime = proc.arrivalTime;
        gantt.push_back({proc.id, currentTime});
        currentTime += proc.burstTime;
        proc.completionTime = currentTime;
        proc.turnaroundTime = proc.completionTime - proc.arrivalTime;
        proc.waitingTime = proc.turnaroundTime - proc.burstTime;

        totalWT += proc.waitingTime;
        totalTAT += proc.turnaroundTime;

        cout << "P" << proc.id << "\t" << proc.arrivalTime << "\t" << proc.burstTime << "\t"
             << proc.completionTime << "\t" << proc.turnaroundTime << "\t" << proc.waitingTime << "\n";
    }

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << "\n";
    printGanttChart(gantt, p);
}

// ======================================================
// SJF Scheduling (Non-preemptive)
// ======================================================
void sjf() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Enter Arrival Time and Burst Time for Process " << p[i].id << ": ";
        cin >> p[i].arrivalTime >> p[i].burstTime;
    }

    int completed = 0, currentTime = 0;
    vector<bool> done(n, false);
    float totalWT = 0, totalTAT = 0;
    vector<pair<int, int>> gantt;

    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    while (completed != n) {
        int idx = -1, minBT = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrivalTime <= currentTime && p[i].burstTime < minBT) {
                minBT = p[i].burstTime;
                idx = i;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        gantt.push_back({p[idx].id, currentTime});
        currentTime += p[idx].burstTime;
        p[idx].completionTime = currentTime;
        p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
        p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;

        done[idx] = true;
        completed++;
        totalWT += p[idx].waitingTime;
        totalTAT += p[idx].turnaroundTime;

        cout << "P" << p[idx].id << "\t" << p[idx].arrivalTime << "\t" << p[idx].burstTime << "\t"
             << p[idx].completionTime << "\t" << p[idx].turnaroundTime << "\t" << p[idx].waitingTime << "\n";
    }

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << "\n";
    printGanttChart(gantt, p);
}

// ======================================================
// Priority Scheduling (Non-preemptive) with Aging
// ======================================================
void priorityScheduling() {
    int n, agingFactor;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Enter Arrival Time, Burst Time, and Priority for Process " << p[i].id << ": ";
        cin >> p[i].arrivalTime >> p[i].burstTime >> p[i].priority;
    }

    cout << "Enter aging factor (priority improvement rate per waiting unit, e.g., 1): ";
    cin >> agingFactor;

    int completed = 0, currentTime = 0;
    vector<bool> done(n, false);
    float totalWT = 0, totalTAT = 0;
    vector<pair<int, int>> gantt;

    cout << "\nProcess\tAT\tBT\tPR\tCT\tTAT\tWT\n";

    while (completed != n) {
        // Apply aging: every process that is ready but not yet executed gets priority boost
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrivalTime <= currentTime)
                p[i].priority = max(1, p[i].priority - agingFactor); // lower number = higher priority
        }

        // Pick process with highest (lowest numeric) priority
        int idx = -1, highestPriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrivalTime <= currentTime && p[i].priority < highestPriority) {
                highestPriority = p[i].priority;
                idx = i;
            }
        }

        // If no process has arrived yet
        if (idx == -1) {
            currentTime++;
            continue;
        }

        // Execute selected process
        gantt.push_back({p[idx].id, currentTime});
        currentTime += p[idx].burstTime;
        p[idx].completionTime = currentTime;
        p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
        p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;

        done[idx] = true;
        completed++;
        totalWT += p[idx].waitingTime;
        totalTAT += p[idx].turnaroundTime;

        cout << "P" << p[idx].id << "\t" << p[idx].arrivalTime << "\t" << p[idx].burstTime << "\t"
             << p[idx].priority << "\t" << p[idx].completionTime << "\t"
             << p[idx].turnaroundTime << "\t" << p[idx].waitingTime << "\n";
    }

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << "\n";
    printGanttChart(gantt, p);
}


// ======================================================
// Round Robin Scheduling (with Gantt Chart)
// ======================================================
void roundRobin() {
    int n, tq;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Enter Arrival Time and Burst Time for Process " << p[i].id << ": ";
        cin >> p[i].arrivalTime >> p[i].burstTime;
        p[i].remainingTime = p[i].burstTime;
    }
    cout << "Enter Time Quantum: ";
    cin >> tq;

    queue<int> q;
    vector<bool> inQueue(n, false);
    int t = 0, completed = 0;
    float totalWT = 0, totalTAT = 0;
    vector<pair<int, int>> gantt;

    while (completed < n) {
        for (int i = 0; i < n; i++)
            if (!inQueue[i] && p[i].arrivalTime <= t && p[i].remainingTime > 0) {
                q.push(i);
                inQueue[i] = true;
            }

        if (q.empty()) { t++; continue; }

        int idx = q.front(); q.pop();
        gantt.push_back({p[idx].id, t});
        int exec = min(tq, p[idx].remainingTime);
        p[idx].remainingTime -= exec;
        t += exec;

        for (int i = 0; i < n; i++)
            if (!inQueue[i] && p[i].arrivalTime <= t && p[i].remainingTime > 0) {
                q.push(i);
                inQueue[i] = true;
            }

        if (p[idx].remainingTime > 0)
            q.push(idx);
        else {
            completed++;
            p[idx].completionTime = t;
            p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
            totalWT += p[idx].waitingTime;
            totalTAT += p[idx].turnaroundTime;
        }
    }

    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &proc : p)
        cout << "P" << proc.id << "\t" << proc.arrivalTime << "\t" << proc.burstTime << "\t"
             << proc.completionTime << "\t" << proc.turnaroundTime << "\t" << proc.waitingTime << "\n";

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << "\n";
    printGanttChart(gantt, p);
}

// ======================================================
// Main Menu
// ======================================================
int main() {
    int choice;
    do {
        cout << "\n===== CPU Scheduling Algorithms =====\n";
        cout << "1. First Come First Serve (FCFS)\n";
        cout << "2. Shortest Job First (SJF)\n";
        cout << "3. Priority Scheduling (Non-Preemptive)\n";
        cout << "4. Round Robin\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: cout << "\n--- FCFS Scheduling ---\n"; fcfs(); break;
            case 2: cout << "\n--- SJF Scheduling ---\n"; sjf(); break;
            case 3: cout << "\n--- Priority Scheduling (Non-Preemptive) ---\n"; priorityScheduling(); break;
            case 4: cout << "\n--- Round Robin Scheduling ---\n"; roundRobin(); break;
            case 5: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice! Please try again.\n"; break;
        }
    } while (choice != 5);
    return 0;
}
