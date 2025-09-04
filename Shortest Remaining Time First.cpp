#include <bits/stdc++.h>
using namespace std;

struct Process {
    int id;               // process id
    int arrival_time;     // arrival time
    int burst_time;       // burst time
    int remaining_time;   // remaining burst time
    int start_time;       // first start time
    int completion_time;  // completion time
    int turnaround_time;  // TAT = CT - AT
    int waiting_time;     // WT = TAT - BT
    int response_time;    // RT = ST - AT
    bool started;         // whether the process has started
    bool is_ready;        // whether the process is in the ready queue

    Process(int pid, int at, int bt) {
        id = pid;
        arrival_time = at;
        burst_time = bt;
        remaining_time = bt;
        started = false;
        is_ready = false;
        start_time = -1;
        completion_time = -1;
        turnaround_time = 0;
        waiting_time = 0;
        response_time = 0;
    }
};

// Preemptive SJF Scheduling
void preemptive_sjf(vector<Process> &processes, bool detail) {
    int n = processes.size();
    int current_time = 0;
    int completed = 0;

    // min heap {remaining_time, arrival_time, index}
    priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<tuple<int,int,int>>> ready_queue;

    if(detail) cout << "Gantt Chart:\n" << "0 ";

    int last_pid = -1; // track last process printed for compact Gantt chart

    while(completed != n) {
        int least_arrival_time = INT_MAX;

        // Add new arrivals
        for(int i=0; i<n; i++) {
            if(!processes[i].is_ready && processes[i].arrival_time <= current_time) {
                ready_queue.push({processes[i].remaining_time, processes[i].arrival_time, i});
                processes[i].is_ready = true;
            }
            if(!processes[i].is_ready)
                least_arrival_time = min(least_arrival_time, processes[i].arrival_time);
        }

        if(!ready_queue.empty()) {
            int idx = get<2>(ready_queue.top());
            ready_queue.pop();

            // First time execution → response time
            if(!processes[idx].started) {
                processes[idx].start_time = current_time;
                processes[idx].started = true;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
            }

            // Run for 1 unit
            processes[idx].remaining_time--;
            current_time++;

            // Completion check
            if(processes[idx].remaining_time == 0) {
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                completed++;
            } else {
                ready_queue.push({processes[idx].remaining_time, processes[idx].arrival_time, idx});
            }

            // Gantt chart printing
            if(detail) {
                if(last_pid != processes[idx].id) {
                    cout << "P" << processes[idx].id << " ";
                    last_pid = processes[idx].id;
                }
                cout << current_time << " ";
            }
        } else {
            // CPU is idle → jump to next arrival
            current_time = least_arrival_time;
            if(detail) {
                cout << "Idle " << current_time << " ";
                last_pid = -1;
            }
        }
    }

    if(detail) cout << "\n\n";

    // Calculate averages
    double waiting_sum=0, turnaround_sum=0, response_sum=0;
    for(int i=0; i<n; i++) {
        waiting_sum += processes[i].waiting_time;
        turnaround_sum += processes[i].turnaround_time;
        response_sum += processes[i].response_time;

        if(detail) {
            cout << "Process P" << processes[i].id
                 << " | Arrival: " << processes[i].arrival_time
                 << " | Burst: " << processes[i].burst_time
                 << " | Start: " << processes[i].start_time
                 << " | Completion: " << processes[i].completion_time
                 << " | Response: " << processes[i].response_time
                 << " | Waiting: " << processes[i].waiting_time
                 << " | Turnaround: " << processes[i].turnaround_time
                 << "\n";
        }
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Response Time: " << response_sum / n << "\n";
    cout << "Average Waiting Time: " << waiting_sum / n << "\n";
    cout << "Average Turnaround Time: " << turnaround_sum / n << "\n";
}

// Main function for testing
int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    for(int i=0; i<n; i++) {
        int at, bt;
        cout << "Enter arrival time and burst time for P" << i+1 << ": ";
        cin >> at >> bt;
        processes.push_back(Process(i+1, at, bt));
    }

    cout << "\n--- Preemptive SJF Scheduling ---\n";
    preemptive_sjf(processes, true);

    return 0;
}
