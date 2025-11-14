#include<bits/stdc++.h>
using namespace std;
class Custom{
    public:
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rt;
    int pl;
    
};

int main(){
    cout<<"Non premtive priority scheduling\n";
    cout<<"Enter the number of process\n";
    int n;
    cin>>n;
    Custom a[n];
    for(int i=0;i<n;i++){
        cout<<"Enter the at:- \n";
        cin>>a[i].at;
        cout<<"Enter the bt:- \n";
        cin>>a[i].bt;
        a[i].pid = i+1;
        cout<<"Enter the priority\n";
        cin>>a[i].pl;
    }
    int visited[n];
    for(int i=0;i<n;i++){
        visited[i] = 0;
    }
    int starting_time = 0;
    int count =0;
    while(count <n){
            int idx =-1;
            int pr = INT_MAX;
        for(int i=0;i<n;i++){
            if((a[i].at<=starting_time) && (!visited[i])){
                if(pr>a[i].pl){
                pr = a[i].pl;
                idx = i;
                }
            }
        }
        if(idx!=-1){
            visited[idx] = 1;
            a[idx].ct = starting_time + a[idx].bt;
            a[idx].tat = a[idx].ct - a[idx].at;
            a[idx].wt = a[idx].tat - a[idx].bt;
            a[idx].rt = starting_time - a[idx].at;
            count ++;
            starting_time += a[idx].bt;
        }
        // cout<<count<<" \n";
    }cout<<"ID\t At\t Bt\t pl\t Ct\t tat\t wt\t rt\n";
    int avgwt=0,avgtat=0;
    for(int i=0;i<n;i++){
        avgwt+=a[i].wt;
        avgtat+=a[i].tat;
        cout<<a[i].pid<<"\t"<<a[i].at<<"\t"<<a[i].bt<<"\t"<<a[i].pl<<"\t"<<a[i].ct<<"\t"<<a[i].tat<<"\t"<<a[i].wt<<"\t"<<a[i].rt<<"\n";
    }
    cout<<"Average waiting time "<<avgwt*1.0/n<<endl;
	cout<<"Average turn around time "<<avgtat*1.0/n<<endl;;
    return 0;
}