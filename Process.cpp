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
ULL Process::default_max_burst_time = 1000;
ULL Process::default_time_quantum = 100;

Process::Process(ULL max_arrival_time, ULL max_burst_time) {
    start_time = completion_time = waiting_time = 0;
    counter += 1;
    pid = counter;
    std::random_device rd;
    std::default_random_engine generator(static_cast<unsigned>(rd()));
    std::uniform_int_distribution<ULL> arrival_dist(0, max_arrival_time);
    std::uniform_int_distribution<ULL> burst_dist(0, max_burst_time);

    arriving_time = arrival_dist(generator);
    burst_time = burst_dist(generator);
    remaining_burst_time = burst_time;
    turnaround_time = 0;

    // std::cout << "Procesul " << pid << " creat!\n";
}

void Process::FCFS(std::vector<Process>& processes) {
    ULL avg_waiting_time = 0;
    ULL avg_turnaround_time = 0;

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

        avg_waiting_time += current.waiting_time;
        avg_turnaround_time += current.turnaround_time;

        completedProcesses.push_back(current);

        fout << "Procesul " << current.pid << " Start time: " << current.start_time << ", Completion  time: " << current.completion_time << ", TAT: " << current.turnaround_time << ", TW: " << current.waiting_time << '\n';
    }

    processes = completedProcesses;

    avg_waiting_time /= processes.size();
    avg_turnaround_time /= processes.size();

    std::cout << "Average waiting time: " << avg_waiting_time << '\n';
    std::cout << "Average turnaround time: " << avg_turnaround_time << '\n';
}

void Process::logData() const {
    std::cout << "Process ID: " << pid << ", Arriving Time: " << arriving_time << ", Burst Time: " << burst_time << '\n';
    std::cout << '\t' << "Start time: " << start_time << ", Completion Time: " << completion_time << ", Waiting Time: " << waiting_time << '\n';
    std::cout << '\n';
}

void Process::displayAllData(std::vector<Process>& Processes) {
    for (const auto& process : Processes) {
        process.logData();
    }
}

