#include "sjf.h"

int main() {
    int x, numOfProcesses;
    double minAvgWaitingTime;
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

    printPropsOfAllProcesses(arrProcess, numOfProcesses);

    printGanttChart(rt);

    printf("\nThe minimum average waiting item of preemptive SJF CPU Scheduling of %d processes is %.3lf second/s\n", numOfProcesses, minAvgWaitingTime);
    
    return 0;
}