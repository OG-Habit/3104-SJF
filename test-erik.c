#include "sjf.h"

int main() {
    int x;
    int numOfProcesses=3;
    Process *arr;
    Runtime rt;
    RuntimeProcessLL trav;

    arr = (Process*)calloc(numOfProcesses, sizeof(Process));
    for(x = 0; x < numOfProcesses; x++) {
        arr[x] = initProcess(x);
    }
    rt = simulateScheduling(arr, numOfProcesses);

    printPropsOfAllProcesses(arr, numOfProcesses);

    printf("\n\tDuration \n");
    for(trav = rt.front; trav != NULL; trav = trav->link) {
        printf("%s \t%d \n", trav->pid, trav->duration);
    }

    return 0;
}