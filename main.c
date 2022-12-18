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

    printf("\nAVG WAITING TIME of %d processes is %.3lf second/s\n", numOfProcesses, minAvgWaitingTime);
    printf("AVG TURNAROUND TIME of %d processes is %.3lf second/s\n", numOfProcesses, minAvgTurnTime);
    
    return 0;
}