#include "sjf.h"

int main() {
    int x, numOfProcesses, minAvgWaitingTime;
    Process *arrProcess;
    Runtime rt;

    printGreeting();

    numOfProcesses = getNumOfProcesses("Enter the number of processes: ");

    arrProcess = (Process*)calloc(numOfProcesses, sizeof(Process));

    for(x = 0; x < numOfProcesses; x++) {
        arrProcess[x] = initProcess(x);
    }

    rt = simulateScheduling(arrProcess, numOfProcesses);

    minAvgWaitingTime = getMinAvgWaitingTime(arrProcess);

    printPropsOfAllProcesses(arrProcess);

    printGanttChart(rt);

    printf("The minimum average waiting item of preemptive SJF CPU Scheduling of %d processes is %d.\n", numOfProcesses, minAvgWaitingTime);
    
    return 0;
}