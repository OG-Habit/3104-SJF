typedef struct process {
    char *pid;
    int timeArrival;
    int timeBurst;
    int timeTurnaround;
    int timeWaiting;
} Process;

typedef struct runtimeProcessLL {
    char *pid;
    int duration;
} *RuntimeProcessLL;

typedef struct runtime {
    RuntimeProcessLL LL;
    RuntimeProcessLL front;
    RuntimeProcessLL rear;
} Runtime;