void Process::RoundRobin(std::vector<Process> &processes, ULL time_quantum) {
    ULL avg_waiting_time = 0;
    ULL avg_turnaround_time = 0;

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
        // std::cout << currentTime << '\n';
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
                    currentProcess.turnaround_time = currentProcess.completion_time - currentProcess.arriving_time;
                    currentProcess.waiting_time = currentProcess.turnaround_time - currentProcess.burst_time;
                    completedProcesses.push_back(currentProcess);
                    avg_waiting_time += currentProcess.waiting_time;
                    avg_turnaround_time += currentProcess.turnaround_time;
                }
                else {
                    pq.push(currentProcess);
                }
                fout << currentProcess.pid << ' ' << currentProcess.start_time << ' ' << execution_time << '\n';

            }
        }
        else if (!pq.empty()) {
            currentTime = pq.top().arriving_time;
        }
    }
    processes = completedProcesses;

    avg_waiting_time /= processes.size();
    avg_turnaround_time /= processes.size();

    std::cout << "Average waiting time: " << avg_waiting_time << '\n';
    std::cout << "Average turnaround time: " << avg_turnaround_time << '\n';
}
void Process::ShortestRemainingTimeFirst(std::vector<Process>& processes) {
    ULL avg_waiting_time = 0;
    ULL avg_turnaround_time = 0;

    // Priority queue sorted by arrival time
    auto arrivalCompare = [](const Process& a, const Process& b) {
        return a.arriving_time > b.arriving_time;
    };
    std::priority_queue<Process, std::vector<Process>, decltype(arrivalCompare)> arrivalQueue(arrivalCompare);

    for (const auto& process : processes) {
        arrivalQueue.push(process);
    }

    // Ready queue sorted by remaining burst time
    auto burstCompare = [](const Process& a, const Process& b) {
        return a.remaining_burst_time > b.remaining_burst_time;
    };
    std::priority_queue<Process, std::vector<Process>, decltype(burstCompare)> readyQueue(burstCompare);

    ULL currentTime = 0;
    std::vector<Process> completedProcesses;

    while (!arrivalQueue.empty() || !readyQueue.empty()) {
        // Add all processes arriving before or at the current time to the ready queue
        while (!arrivalQueue.empty() && arrivalQueue.top().arriving_time <= currentTime) {
            readyQueue.push(arrivalQueue.top());
            // std::cout << "Process " << arrivalQueue.top().pid
                      // << " added to ready queue at time " << currentTime << '\n';
            arrivalQueue.pop();
        }

        if (readyQueue.empty()) {
            // If no process is ready, advance time to the next arrival
            currentTime = arrivalQueue.top().arriving_time;
            continue;
        }

        // Get the process with the shortest remaining time
        Process currentProcess = readyQueue.top();
        readyQueue.pop();

        // Check for potential preemption
        ULL runtime = currentProcess.remaining_burst_time;
        Process* preemptingProcess = nullptr;

        // Collect potential preempting processes
        std::vector<Process> delayedProcesses;
        while (!arrivalQueue.empty() && arrivalQueue.top().arriving_time < currentTime + runtime) {
            Process potentialEnemy = arrivalQueue.top();
            arrivalQueue.pop();

            if (potentialEnemy.arriving_time + potentialEnemy.remaining_burst_time <
                currentTime + runtime) {
                // Found a preempting process
                runtime = potentialEnemy.arriving_time - currentTime;
                preemptingProcess = &potentialEnemy;
                break;
            } else {
                delayedProcesses.push_back(potentialEnemy);
            }
        }

        // Push back delayed processes
        for (auto& delayedProcess : delayedProcesses) {
            arrivalQueue.push(delayedProcess);
        }

        if (preemptingProcess) {
            // Preemption occurs
            currentProcess.remaining_burst_time -= runtime;
            currentTime += runtime;
            fout << currentProcess.pid << ' ' << currentTime - runtime <<  " " << runtime << '\n';


            if (currentProcess.remaining_burst_time > 0) {
                readyQueue.push(currentProcess);
            } else {
                // Process completes during the runtime
                currentProcess.completion_time = currentTime;
                currentProcess.turnaround_time = currentTime - currentProcess.arriving_time;
                currentProcess.waiting_time = currentProcess.turnaround_time - currentProcess.burst_time;
                avg_turnaround_time += currentProcess.turnaround_time;
                avg_waiting_time += currentProcess.waiting_time;

                fout << currentProcess.pid << ' ' << currentTime - runtime <<  " " << runtime << '\n';
                completedProcesses.push_back(currentProcess);
            }

            // Add the preempting process to the ready queue
            readyQueue.push(*preemptingProcess);
            // std::cout << "Process " << currentProcess.pid
                      // << " preempted by process " << preemptingProcess->pid
                      // << " at time " << currentTime << '\n';
        } else {
            // No preemption; process completes execution
            currentTime += runtime;
            currentProcess.remaining_burst_time = 0;
            currentProcess.completion_time = currentTime;
            currentProcess.turnaround_time = currentTime - currentProcess.arriving_time;
            currentProcess.waiting_time = currentProcess.turnaround_time - currentProcess.burst_time;
            completedProcesses.push_back(currentProcess);
            avg_waiting_time += currentProcess.waiting_time;
            avg_turnaround_time += currentProcess.turnaround_time;

            fout << currentProcess.pid << ' ' << currentTime - runtime <<  " " << runtime << '\n';


            // std::cout << "Process " << currentProcess.pid
                      // << " completed at time " << currentTime << '\n';
        }
    }

    // Ensure all processes are properly sorted back into the original vector
    processes = completedProcesses;

    avg_waiting_time /= processes.size();
    avg_turnaround_time /= processes.size();

    std::cout << "Average waiting time: " << avg_waiting_time << '\n';
    std::cout << "Average turnaround time: " << avg_turnaround_time << '\n';
}