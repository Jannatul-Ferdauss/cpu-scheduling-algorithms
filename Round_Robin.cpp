#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time = -1;
    int end_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    bool is_completed = false;
};

bool arrival_compare(Process a, Process b) {
    return a.arrival_time < b.arrival_time;
}

int main() {
    int n;
    cout << "Number of processes, n: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> processes[i].burst_time;
        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> processes[i].arrival_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    int time_quantum;
    cout << "Time Quantum: ";
    cin >> time_quantum;

    sort(processes.begin(), processes.end(), arrival_compare);

    queue<int> ready_queue;
    int current_time = 0, completed = 0;
    float total_response_time = 0, total_waiting_time = 0, total_turnaround_time = 0;
    vector<pair<int, int>> gantt_chart; // (time, process id)

    vector<bool> visited(n, false);
    ready_queue.push(0);
    visited[0] = true;

    cout << "\nGantt Chart:\n";

    while (completed != n) {
        if (!ready_queue.empty()) {
            int idx = ready_queue.front();
            ready_queue.pop();

            if (processes[idx].start_time == -1) {
                processes[idx].start_time = current_time;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
                total_response_time += processes[idx].response_time;
            }

            gantt_chart.push_back({current_time, processes[idx].id});

            if (processes[idx].remaining_time <= time_quantum) {
                current_time += processes[idx].remaining_time;
                processes[idx].remaining_time = 0;
                processes[idx].end_time = current_time;
                processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                total_waiting_time += processes[idx].waiting_time;
                total_turnaround_time += processes[idx].turnaround_time;
                processes[idx].is_completed = true;
                completed++;
            } else {
                current_time += time_quantum;
                processes[idx].remaining_time -= time_quantum;
            }

            // Add newly arrived processes to the ready queue
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= current_time && !visited[i] && !processes[i].is_completed) {
                    ready_queue.push(i);
                    visited[i] = true;
                }
            }
            // If the process is not yet complete, push it back to the queue
            if (!processes[idx].is_completed) {
                ready_queue.push(idx);
            }
        } else {
            // If no process is available to execute
            current_time++;
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= current_time && !visited[i] && !processes[i].is_completed) {
                    ready_queue.push(i);
                    visited[i] = true;
                }
            }
        }
    }

    // Print Gantt Chart
    for (auto it : gantt_chart) {
        cout << it.first << " P" << it.second << " ";
    }
    cout << current_time << endl;

    // Print process details
    for (auto p : processes) {
        cout << "Process: P" << p.id
             << " Start time: " << p.start_time
             << " End time: " << p.end_time
             << " Response Time: " << p.response_time
             << " Waiting time: " << p.waiting_time
             << " Turnaround time: " << p.turnaround_time
             << endl;
    }

    cout << "Average Response Time: " << (total_response_time / n) << endl;
    cout << "Average Waiting Time: " << (total_waiting_time / n) << endl;
    cout << "Average Turnaround Time: " << (total_turnaround_time / n) << endl;

    return 0;
}
