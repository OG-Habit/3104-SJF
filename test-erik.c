#include "sjf.h"

int main() {
    int x;
    int numOfProcesses=3;
    Process *arr;

    arr = (Process*)calloc(numOfProcesses, sizeof(Process));
    for(x = 0; x < numOfProcesses; x++) {
        arr[x] = initProcess(x);
    }

    printPropsOfAllProcesses(arr, numOfProcesses);

    return 0;
}