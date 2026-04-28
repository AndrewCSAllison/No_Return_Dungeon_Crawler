#ifndef GAMELOG_H
#define GAMELOG_H
#include <stddef.h>

#define QUEUE_SIZE      5
#define LOG_BUF_COUNT   16
#define LOG_BUF_SIZE    64

typedef struct {
    char *data[QUEUE_SIZE];
    int head, tail, count;
} Queue;

extern Queue gameLog;

void queuePush(Queue *q, char *val);
char *queuePop(Queue *q);
void logMessage(char *msg);
char *buildLog(const char *a, const char *b, int value);
char *buildLogFull(const char *a, const char *b, const char *c, int value);
void clearLog();
#endif
