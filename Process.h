//
// Created by Cosmin Glod on 04.12.2024.
//

#ifndef PROCESS_H
#define PROCESS_H
#define ULL unsigned long long
#include <vector>
#include <random>

// time is measured in nanoseconds 10^-9
// te sun pe clion
class Process {
    static ULL counter;
    static ULL max_arrival_time;
    static ULL max_burst_time;
    ULL pid;
    ULL arriving_time; //time when the process arrived in queue
    ULL start_time; // time when the process loaded on the processor (for the first time)
    ULL completion_time; // time when the procees completes the task
    ULL turnaround_time; // the time between the completion and the arriving time
    ULL burst_time; // time on the CPU
    ULL waiting_time; // time waiting in queue
    int priority; // the priority :))
    ULL estimated_burst_time;
public:
    Process();
    Process(int pid_, ULL arriving_time_) : pid(pid_), arriving_time(arriving_time_),  turnaround_time(0), burst_time(0), waiting_time(0), priority(0) {}
    
    static void FCFS(std::vector<Process>& processes);

    static std::vector<Process> generateProcesses(int num_processes, int max_arrival_time, int max_burst_time) {
        std::vector<Process> processes;
        std::default_random_engine generator(static_cast<unsigned>(time(0))); // Seed with current time
        std::uniform_int_distribution<int> arrival_dist(0, max_arrival_time);
        std::uniform_int_distribution<int> burst_dist(1, max_burst_time); // Burst time can't be 0

        for (int i = 1; i <= num_processes; ++i) {
            ULL arrival_time = arrival_dist(generator);
            ULL burst_time = burst_dist(generator);
            processes.emplace_back();
        }

        return processes;
    }

};





#endif //PROCESS_H
