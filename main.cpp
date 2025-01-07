#include <iostream>
#include <vector>
#include "Process.h"

int main() {
    std::cout << "hai sa facem proiectul la SO!\n";

    std::vector<Process> procese = Process::generateProcesses(10);
    Process::displayAllData(procese);

    Process::RoundRobin(procese, 1000);
    Process::displayAllData(procese);
    return 0;
}