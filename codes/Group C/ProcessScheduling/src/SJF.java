import java.util.Arrays;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;

import javax.swing.text.html.MinimalHTMLWriter;

public class SJF {
	private Scanner sc;

	public void  execute()
	{
		sc = new Scanner(System.in);

		//--------SJF
		System.out.println("Enter Number of Processes:");
		int numProcess=sc.nextInt();
		Process []process=new Process[numProcess];

		//Accept Input
		for(int i=0;i<numProcess;i++)
		{
			System.out.println("P("+(i+1)+"):Enter Arrival time, Burst time, and Priority");
			int at=sc.nextInt();
			int bt=sc.nextInt();
			int priority=sc.nextInt();

			process[i]=new Process("P"+(i+1), bt, at, priority);
		}

		int completed=0, time=0;
		double avgWT=0,avgTAT=0;
		List<Process.GanttBlock> ganttChart = new ArrayList<>();
		String lastProcess = "";

		System.out.println("\n\nPRNo\tBT\tAT\tCT\tTAT\tWT\tPR");
		System.out.println("============================================================================================");
		
		while(completed < numProcess)
		{
			int shortest = -1;
			int minBurst = Integer.MAX_VALUE;
			int highestPriority = Integer.MAX_VALUE;

			//find shortest process till time
			for(int i=0;i<numProcess;i++)
			{
				if(process[i].AT <= time && process[i].remBT > 0) {
					if (process[i].remBT < minBurst) {
						minBurst = process[i].remBT;
						highestPriority = process[i].priority;
						shortest = i;
					} else if (process[i].remBT == minBurst) {
						if (process[i].priority < highestPriority) {
							highestPriority = process[i].priority;
							shortest = i;
						}
					}
				}
			}

			if(shortest == -1) { //No process is present currently
				time++;
				continue;
			}

			// Gantt Chart Logic
			if (!process[shortest].name.equals(lastProcess)) {
				ganttChart.add(new Process.GanttBlock(process[shortest].name, time, 0));
				lastProcess = process[shortest].name;
			}

			process[shortest].remBT--;
			time++;

			if(process[shortest].remBT == 0) { //process completes its execution
				completed++;
				process[shortest].CT = time;
				process[shortest].TAT = process[shortest].CT - process[shortest].AT;
				process[shortest].WT = process[shortest].TAT - process[shortest].BT;
				
				avgWT += process[shortest].WT;
				avgTAT += process[shortest].TAT;

				process[shortest].display();
			}
		}
		
		avgTAT=(double)avgTAT/numProcess;
		avgWT=(double)avgWT/numProcess;
		System.out.println("Average Waiting Time"+avgWT);
		System.out.println("Average TAT="+avgTAT);

		drawGanttChart(ganttChart, time);
	}

	public void drawGanttChart(List<Process.GanttBlock> ganttChart, int totalTime) {
		// Set end times for blocks
		for (int i = 0; i < ganttChart.size() - 1; i++) {
			ganttChart.get(i).endTime = ganttChart.get(i+1).startTime;
		}
		ganttChart.get(ganttChart.size() - 1).endTime = totalTime;

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
