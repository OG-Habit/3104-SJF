#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataStruct.h"

int getNumOfProcesses(char *prompt);
Process initProcess(int pid);
Runtime simulateScheduling(Process *arr, int count);
Bitmap initBitmap(int count);
int isReady(Bitmap ready, int ndx);
int isUnfinished(Bitmap unfinished, int ndx);
int isCurrentProcess(Runtime rt, char *pid);
Bitmap setBit(Bitmap ready, int ndx);
Bitmap clearBit(Bitmap ready, int ndx);
void enqueueReadyQueue(ProcessLL *head, Process *process, int ndx);
ProcessLL dequeueReadyQueue(ProcessLL *queue);
void enqueueRuntime(Runtime *rt, Process p);


int getMinAvgWaitingTime(Process *arr, int);

void printGreeting();
void printPropsOfAllProcesses(Process *arr, int);
void printGanttChart(Runtime rt);


/**
 * @brief Get the number of processes
 * 
 * @param prompt - question
 * @return int - total number of processes to be simulated
 */
int getNumOfProcesses(char *prompt) {
    prompt = prompt;
    return 0;
}

/**
 * @brief Initialize the arrival time, remaining time, CPU burst time, pid
 *        For pid => P1, P2, P3, ..., Pn
 *        Burst time == remaining time
 *        Arrival time == decreasing arrival time
 * 
 * @param pid - process identifier
 * @return Process 
 */
Process initProcess(int pid) {
    Process p = {0};
    pid = pid;
    return p;
}

/**
 * @brief Simulate scheduling of a given array of processes of n size
 *        Solve turnaround time
 *        Solve waiting time
 * 
 * @param arr - array of processes
 * @param count - size of the array
 * @return Runtime - sequence of the SJF scheduling of the given array of processes
 */
Runtime simulateScheduling(Process *arr, int count) {
    Runtime rt = {0};
    ProcessLL queue, currProcess, trav;
    Bitmap unfinished, ready;
    int x;

    unfinished = initBitmap(count);
    ready = 0;
    queue = currProcess = NULL;

    // each iteration is 1 second
    while(unfinished != 0) {
        // fill up ready queue
        for(x = 0; x < count; x++) {
            if(!isCurrentProcess(rt, arr[x].pid) &&
                !isReady(ready, x) && 
                isUnfinished(unfinished, x) &&
                arr[x]._timeArrivalDec == 0) {
                ready = setBit(ready, x);
                enqueueReadyQueue(&queue, &arr[x], x);
            }
        }

        // get first process in ready queue if nothing else
        if(currProcess == NULL) {
            currProcess = dequeueReadyQueue(&queue);

            // ready queue may be empty as proccesses have yet to arrive
            // ensures current process is not yet added to runtime
            // ensures no duplicates of processes in runtime
            if(currProcess != NULL) {
                enqueueRuntime(&rt, *currProcess->processPtr);
                ready = clearBit(ready, currProcess->ndx);
            }
        } else {
            // check if the shortest job in queue is smaller than current
            if(queue != NULL &&
                queue->processPtr->_timeRem < currProcess->processPtr->_timeRem) {
                // do preemption
                // return current process to ready queue
                enqueueReadyQueue(&queue, currProcess->processPtr, currProcess->ndx);
                ready = setBit(ready, currProcess->ndx);

                // preempt current process with a process with a shorter burst time
                currProcess = dequeueReadyQueue(&queue);
                ready = clearBit(ready, currProcess->ndx);
                enqueueRuntime(&rt, *currProcess->processPtr);
            }
        }

        // working current process
        if(currProcess != NULL) {
            currProcess->processPtr->_timeRem--;
            rt.rear->duration++;
            // set flag to 0 for process when finishes running
            if(currProcess->processPtr->_timeRem == 0) {
                unfinished = clearBit(unfinished, currProcess->ndx);
                currProcess = NULL;
            }
        }

        // increase waiting time for processes in ready queue
        for(trav = queue; trav != NULL; trav = trav->link) {
            trav->processPtr->timeWaiting++;
        }

        // decrease arrival time
        for(x = 0; x < count; x++) {
            if(arr[x]._timeArrivalDec > 0) {
                arr[x]._timeArrivalDec--;
            }
        }
    }
    
    // calculate turnaround time for each process
    for(x = 0; x < count; x++) {
        arr[x].timeTurnaround = arr[x].timeBurst + arr[x].timeWaiting;
    }

    return rt;
}

