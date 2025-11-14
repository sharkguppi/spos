import java.util.*;
public class priority {
    public static void main(String args[]){
        Scanner sc = new Scanner(System.in);
        System.out.println("enter number of process: ");
        int n = sc.nextInt();
        int pid[] = new int[n];
        int pri[] = new int[n];
        int ar[] = new int[n];
        int bt[] = new int[n];
        int ct[] = new int[n];
        int ta[] = new int[n];
        int wt[] = new int[n];
        float avgwt = 0, avgta = 0;
        for (int i = 0; i < n; i++) {
            System.out.println("enter process " + (i + 1) + " arrival time");
            ar[i] = sc.nextInt();
            System.out.println("enter process " + (i + 1) + " burst time");
            bt[i] = sc.nextInt();
            System.out.println("enter process " + (i + 1) + "priority");
            pri[i]= sc.nextInt();
            pid[i] = i + 1;
        }
        sortarr(ar,bt,pri,pid,n);

        for (int i = 0; i < n; i++) {
            if (i == 0)
                ct[i] = ar[i] + bt[i];
            else if (ar[i] > ct[i - 1])
                ct[i] = ar[i] + bt[i];
            else
                ct[i] = ct[i - 1] + bt[i];
            ta[i]=ct[i]-ar[i];
            wt[i]=ta[i]-bt[i];
            avgta+=ta[i];
            avgwt+=wt[i];
        }
        System.out.println("\npid priority arrival brust complete turn waiting");
        for(int i = 0; i < n; i++)
        System.out.println(pid[i]+ " \t "+pri[i]+" \t"+ar[i]+" \t"+bt[i]+" \t"+ct[i]+" \t"+ta[i]+" \t"+wt[i]);
        sc.close();
        System.out.println("\naverage waiting time: "+(avgwt/n));
        System.out.println("\naverage turn around time: "+(avgta/n));
    }
    static void sortarr(int at[],int bt[],int pri[],int pid[],int n){
        int temp;
        for(int i=0;i<n;i++){
            for(int j=0;j<n-(i+1);i++){
                if(at[j]>at[j+1]){
                    temp=at[j];
                    at[j]=at[j+1];
                    at[j+1]=temp;

                    temp=bt[j];
                    bt[j]=bt[j+1];
                    bt[j+1]=temp;

                    temp=pri[j];
                    pri[j]=pri[j+1];
                    pri[j+1]=temp;

                    temp=pid[j];
                    pid[j]=pid[j+1];
                    pid[j+1]=temp;

                }
                if(at[j]==at[j+1]){
                    if(pri[j]>pri[j+1])
                    {
                        temp = at[j];
                        at[j] = at[j + 1];
                        at[j + 1] = temp;

                        //swapping burst time
                        temp = bt[j];
                        bt[j] = bt[j + 1];
                        bt[j + 1] = temp;

                        //swapping priority
                        temp = pri[j];
                        pri[j] = pri[j + 1];
                        pri[j + 1] = temp;

                        temp=pid[j];
                        pid[j]=pid[j+1];
                        pid[j+1]=temp;

                    }
                }
            }
        }
    }
}
