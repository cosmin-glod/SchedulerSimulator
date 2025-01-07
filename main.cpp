#include <iostream>
#include <vector>
#include "Process.h"

int main() {
    // std::cout << "hai sa facem proiectul la SO!\n";

    std::vector<Process> procese = Process::generateProcesses(50);
    Process::displayAllData(procese);

    Process::RoundRobin(procese, 500);
    // fout << "Average waiting time: " << avg_waiting_time << '\n';
    // fout << "Average turnaround time: " << avg_turnaround_time << '\n';
    // std::cout << "gata schedulingul\n";
    Process::displayAllData(procese);
    return 0;
}