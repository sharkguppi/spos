#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cout << "enter no of processes" << endl;
    cin >> n;
    int pid[10];
    int ar[10];
    int bt[10];
    int ct[10];
    int ta[10];
    int wt[10];
    int temp;
    float avgwt = 0, avgta = 0;
    for (int i = 0; i < n; i++)
    {
        cout << "arrival of process " << i + 1 << " ";
        cin >> ar[i];
        cout << "burst time of process " << i + 1 << " ";
        cin >> bt[i];
        pid[i] = i + 1;
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - (i + 1); j++)
        {
            if (ar[j] > ar[j + 1])
            {
                swap(ar[j], ar[j + 1]);
                swap(bt[j], bt[j + 1]);
                swap(pid[j], pid[j + 1]);
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (i == 0 or ar[i] > ct[i - 1])
            ct[i] = ar[i] + bt[i];
        else
            ct[i] = ct[i - 1] + bt[i];
        ta[i]=ct[i]-ar[i];
        wt[i]=ta[i]-bt[i];
        avgta+=ta[i];
        avgwt+=wt[i];
    }
    cout<<"\npid arrival brust complete turn waiting\n";
    for(int i = 0; i < n; i++)
    cout<<pid[i]<<"\t"<<ar[i]<<"\t"<<bt[i]<<"\t"<<ct[i]<<"\t"<<ta[i]<<"\t"<<wt[i]<<endl;
    cout<<"\naverage waiting time: "<<avgwt/n;
    cout<<"\naverage turn around time: "<<avgta/n;

}