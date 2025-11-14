import java.util.Arrays;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;

public class RoundRobin {
	private Scanner sc;
	public void execute()
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
		Arrays.sort(process,new SortByArrival()); //sort according to arrival time

		System.out.println("Enter Quantum Time: ");
		int quantum=sc.nextInt();
		
		double avgWT=0,avgTAT=0;
		int time=0;
		List<Process.GanttBlock> ganttChart = new ArrayList<>();
		System.out.println("\n\nPRNo\tBT\tAT\tCT\tTAT\tWT\tPR");
		System.out.println("============================================================================================");
		while(true)
		{
			boolean done=true;
			for(int i=0;i<numProcess;i++)
			{
				if(process[i].remBT>0 && process[i].AT<=time)
				{
					done=false;
					
					if(process[i].remBT>quantum) //Ajun time remaining aahe :v
					{
						ganttChart.add(new Process.GanttBlock(process[i].name, time, time + quantum));
						time=time+quantum;
						process[i].remBT=process[i].remBT-quantum;
						// System.out.println(process[i].name + " ran till TIME " + time);
						
					}
					else //process will finish execution
					{
						ganttChart.add(new Process.GanttBlock(process[i].name, time, time + process[i].remBT));
						time+=process[i].remBT;
						// System.out.println(process[i].name + " ran till TIME " + time);

						process[i].remBT=0;
						process[i].CT=time;
						process[i].TAT=process[i].CT-process[i].AT;
						process[i].WT=process[i].TAT-process[i].BT;
						avgWT+=process[i].WT;
						avgTAT+=process[i].TAT;
						process[i].display();
					}
				}
				/*else //no process is arrived currently
				{
					time++;
					
				}*/
				
			}
			if(done==true) //done executing all processes
			{
				break;
			}

		}
		drawGanttChart(ganttChart);
    }

    public void drawGanttChart(List<Process.GanttBlock> ganttChart) {
        System.out.println("\n\n----- Gantt Chart -----");
        System.out.print("+");
        for (Process.GanttBlock block : ganttChart) {
            int width = Math.max(block.processName.length() + 2, 4);
            for (int i = 0; i < width; i++) System.out.print("-");
            System.out.print("+");
        }
        System.out.println();

        System.out.print("|");
        for (Process.GanttBlock block : ganttChart) {
            int width = Math.max(block.processName.length() + 2, 4);
            System.out.printf(" %-" + (width - 2) + "s |", block.processName);
        }
        System.out.println();

        System.out.print("+");
        for (Process.GanttBlock block : ganttChart) {
            int width = Math.max(block.processName.length() + 2, 4);
            for (int i = 0; i < width; i++) System.out.print("-");
            System.out.print("+");
        }
        System.out.println();

        System.out.print(ganttChart.get(0).startTime);
        for (Process.GanttBlock block : ganttChart) {
            String endTimeStr = String.valueOf(block.endTime);
            int width = Math.max(block.processName.length() + 2, 4);
            int spacing = width + 1 - endTimeStr.length();
            for (int i = 0; i < spacing; i++) System.out.print(" ");
            System.out.print(endTimeStr);
        }
        System.out.println();
    }
}
