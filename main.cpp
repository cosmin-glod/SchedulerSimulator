#include <iostream>
#include <vector>
#include "Process.h"

int main() {
    std::cout << "hai sa facem proiectul la SO!\n";

    std::vector<Process> procese = Process::generateProcesses(10);
    Process::FCFS(procese);
    //
    Process::displayAllData(procese);
    return 0;
}