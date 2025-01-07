//
// Created by Cosmin Glod on 04.12.2024.
//

#ifndef PROCESS_H
#define PROCESS_H
#define ULL unsigned long long

// time is measured in nanoseconds 10^-9 s
class Process {
    int pid;
    ULL arriving_time;
    ULL start_time;
    ULL completion_time;
    ULL turnaround_time;
    ULL burst_time;
    ULL waiting_time;
    int priority;
    ULL estimated_burst_time;
public:
    Process(int pid_, ULL arriving_time_) : pid(pid_), arriving_time(arriving_time_) {}


    static void FCFS();
};



#endif //PROCESS_H
