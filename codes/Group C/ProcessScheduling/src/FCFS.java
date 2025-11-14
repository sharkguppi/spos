import java.util.Arrays;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;


public class FCFS {

    private Scanner sc;

    public void  execute()
    {
        sc = new Scanner(System.in);

        //--------FCFS
        System.out.println("Enter Number of Processes:");
        int numProcess=sc.nextInt();
        Process []process=new Process[numProcess];

        //Accept Input
        for(int i=0;i<numProcess;i++)
        {
            System.out.println("P("+(i+1)+"):Enter Arrival time & Burst time");
            int at=sc.nextInt();
            int bt=sc.nextInt();
            //System.out.println("P("+(i+1)+"):Enter Arrival time");

            process[i]=new Process("P"+(i+1), bt, at);
        }

        //Sorting processes according to Arrival Time //No need if you take AT=0 or in ascending order
        Arrays.sort(process,new SortByArrival());

        int sum=0;
        double avgWT=0,avgTAT=0;
        List<Process.GanttBlock> ganttChart = new ArrayList<>();
        System.out.println("\n\nPRNo\tBT\tAT\tCT\tTAT\tWT");
        System.out.println("============================================================================================");
        for(int i=0;i<numProcess;i++)
        {
            if (sum < process[i].AT) {
                sum = process[i].AT;
            }
            ganttChart.add(new Process.GanttBlock(process[i].name, sum, sum + process[i].BT));
            sum=process[i].CT=sum+process[i].BT;
            process[i].TAT=process[i].CT-process[i].AT;
            process[i].WT=process[i].TAT-process[i].BT;

            avgWT=avgWT+process[i].WT;
            avgTAT=avgTAT+process[i].TAT;

            process[i].display();
        }
        avgTAT=(double)avgTAT/numProcess;
        avgWT=(double)avgWT/numProcess;
        System.out.println("Average Waiting Time"+avgWT);
        System.out.println("Average TAT="+avgTAT);

        drawGanttChart(ganttChart);
    }

    public void drawGanttChart(List<Process.GanttBlock> ganttChart) {
        System.out.println("\n\n----- Gantt Chart -----");
        // Top border
        System.out.print("+");
        for (Process.GanttBlock block : ganttChart) {
            for (int i = 0; i < block.processName.length() + 2; i++) System.out.print("-");
            System.out.print("+");
        }
        System.out.println();

        // Process names
        System.out.print("|");
        for (Process.GanttBlock block : ganttChart) {
            System.out.print(" " + block.processName + " |");
        }
        System.out.println();

        // Bottom border
        System.out.print("+");
        for (Process.GanttBlock block : ganttChart) {
            for (int i = 0; i < block.processName.length() + 2; i++) System.out.print("-");
            System.out.print("+");
        }
        System.out.println();

        // Timestamps
        System.out.print(ganttChart.get(0).startTime);
        for (Process.GanttBlock block : ganttChart) {
            String endTimeStr = String.valueOf(block.endTime);
            int spacing = block.processName.length() + 3 - endTimeStr.length();
            for (int i = 0; i < spacing; i++) System.out.print(" ");
            System.out.print(endTimeStr);
        }
        System.out.println();
    }
}
