//
// Created by Cosmin Glod on 04.12.2024.
//

#include "Process.h"
#include <iostream>
#include <ostream>
#include <random>
#include <queue>
#include <algorithm>

ULL Process::counter = 0;
ULL Process::default_max_arrival_time = 100;
ULL Process::default_max_burst_time = 100;

Process::Process(ULL max_arrival_time, ULL max_burst_time) {
    start_time = completion_time = waiting_time = estimated_burst_time = 0;
    counter += 1;
    pid = counter;
    std::random_device rd;
    std::default_random_engine generator(static_cast<unsigned>(rd()));
    std::uniform_int_distribution<ULL> arrival_dist(0, max_arrival_time);
    std::uniform_int_distribution<ULL> burst_dist(0, max_burst_time);
    std::uniform_int_distribution<int> priority_dist(0, 2);

    arriving_time = arrival_dist(generator);
    burst_time = burst_dist(generator);
    priority = priority_dist(generator);
    remaining_burst_time = burst_time;
    turnaround_time = -arriving_time;

    std::cout << "procesul " << pid << " creat!\n";
}

void Process::FCFS(std::vector<Process>& processes) {

    auto compare = [](const Process& a, const Process& b) {
        return a.arriving_time > b.arriving_time; // Min-heap based on arriving_time
    };
    std::priority_queue<Process, std::vector<Process>, decltype(compare)> pq(compare);

    for (const auto& process : processes) {
        pq.push(process);
    }

    std::vector<Process> completedProcesses;

    ULL currentTime = 0;
    while (!pq.empty()) {
        Process current = pq.top();
        pq.pop();

        if (currentTime < current.arriving_time) {
            currentTime = current.arriving_time;
        }

        current.start_time = currentTime;
        current.completion_time = currentTime + current.burst_time;
        current.turnaround_time = current.completion_time - current.arriving_time;
        current.waiting_time = current.start_time - current.arriving_time;
        currentTime += current.burst_time;

        completedProcesses.push_back(current);
        std::cout << "Procesul " << current.pid << " Start time: " << current.start_time << ",completion  time: " << current.completion_time << ", TAT: " << current.turnaround_time << ", TW: " << current.waiting_time << '\n';

    }

    processes = completedProcesses;
}

void Process::logData() const {
    std::cout << "Process ID: " << pid << ", Arriving Time: " << arriving_time << ", Burst Time: " << burst_time << '\n';
    std::cout << '\t' << "Start time: " << start_time << ", Completion Time: " << completion_time << ", Waiting Time:" << waiting_time << '\n';
    std::cout << '\n';
}

void Process::displayAllData(std::vector<Process>& Processes) {
    for (const auto& process : Processes) {
        process.logData();
    }

}

void Process::RoundRobin(std::vector<Process> &processes, ULL time_quantum) {
    std::queue<Process> readyQueue;
    std::vector<Process> completedProcesses;

    // priority queue for the processes
    auto compare = [](const Process& a, const Process& b) {
        return a.arriving_time > b.arriving_time;
    };
    std::priority_queue<Process, std::vector<Process>, decltype(compare)> pq(compare);

    for (const auto& process : processes) {
        pq.push(process);
    }

    ULL currentTime = pq.top().arriving_time;
    bool busy = false;
    // filling the queue with processes that arrive before the currentTime
    while (!pq.empty()) {
        std::cout << currentTime << '\n';
        busy = false;
        while (!pq.empty() && pq.top().arriving_time <= currentTime) {
            readyQueue.push(pq.top());
            busy = true;
            pq.pop();
        }
        if (busy) {
            while (!readyQueue.empty()) {
                Process currentProcess = readyQueue.front();
                readyQueue.pop();

                currentProcess.start_time = currentTime;
                ULL execution_time = std::min(time_quantum, currentProcess.remaining_burst_time);
                currentProcess.remaining_burst_time -= execution_time;
                currentTime += execution_time;

                // check for newly arrived processes
                while (!pq.empty() && pq.top().arriving_time <= currentTime) {
                    readyQueue.push(pq.top());
                    pq.pop();
                }

                // the process is done executing
                if (currentProcess.remaining_burst_time == 0) {
                    currentProcess.completion_time = currentTime;
                    currentProcess.turnaround_time += currentProcess.completion_time;
                    currentProcess.waiting_time = currentProcess.turnaround_time - currentProcess.burst_time;
                    completedProcesses.push_back(currentProcess);
                    std::cout << "Procesul: " << currentProcess.pid << " a terminat.\n";
                }
                else {
                    currentProcess.arriving_time = currentTime;
                    pq.push(currentProcess);
                }
            }
        }
        else if (!pq.empty()) {
            currentTime = pq.top().arriving_time;
        }
    }
    processes = completedProcesses;
}