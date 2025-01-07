//
// Created by Cosmin Glod on 04.12.2024.
//

#include "Process.h"

#include <iostream>
#include <ostream>

ULL Process::counter = 0;

Process::Process() {
    pid = ++counter;
    // arriving_time = random ceva
    // burst_time = ceva random
    // priority = ceva random;
}

