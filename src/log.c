#include "log.h"

Queue gameLog;

static char log_buffers[LOG_BUF_COUNT][LOG_BUF_SIZE];
static int log_buf_index = 0;

static char *nextBuf() {
    char *buf = log_buffers[log_buf_index];
    log_buf_index = (log_buf_index + 1) % LOG_BUF_COUNT;
    return buf;
}

static void strAppend(char *dst, const char *src, int *pos) {
    while (src[0] && *pos < LOG_BUF_SIZE - 1)
        dst[(*pos)++] = *src++;
    dst[*pos] = '\0';
}

static void intAppend(char *dst, int n, int *pos) {
    char tmp[12];
    int k = 0;
    if (n == 0) { tmp[k++] = '0'; }
    else { while (n > 0) { tmp[k++] = '0' + (n % 10); n /= 10; } }
    while (k > 0) dst[(*pos)++] = tmp[--k];
    dst[*pos] = '\0';
}

char *buildLog(const char *a, const char *b, int value) {
    char *buf = nextBuf();
    int pos = 0;
    if (a) strAppend(buf, a, &pos);
    if (b) strAppend(buf, b, &pos);
    if (value != 0) intAppend(buf, value, &pos);
    return buf;
}

void logMessage(char *msg) {
    if (gameLog.count == QUEUE_SIZE)
        queuePop(&gameLog);
    queuePush(&gameLog, msg);
}

void queuePush(Queue *q, char *val) {
    if (q->count == QUEUE_SIZE) return;
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;
}

char *queuePop(Queue *q) {
    if (q->count == 0) return NULL;
    char *val = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    return val;
}

char *buildLogFull(const char *a, const char *b, const char *c, int value) {
    char *buf = nextBuf();
    int pos = 0;
    if (a) strAppend(buf, a, &pos);
    if (b) strAppend(buf, b, &pos);
    if (c) strAppend(buf, c, &pos);
    if (value != 0) intAppend(buf, value, &pos);
    return buf;
}

void clearLog() {
    gameLog.head  = 0;
    gameLog.tail  = 0;
    gameLog.count = 0;
}
