#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int id;
    int burstTime;
    int arrivalTime;
    int startTime;
    int waitingTime;
    int turnaroundTime;
};


bool compareArrival(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

int main() {
    int n;
    cout << "Number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input burst time and arrival time
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> processes[i].burstTime;
        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> processes[i].arrivalTime;
    }

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), compareArrival);

    int currentTime = 0;
    float totalWaitingTime = 0, totalTurnaroundTime = 0;

    cout << "\nGantt Chart\n";
    for (int i = 0; i < n; i++) {
        // If CPU is idle, jump to next arrival
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;

        processes[i].startTime = currentTime;
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        currentTime += processes[i].burstTime;

        cout << processes[i].startTime << " P" << processes[i].id << " ";
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }
    cout << currentTime << endl;

    cout << fixed << setprecision(1);
    for (int i = 0; i < n; i++) {
        cout << "Process: P" << processes[i].id
             << " Start time: " << processes[i].startTime
             << " Waiting time: " << processes[i].waitingTime
             << " Turnaround time: " << processes[i].turnaroundTime << endl;
    }

    cout << "Average waiting time : " << totalWaitingTime / n << endl;
    cout << "Average turnaround time : " << totalTurnaroundTime / n << endl;

    return 0;
}

