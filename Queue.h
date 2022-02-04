#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "MultitThreadMemoryManager.h"

typedef struct q {
  void *data;
  void *note;
  void *ft;
  struct q *next;
  MemoryInfo *mi;
} Queue;

typedef struct wer {
  Queue *t;
  Queue *w;
  int Size;
  MemoryBigUnit *mbu;
  MemorySmallUnit *msu;
  MemoryInfo *mi;
} QueueManager;

QueueManager *QueueManagerInit(MTMemoryManager *mm);

Queue *QueueSetNote(Queue * q, void *v);

Queue *QueueManagerAdd(MTMemoryManager *mm,QueueManager * qm, void *b, void *data);

Queue *QueueManagerAddQueue(QueueManager * qm, Queue * q);

QueueManager *QueueManagerDelete(MTMemoryManager *mm,QueueManager * qm);

QueueManager *QueueManagerDelete2(MTMemoryManager *mm,QueueManager * qm);

Queue *QueueManagerOut(QueueManager * qm);

Queue *QueueInit(MTMemoryManager *mm,QueueManager *qm,void *ft, void *data);

void QueueDestroy(MTMemoryManager *mm,QueueManager *qm,Queue * a);

void QueueDestroy2(QueueManager *qm,Queue * a);

Queue *QueueManagerGet(QueueManager * qm);

void QueueManagerDestroy(MTMemoryManager *mm,QueueManager * qm);

void QueueManagerDestroy2(QueueManager * qm);
#endif
