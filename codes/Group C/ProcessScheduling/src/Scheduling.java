import java.util.Scanner;

public class Scheduling {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int choice;

        while (true) {
            System.out.println("\n----- CPU Scheduling Algorithms Menu -----");
            System.out.println("1. First-Come, First-Served (FCFS)");
            System.out.println("2. Shortest Job First (SJF - Preemptive)");
            System.out.println("3. Priority (Non-Preemptive)");
            System.out.println("4. Round Robin");
            System.out.println("5. Exit");
            System.out.print("Enter your choice: ");

            choice = sc.nextInt();

            switch (choice) {
                case 1:
                    System.out.println("\n--- Executing FCFS ---");
                    FCFS fcfs = new FCFS();
                    fcfs.execute();
                    break;
                case 2:
                    System.out.println("\n--- Executing SJF (Preemptive) ---");
                    SJF sjf = new SJF();
                    sjf.execute();
                    break;
                case 3:
                    System.out.println("\n--- Executing Priority (Non-Preemptive) ---");
                    PriorityNonPreemptive pr = new PriorityNonPreemptive();
                    pr.execute();
                    break;
                case 4:
                    System.out.println("\n--- Executing Round Robin ---");
                    RoundRobin rr = new RoundRobin();
                    rr.execute();
                    break;
                case 5:
                    System.out.println("Exiting program. Goodbye!");
                    sc.close();
                    return; // Exit the main method
                default:
                    System.out.println("Invalid choice. Please try again.");
            }
        }
    }
}
