#include<bits/stdc++.h>
using namespace std;

int main(){
	int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time,n;
    bool check = false;

	int wat=0,tat=0;

	cout<<"Enter the total number of process="<<endl;
	cin>>n;
	// assigning the number of process to remain variable

	int at[n],bt[n],rt[n],wt[n],taty[n];

	cout<<"Enter the Arrival time, Burst time for All the processes"<<endl;
	for(int i=0;i<n;i++)
	{
	  cout<<"Arrival time for process "<<i+1<<endl;
		cin>>at[i];
		cout<<"Burst time for process "<<i+1<<endl;
		cin>>bt[i];
		rt[i]=bt[i];
	}
    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if ((at[j] <= t) and (rt[j] < minm) and rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = true;
            }
        }
  
        if (check == false) {
            t++;
            continue;
        }
  
        // Reduce remaining time by one
        rt[shortest]--;
  
        // Update minimum
        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;
        if (rt[shortest] == 0) {
            complete++;
            check = false;
            finish_time = t + 1;
            wt[shortest] = finish_time -bt[shortest] -at[shortest];
  
            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        // Increment time
        t++;
    }
    for (int i = 0; i < n; i++)
        taty[i] = bt[i] + wt[i];


	cout<<"\n\nPid\tarr\tbur\twat\ttat\n\n";
	for (int i = 0; i < n; i++) {
        wat = wat + wt[i];
        tat = tat + taty[i];
        cout << i+1 << "\t"<<at[i]<<"\t"<< bt[i] << "\t " << wt[i]<< "\t " << taty[i] << endl;
    }

	cout<<"Average waiting time "<<wat*1.0/n<<endl;
	cout<<"Average turn around time "<<tat*1.0/n<<endl;;

	return 0;
}