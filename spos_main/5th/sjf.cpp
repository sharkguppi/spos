#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cout << "Enter the total number of processes=" << endl;
    cin >> n;

    int at[n], bt[n], ct[n], tat[n], wt[n];
    bool done[n] = {false};

    cout << "Enter the Arrival time and Burst time for all the processes" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Arrival time for process " << i + 1 << endl;
        cin >> at[i];
        cout << "Burst time for process " << i + 1 << endl;
        cin >> bt[i];
    }

    int completed = 0, t = 0;
    while (completed < n) {
        int idx = -1;
        int min_bt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!done[i] && at[i] <= t && bt[i] < min_bt) {
                min_bt = bt[i];
                idx = i;
            }
        }

        if (idx != -1) {
            t = max(t, at[idx]);
            ct[idx] = t + bt[idx];
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            t = ct[idx];
            done[idx] = true;
            completed++;
        } else {
            t++; // If no process has arrived, increase time
        }
    }

    // Output
    float total_wt = 0, total_tat = 0;
    cout << "\n\nPid\tarr\tbur\twat\ttat\n\n";
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        cout << i + 1 << "\t" << at[i] << "\t" << bt[i]
             << "\t " << wt[i] << "\t " << tat[i] << endl;
    }

    cout << "Average waiting time " << total_wt / n << endl;
    cout << "Average turn around time " << total_tat / n << endl;

    return 0;
}
