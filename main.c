/**
 * @file main.c
 * @authors - Team 1
 *  Celdran, Erik
 *  Dayata, Wayne
 *  Labana, Hannah
 *  Valeros, Mark
 *  Woogue, Ivan 
 * @brief Course Code - CS3104. This preemptive Shortest Job First is a final project requirement for CS 3104 - Operating Systems. This program accepts upto 8 processes only. The burst time and arrival time of each process is provided by user. The average waiting time and average turnaroud time given the list of processes will be printed out. A gantt chart showing the sequence of processes being executed will be printed as well. No decimals will be permitted for any of the inputs nor outputs.
 * @version 0.1
 * @date 2022-12-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "sjf.h"

int main() {
    int x, numOfProcesses;
    double minAvgWaitingTime,minAvgTurnTime;
    Process *arrProcess;
    Runtime rt;

    printGreeting();

    numOfProcesses = getNumOfProcesses("\nEnter the number of processes: ");

    arrProcess = (Process*)calloc(numOfProcesses, sizeof(Process));

    for(x = 0; x < numOfProcesses; x++) {
        arrProcess[x] = initProcess(x);
    }

    rt = simulateScheduling(arrProcess, numOfProcesses);

    minAvgWaitingTime = getMinAvgWaitingTime(arrProcess, numOfProcesses);
    minAvgTurnTime = getMinAvgTurnaroundTime(arrProcess, numOfProcesses);

    printPropsOfAllProcesses(arrProcess, numOfProcesses);

    printGanttChart(rt);

    printf("\nAVG WAITING TIME of %d processes is %.3lf ms\n", numOfProcesses, minAvgWaitingTime);
    printf("AVG TURNAROUND TIME of %d processes is %.3lf ms\n", numOfProcesses, minAvgTurnTime);
    
    return 0;
}