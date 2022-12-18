#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

double getMinAvgWaitingTime(Process *arr, int);
double getMinAvgTurnaroundTime(Process *arr, int);

void printGreeting();
void printPropsOfAllProcesses(Process *arr, int);
void printGanttChart(Runtime rt);

// void ftoa(float n, char* res, int ap);
// int intToStr(int x, char str[], int d);
// void reverse(char* str, int len);



/**
 * @brief Get the number of processes
 * 
 * @param prompt - question
 * @return int - total number of processes to be simulated
 */
int getNumOfProcesses(char *prompt) {
     int num, min = 1, max = 8;
    do{
        printf("%s",prompt);
        scanf("%d",&num);

        if(num > max){
            printf("Maximum number of processes is %d",max);
        }else if(num < 1){
            printf("Minimum number of process is %d\n",min);
        }
    }while(num > max || num < 1);

    return num;

}

/**
 * @brief Initialize the arrival time, decreasing arrival time, remaining time, CPU 
 *        burst time, pid
 *        For pid => P1, P2, P3, ..., Pn
 *        Burst time == remaining time
 *        Arrival time == decreasing arrival time
 *        NOTE: The input values for time are MILLISECONDS
 * 
 * @param pid - process identifier
 * @return Process 
 */
Process initProcess(int pid) {
    char buffer[4];
    Process p = {0};
    pid++;
    sprintf(buffer, "P%d", pid);
    p.pid = (char*)malloc(sizeof(char) * strlen(buffer)+1);
    strcpy(p.pid,buffer);
    p.timeArrival = 0;
    p.timeBurst = 0;
    p.timeTurnaround = 0;
    p.timeWaiting = 0;

    printf("Enter Arrival Time(in milliseconds) of P%d: ",pid);
    scanf("%d",&p.timeArrival);
    printf("Enter Burst Time(in milliseconds) of P%d: ",pid);
    scanf("%d",&p.timeBurst);
    printf("\n");
    p._timeArrivalDec = p.timeArrival;
    p._timeRem = p.timeBurst; 


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
            if(currProcess != NULL) {
                enqueueRuntime(&rt, *currProcess->processPtr);
                ready = clearBit(ready, currProcess->ndx);
            }
        } else {
            // check if the shortest job in queue is smaller than current
            if(queue != NULL &&
                queue->processPtr->_timeRem < currProcess->processPtr->_timeRem) {
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

/**
 * @brief Creates a bitmap of 1s
 * 
 * @param count - determines how many 1s
 * @return Bitmap 
 */
Bitmap initBitmap(int count) {
    int x, one = 1;
    Bitmap bitmap = 0;
    for(x = 0; x < count; x++) {
        bitmap |= one;
        one <<= 1;
    }
    return bitmap;
}

/**
 * @brief Checks if process is in ready queue
 * 
 * @param ready - bitmap of ready processes
 * @param ndx - index of process in the array
 * @return int - 1 for ready, 0 for not ready
 */
int isReady(Bitmap ready, int ndx) {
    int x = 1 << ndx;
    return (ready | x) == ready ? 1 : 0; 
}

/**
 * @brief Checks if process is unfinished
 * 
 * @param unfinished - bitmap of unfinished processes
 * @param ndx - index of process in the array
 * @return int - 1 for unfinished, 0 for finished
 */
int isUnfinished(Bitmap unfinished, int ndx) {
    int x = 1 << ndx;
    return (unfinished | x) == unfinished ? 1 : 0; 
}

/**
 * @brief Checks if the selected process is currently running 
 * 
 * @param rt - runtime where we record running processes
 * @param pid - id / name of the selected process
 * @return int - 1 for selected process is currently running, 0 for otherwise
 */
int isCurrentProcess(Runtime rt, char *pid) {
    if(rt.rear != NULL) {
        return strcmp(pid, rt.rear->pid) == 0 ? 1 : 0;
    } else {
        return 0;
    }
}

/**
 * @brief Set a bit in the bitmap
 * 
 * @param bitmap - given bitmap 
 * @param ndx - position within the bitmap to be set
 * @return Bitmap - updated bitmap with the ndx bit set
 */
Bitmap setBit(Bitmap bitmap, int ndx) {
    return bitmap | 1 << ndx;
}

/**
 * @brief Clear a bit in the bitmap
 * 
 * @param bitmap - given bitmap 
 * @param ndx - position within the bitmap to be cleared
 * @return Bitmap - updated bitmap with the ndx bit cleared
 */
Bitmap clearBit(Bitmap bitmap, int ndx) {
    return bitmap ^ 1 << ndx;
}

/**
 * @brief Enqueue a process into the ready queue in an ascending order  
 *        based from   
 *        the burst time
 * 
 * @param head - ready queue
 * @param process - process to be enqueued
 * @param ndx - index of the process in the array of processes
 */
void enqueueReadyQueue(ProcessLL *head, Process *process, int ndx) {
    ProcessLL *trav, newLL;

    newLL = (ProcessLL)calloc(1, sizeof(struct processLL));
    newLL->ndx = ndx;
    newLL->processPtr = process;
    for(trav = head; *trav != NULL; trav = &(*trav)->link) {
        if(process->_timeRem < (*trav)->processPtr->_timeRem) {
            break;
        }
    } 
    newLL->link = *trav;
    *trav = newLL;
}

/**
 * @brief Dequeue a process from the ready queue
 * 
 * @param queue - ready queue
 * @return ProcessLL - dequeued process
 */
ProcessLL dequeueReadyQueue(ProcessLL *queue) {
    ProcessLL ll = *queue;
    if(*queue != NULL) {
        *queue = (*queue)->link;
    }
    return ll;
}

/**
 * @brief Enqueue a running process into the runtime 
 * 
 * @param rt - runtime of the execution of sjf
 * @param p - current running process
 */
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
 * @return double - min avg waiting time\
 * 
 * Note: Unit is SECONDS
 */
double getMinAvgWaitingTime(Process *arr, int count) {
    int x;
    double result,res,total = 0.00;
    for(x=0;x<count;x++){
        result = (double)arr[x].timeTurnaround - (double)arr[x].timeBurst;
        total += result;
    }
    res = total/count;
    return res;
}


/**
 * @brief Get the min avg waiting time for the processes
 * 
 * @param arr - array of processes
 * @param count - length of array
 * @return double - avg turnaround time
 * 
 * Note: Unit is SECONDS
 */
double getMinAvgTurnaroundTime(Process *arr, int count) {
    int x;
    double res,total = 0.00;
    for(x=0;x<count;x++){
        total += (double)arr[x].timeTurnaround;
    }
    res = total/count;
    return res;
}

/**
 * @brief print the greeting/introduction
 * 
 */
void printGreeting() {
    printf("\n============================  PREEMPTIVE SJF CPU SCHEDULING PROGRAM  ============================\n");
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
 *        NOTE: Time must be in SECONDS
 * 
 * @param arr - array of processes
 * @param count - length of array
 */
void printPropsOfAllProcesses(Process *arr, int count) {
    int x;
    arr = arr;
    count = count;
    printf("================================================================================================\n");
    printf("PID\t Arrival Time\t\t Burst Time\t\t Turnaround Time\t Waiting Time\n"); 
    printf("================================================================================================\n");

    for(x = 0; x < count; x++) {
        printf("%s\t %d ms\t\t\t %d ms\t\t\t %d ms\t\t\t %d ms\n",arr[x].pid,arr[x].timeArrival,arr[x].timeBurst,arr[x].timeTurnaround,arr[x].timeWaiting);
    }
}

/**
 * @brief print gantt chart of the scheduling
 * 
 * @param rt - runtime/simulation of the SJF scheduling of the given processes
 * 
 * This prints a gantt chart with int values
 */
void printGanttChart(Runtime rt) {
    RuntimeProcessLL trav;
    int x, SPACE = 7;
     printf("\n\nGANTT CHART\n\n");

    for(trav=rt.front; trav != NULL; trav =  trav->link){
        char *str = (char *) calloc(trav->duration*SPACE+1,sizeof(char));
        memset(str,'=', trav->duration*SPACE);
        printf("|%s",str);
    }
        printf("|\n");

    for(trav=rt.front; trav != NULL; trav =  trav->link){
        char *str = (char *) calloc(trav->duration*SPACE+1,sizeof(char));
        memset(str,' ', (trav->duration*SPACE));
        str[strlen(str)/2-1] = trav->pid[0];
        str[(strlen(str)/2)] = trav->pid[1];
        printf("|%s",str);
    }
        printf("|\n");

    for(trav=rt.front; trav != NULL; trav =  trav->link){
        char *str = (char *) calloc(trav->duration*SPACE+1,sizeof(char));
        memset(str,'=', (trav->duration*SPACE));
        printf("|%s",str);
    }
        printf("|\n0");

    for(trav=rt.front,x=0; trav != NULL; trav =  trav->link){
        char res[15],*str = (char *) calloc((trav->duration*SPACE+10),sizeof(char));
        memset(str,' ', (trav->duration*SPACE));
        itoa(trav->duration+x, res, 10);
        x += trav->duration;
        int res_len = strlen(res);
        memcpy(str+((trav->duration*SPACE)-(res_len-1)),res,res_len);
        printf("%s",str);
    }
    printf("\n");
}

/**
 * @brief print gantt chart of the scheduling
 * 
 * @param rt - runtime/simulation of the SJF scheduling of the given processes
 * 
 * This prints a gantt chart with floating values
 */
// void printGanttChartv2(Runtime rt) {
//     RuntimeProcessLL trav;
//     int x;
//      printf("\n\nGANTT CHART\n\n");

//     for(trav=rt.front; trav != NULL; trav =  trav->link){
//         char *str = (char *) calloc(trav->duration,sizeof(char));
//         memset(str,'=', (trav->duration*8)/1000);
//         printf("|%s",str);
//     }
//         printf("|\n");

//     for(trav=rt.front; trav != NULL; trav =  trav->link){
//         char *str = (char *) calloc(trav->duration,sizeof(char));
//         memset(str,' ', (trav->duration*8)/1000);
//         str[strlen(str)/2] = trav->pid[0];
//         str[(strlen(str)/2)+1] = trav->pid[1];
//         printf("|%s",str);
//     }
//         printf("|\n");

//     for(trav=rt.front; trav != NULL; trav =  trav->link){
//         char *str = (char *) calloc(trav->duration,sizeof(char));
//         memset(str,'=', (trav->duration*8)/1000);
//         printf("|%s",str);
//     }
//         printf("|\n0");

//     double dur = 0.00; 
//     for(trav=rt.front,x=0; trav != NULL; trav =  trav->link,x++){
//         char *res,*str = (char *) calloc((trav->duration*8)/1000,sizeof(char));
//         dur += (double)trav->duration/1000;
//         memset(str,' ', (trav->duration*8)/1000);
//         ftoa(dur,res,1);
//         int res_len = strlen(res);
//         memcpy(str+(((trav->duration*8)/1000)-(res_len-1)),res,res_len);
//         printf("%s",str);
//     }
//     printf("\n");
// }

// /**
//  * @brief converts float number to string
//  * 
//  * @param n - input number
//  * @param res - buffer array where output string will be stored
//  * @param ap - number of digits after float/double point
//  * 
//  */
// void ftoa(float n, char* res, int ap){
//     // Extract integer part
//     int ipart = (int)n;
 
//     // Extract floating part
//     float fpart = n - (float)ipart;
 
//     // convert integer part to string
//     int i = intToStr(ipart, res, 0);
 
//     // check for display option after point
//     if (ap != 0) {
//         res[i] = '.'; // add dot
 
//         // Get the value of fraction part upto given no.
//         // of points after dot. The third parameter
//         // is needed to handle cases like 233.007
//         fpart = fpart * pow(10, ap);
 
//         intToStr((int)fpart, res + i + 1, ap);
//     }
// }

// /**
//  * @brief converts int to string
//  * 
//  * @param x - input number
//  * @param str - buffer array where output string will be stored
//  * @param d - number of digits required in the output
//  * 
//  */
// int intToStr(int x, char str[], int d)
// {
//     int i = 0;
//     while (x) {
//         str[i++] = (x % 10) + '0';
//         x = x / 10;
//     }
 
//     // If number of digits required is more, then
//     // add 0s at the beginning
//     while (i < d)
//         str[i++] = '0';
 
//     reverse(str, i);
//     str[i] = '\0';
//     return i;
// }

// /**
//  * @brief  reverse the string
//  * 
//  * @param str -the string to be reversed
//  * @param d - len of the string
//  * 
//  */
// void reverse(char* str, int len)
// {
//     int i = 0, j = len - 1, temp;
//     while (i < j) {
//         temp = str[i];
//         str[i] = str[j];
//         str[j] = temp;
//         i++;
//         j--;
//     }
// }