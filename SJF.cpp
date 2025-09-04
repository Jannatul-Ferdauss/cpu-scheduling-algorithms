#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    bool done = false;
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> processes[i].burst_time;
        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> processes[i].arrival_time;
    }

    int current_time = 0, completed = 0;
    float total_wt = 0, total_tat = 0;

    cout << "\nGantt Chart\n";

    while (completed < n) {
        int idx = -1;
        int min_burst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!processes[i].done && processes[i].arrival_time <= current_time) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    idx = i;
                }
                else if (processes[i].burst_time == min_burst) {
                    // Tie-breaking: earlier arrival
                    if (processes[i].arrival_time < processes[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx == -1) {
            current_time++;
        } else {
            processes[idx].start_time = current_time;
            processes[idx].completion_time = current_time + processes[idx].burst_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;

            current_time = processes[idx].completion_time;
            processes[idx].done = true;
            completed++;

            cout << processes[idx].start_time << " P" << processes[idx].id << " ";
        }
    }

    cout << current_time << endl << endl;

    for (const auto& p : processes) {
        cout << "Process: P" << p.id
             << " Start time: " << p.start_time
             << " Waiting time: " << p.waiting_time
             << " Turnaround time: " << p.turnaround_time << endl;
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
    }

    cout << "\nAverage waiting time: " << total_wt / n << endl;
    cout << "Average turnaround time: " << total_tat / n << endl;

    return 0;
}

