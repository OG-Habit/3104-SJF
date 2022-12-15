#include "dataStruct.h"

int getNumOfProcesses(char *prompt);
Process initProcess(int pid);
Runtime simulateScheduling(Process *arr, int count);
int getMinAvgWaitingTime(Process *arr);

void printGreeting();
void printPropsOfAllProcesses(Process *arr);
void printGanttChart(Runtime rt);

/**
 * @brief Get the number of processes
 * 
 * @param prompt - question
 * @return int - total number of processes to be simulated
 */
int getNumOfProcesses(char *prompt) {

}

/**
 * @brief Initialize the arrival time, CPU burst time, pid
 *        For pid => P1, P2, P3, ..., Pn
 * 
 * @param pid - process identifier
 * @return Process 
 */
Process initProcess(int pid) {

}

/**
 * @brief Simulate scheduling of a given array of processes of n size
 *        Init turnaround time
 *        Init waiting time
 * 
 * @param arr - array of processes
 * @param count - size of the array
 * @return Runtime - sequence of the SJF scheduling of the given array of processes
 */
Runtime simulateScheduling(Process *arr, int count) {

}

/**
 * @brief Get the min avg waiting time for the processes
 * 
 * @param arr - array of processes
 * @return int - min avg waiting time
 */
int getMinAvgWaitingTime(Process *arr) {

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
 * @param arr - array of processes
 */
void printPropsOfAllProcesses(Process *arr) {

}

/**
 * @brief print gantt chart of the scheduling
 * 
 * @param rt - runtime/simulation of the SJF scheduling of the given processes
 */
void printGanttChart(Runtime rt) {
    
}