Bitmap initBitmap(int count) {
    int x, one = 1;
    Bitmap bitmap = 0;
    for(x = 0; x < count; x++) {
        bitmap |= one;
        one <<= 1;
    }
    return bitmap;
}

int isReady(Bitmap ready, int ndx) {
    int x = 1 << ndx;
    return (ready | x) == ready ? 1 : 0; 
}

int isUnfinished(Bitmap unfinished, int ndx) {
    int x = 1 << ndx;
    return (unfinished | x) == unfinished ? 1 : 0; 
}

int isCurrentProcess(Runtime rt, char *pid) {
    if(rt.rear != NULL) {
        return strcmp(pid, rt.rear->pid) == 0 ? 1 : 0;
    } else {
        return 0;
    }
}

Bitmap setBit(Bitmap bitmap, int ndx) {
    return bitmap | 1 << ndx;
}

Bitmap clearBit(Bitmap bitmap, int ndx) {
    return bitmap ^ 1 << ndx;
}

void enqueueReadyQueue(ProcessLL *head, Process *process, int ndx) {
    ProcessLL *trav, newLL;

    newLL = (ProcessLL)calloc(1, sizeof(struct processLL));
    newLL->ndx = ndx;
    newLL->processPtr = process;
    for(trav = head; *trav != NULL; trav = &(*trav)->link) {
        if(process->timeArrival > (*trav)->processPtr->timeArrival) {
            break;
        }
    } 
    if(*trav == NULL) {
        *trav = newLL;
    } else {
        (*trav)->link = newLL;
    }
}

ProcessLL dequeueReadyQueue(ProcessLL *queue) {
    ProcessLL ll = *queue;
    if(*queue != NULL) {
        *queue = (*queue)->link;
    }
    return ll;
}

void enqueueRuntime(Runtime *rt, Process p) {
    RuntimeProcessLL rp = (RuntimeProcessLL)calloc(1, sizeof(struct runtimeProcessLL));
    
    rp->pid = (char*)calloc(4,sizeof(char));
    strcpy(rp->pid, p.pid);
    if(rt->front == NULL) {
        rt->front = rt->rear = rp;
    } else {
        rt->rear->link = rp;
        rt->rear = rp;
    }
}


/**
 * @brief Get the min avg waiting time for the processes
 * 
 * @param arr - array of processes
 * @param count - length of array
 * @return int - min avg waiting time
 */
int getMinAvgWaitingTime(Process *arr, int count) {
    arr = arr;
    return count;
}



/**
 * @brief print the greeting/introduction
 * 
 */
void printGreeting() {

}

/**
 * @brief print a table of the processes and their struct members
 *        R1 C1 => empty
 * 
 *        EACH ROW => process pid and their struct members
 *        COL 1 => process pids
 * 
 *        COLUMN HEADERS
 *        COL 2 => arrival time
 *        COL 3 => burst time
 *        COL 4 => turnaround time
 *        COL 5 => waiting time
 * 
 *        NOTE: DONT PRINT timeRem
 * 
 * @param arr - array of processes
 * @param count - length of array
 */
void printPropsOfAllProcesses(Process *arr, int count) {
    arr = arr;
    count = count;
}

/**
 * @brief print gantt chart of the scheduling
 * 
 * @param rt - runtime/simulation of the SJF scheduling of the given processes
 */
void printGanttChart(Runtime rt) {
    rt = rt;
}