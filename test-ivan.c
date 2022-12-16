#include "sjf.h"

int main() {
    int x;
    int count = 4;
    Runtime rt;
    RuntimeProcessLL trav;

    Process arr[] = {
        {"P1", 2, 2, 0, 0, 2, 2},
        {"P2", 6, 2, 0, 0, 2, 6},
        {"P3", 3, 4, 0, 0, 4, 3},
        {"P4", 0, 3, 0, 0, 3, 0}
    };

    rt = simulateScheduling(arr, count);

    // for(x = 0; x < count; x++) {
    //     printf("%s - %d, %d, %d, %d", arr[x].pid, arr[x].timeArrival, arr[x].timeBurst, arr[x].timeTurnaround, arr[x].timeWaiting);
    //     printf("\n");
    // }

    printf("\tduration \n");
    for(trav = rt.front; trav != NULL; trav = trav->link) {
        printf("%s \t%d \n", trav->pid, trav->duration);
    }
    
    return 0;
}