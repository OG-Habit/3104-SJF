typedef unsigned char Bitmap;

typedef struct process {
    char *pid;
    int timeArrival;
    int timeBurst;
    int timeTurnaround;
    int timeWaiting;
    int _timeRem;
    int _timeArrivalDec;
} Process;

typedef struct processLL {
    Process *processPtr;
    int ndx;
    struct processLL *link;
} *ProcessLL;

typedef struct runtimeProcessLL {
    char *pid;
    int duration;
    struct runtimeProcessLL *link;
} *RuntimeProcessLL;

typedef struct runtime {
    RuntimeProcessLL front;
    RuntimeProcessLL rear;
} Runtime;

