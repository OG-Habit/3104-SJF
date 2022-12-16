#include "sjf.h"

int main() {
    int x;
    int count = 3;
    Runtime rt;
    RuntimeProcessLL trav;

    // Process arr[] = {
    //     {"P1", 2000, 2000, 0, 0, 2000, 2000},
    //     {"P2", 6000, 2000, 0, 0, 2000, 6000},
    //     {"P3", 3000, 4000, 0, 0, 4000, 3000},
    //     {"P4", 0, 3000, 0, 0, 3000, 0}
    // };

    Process arr[] = {
        {"P1", 0, 8000, 0, 0, 8000, 0},
        {"P2", 400, 4000, 0, 0, 4000, 400},
        {"P3", 1000, 1000, 0, 0, 1000, 1000}
    };

    rt = simulateScheduling(arr, count);

    for(x = 0; x < count; x++) {
        printf("%s - %d, %d, %d, %d", arr[x].pid, arr[x].timeArrival, arr[x].timeBurst, arr[x].timeTurnaround, arr[x].timeWaiting);
        printf("\n");
    }

    printf("\n\tduration \n");
    for(trav = rt.front; trav != NULL; trav = trav->link) {
        printf("%s \t%d \n", trav->pid, trav->duration);
    }
    
    return 0;
}