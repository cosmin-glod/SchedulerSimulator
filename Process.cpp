//
// Created by Cosmin Glod on 04.12.2024.
//

#include "Process.h"

#include <iostream>
#include <ostream>

ULL Process::counter = 0;
ULL Process::max_arrival_time = 1000000000;
ULL Process::max_burst_time = 1000000000;

Process::Process() {
    start_time = completion_time = turnaround_time = waiting_time = estimated_burst_time = 0;
    counter += 1;
    pid = counter;
    std::default_random_engine generator(static_cast<unsigned>(time(0)));
    std::uniform_int_distribution<ULL> arrival_dist(0, max_arrival_time);
    std::uniform_int_distribution<ULL> burst_dist(0, max_burst_time);
    arriving_time = arrival_dist(generator);
    std::uniform_int_distribution<int> priority_dist(0, 2);
    burst_time = burst_dist(generator);
    priority = priority_dist(generator);
}