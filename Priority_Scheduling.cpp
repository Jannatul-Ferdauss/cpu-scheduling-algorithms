#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int start_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    bool is_completed;
};

bool compareArrival(Process a, Process b) {
    return a.arrival_time < b.arrival_time;
}

int main() {
    int n;
    cout << "Number of processes, n: ";
    cin >> n;
    vector<Process> processes(n);
    vector<Process> execution_order;

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter the burst time of P" << i+1 << ": ";
        cin >> processes[i].burst_time;
        cout << "Enter the arrival time of P" << i+1 << ": ";
        cin >> processes[i].arrival_time;
        cout << "Enter the priority of P" << i+1 << ": ";
        cin >> processes[i].priority;
        processes[i].is_completed = false;
    }

    sort(processes.begin(), processes.end(), compareArrival);

    int current_time = 0, completed = 0;
    float total_waiting_time = 0, total_turnaround_time = 0;

    cout << "\nGantt Chart\n";

    while (completed != n) {
        int idx = -1;
        int highest_priority = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !processes[i].is_completed) {
                if (processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    idx = i;
                }
                else if (processes[i].priority == highest_priority) {
                    if (processes[i].arrival_time < processes[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            processes[idx].start_time = current_time;
            processes[idx].completion_time = processes[idx].start_time + processes[idx].burst_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;

            total_waiting_time += processes[idx].waiting_time;
            total_turnaround_time += processes[idx].turnaround_time;

            processes[idx].is_completed = true;
            completed++;

            execution_order.push_back(processes[idx]); // Save the execution order

            cout << current_time << " P" << processes[idx].id << " ";

            current_time = processes[idx].completion_time;
        } else {
            current_time++; // CPU idle
        }
    }
    cout << current_time << endl;

    for (auto p : execution_order) {
        cout << "Process: P" << p.id
             << " Start time: " << p.start_time
             << " Waiting time: " << p.waiting_time
             << " Turnaround time: " << p.turnaround_time
             << endl;
    }

    cout << "Average waiting time: " << (total_waiting_time / n) << endl;
    cout << "Average turnaround time: " << (total_turnaround_time / n) << endl;

    return 0;
}
