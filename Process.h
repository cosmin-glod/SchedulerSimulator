//
// Created by Cosmin Glod on 04.12.2024.
//

#ifndef PROCESS_H
#define PROCESS_H
#define ULL unsigned long long
#include <vector>
#include <fstream>

inline std::ofstream fout("output.txt");

// time is measured in nanoseconds 10^-9
// te sun pe clion
class Process {
    static ULL counter;
    static ULL default_max_arrival_time;
    static ULL default_max_burst_time;
    static ULL default_time_quantum;
    ULL pid;
    ULL arriving_time; //time when the process arrived in queue
    ULL start_time; // time when the process loaded on the processor (for the first time)
    ULL completion_time; // time when the procees completes the task
    ULL turnaround_time; // the time between the completion and the arriving time
    ULL burst_time; // time on the CPU
    ULL waiting_time; // time waiting in queue
    ULL remaining_burst_time; // the remaining time the process has to complete the task

protected:
    void logData() const;
public:
    Process(ULL max_arrival_time = default_max_arrival_time, ULL max_burst_time = default_max_burst_time);

    static void displayAllData(std::vector<Process>& processes);

    static std::vector<Process> generateProcesses(int num_processes, ULL max_arrival_time = default_max_arrival_time, ULL max_burst_time = default_max_burst_time) {
        std::vector<Process> processes;
        for (int i = 1; i <= num_processes; ++i) {
            processes.emplace_back(max_arrival_time, max_burst_time);
        }
        return processes;
    }

    static void FCFS(std::vector<Process>& processes);

    // generally time quantum is between 10-100 milliseconds = 10000000 - 100000000 nanoseconds
    static void RoundRobin(std::vector<Process>& processes, ULL time_quantum = default_time_quantum);

    static void ShortestRemainingTimeFirst(std::vector<Process>& processes);
};


#endif //PROCESS_H
