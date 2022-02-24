#include "Queue.h"
#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>
#include "MultitThreadMemoryManager.h"

QueueManager *QueueManagerInit(MTMemoryManager *mm) {
    MemoryBigUnit *mbu = MTMemoryManagerBindingThread(mm, 1);
    MemoryInfo *mi = MTMemoryManagerUnitCalloc(mm, mbu, sizeof(QueueManager));
    MemoryInfo *semm = MTMemoryManagerUnitCalloc(mm, mbu, sizeof(QueueManager));
    sem_t *sem = (sem_t *) semm->m;
    QueueManager *qm = (QueueManager *) mi->m;
    qm->t = NULL;
    qm->w = NULL;
    qm->mbu = mbu;
    qm->msu = mbu->snext;
    qm->sem = sem;
    qm->semm = semm;
    qm->mi = mi;
    return qm;
}

Queue *QueueSetNote(Queue *q, void *v) {
    q->note = v;
    return q;
}

Queue *QueueManagerAdd(MTMemoryManager *mm, QueueManager *qm, void *b, void *data,void *note) {
    MemoryInfo *mi = MTMemoryManagerUnitCalloc(mm, qm->mbu, sizeof(Queue));
    Queue *q = (Queue *) mi->m;
    q->mi = mi;
    q->data = data;
    q->ft = b;
    q->next = NULL;
    q->note = note;
    if (qm->t == NULL) {
        qm->t = q;
        qm->w = q;
    } else {
        qm->w->next = q;
        qm->w = q;
    }
    sem_post(qm->sem);
    return q;
}

Queue *QueueManagerAddQueue(QueueManager *qm, Queue *q) {
    if (qm->t == NULL) {
        qm->t = q;
        qm->w = q;
    } else {
        qm->w->next = q;
        qm->w = q;
    }
    sem_post(qm->sem);
    return q;
}

QueueManager *QueueManagerDelete(MTMemoryManager *mm, QueueManager *qm) {
    if (qm->t != NULL) {
        Queue *q = qm->t;
        if (qm->t->next != NULL) {
            qm->t = qm->t->next;
            sem_wait(qm->sem);
        } else {
            if (qm->t != NULL) {
                qm->t = NULL;
                sem_wait(qm->sem);
            }
        }
        if (q->data != NULL) {
            MTMemoryManagerFree(mm, q->data);
            q->data = NULL;
        }
        if (q->note != NULL) {
            MTMemoryManagerFree(mm, q->note);
            q->note = NULL;
        }
        MTMemoryManagerUnitFree(qm->mbu, q->mi);
        q = NULL;
    }
    return qm;
}

QueueManager *QueueManagerDelete2(MTMemoryManager *mm, QueueManager *qm) {
    if (qm->t != NULL) {
        Queue *q = qm->t;
        if (qm->t->next != NULL) {
            qm->t = qm->t->next;
            sem_wait(qm->sem);
        } else {
            if (qm->t != NULL) {
                qm->t = NULL;
                sem_wait(qm->sem);
            }
        }
        MTMemoryManagerUnitFree(qm->mbu, q->mi);
        q = NULL;
    }
    return qm;
}

Queue *QueueManagerOut(QueueManager *qm) {
    sem_wait(qm->sem);
    Queue *b=NULL;
    if (qm->t->next == NULL) {
        b = qm->t;
        qm->t = NULL;
    } else {
        b = qm->t;
        qm->t = qm->t->next;
    }
    return b;
}

Queue *QueueInit(MTMemoryManager *mm, QueueManager *qm, void *ft, void *data) {
    MemoryInfo *mi = MTMemoryManagerUnitCalloc(mm, qm->mbu, sizeof(Queue));
    Queue *q = (Queue *) mi->m;
    q->mi = mi;
    q->data = data;
    q->ft = ft;
    q->next = NULL;
    q->note = NULL;
    return q;
}

void QueueDestroy(MTMemoryManager *mm, QueueManager *qm, Queue *a) {
    if (a != NULL) {
        MTMemoryManagerFree(mm, a->data);
        MTMemoryManagerUnitFree(qm->mbu, a->mi);
    }
}

void QueueDestroy2(QueueManager *qm, Queue *a) {
    if (a != NULL) {
        MTMemoryManagerUnitFree(qm->mbu, a->mi);
    }
}

Queue *QueueManagerGet(QueueManager *qm) {
    if (qm->t == NULL) {
        return NULL;
    } else {
        return qm->t;
    }
}

void QueueManagerDestroy(MTMemoryManager *mm, QueueManager *qm) {
    while (true) {
        Queue *t = qm->t;
        if (t != NULL) {
            if (qm->t->data != NULL) {
                MTMemoryManagerFree(mm, qm->t->data);
                qm->t->data = NULL;
            }
            if (qm->t->note != NULL) {
                MTMemoryManagerFree(mm, qm->t->note);
                qm->t->note = NULL;
            }
            MTMemoryManagerUnitFree(qm->mbu, qm->t->mi);
            qm->t = NULL;
            qm->t = t->next;
        } else {
            break;
        }
    }
    sem_destroy(qm->sem);
    MTMemoryManagerAppointComleteInitUnit(qm->mbu);
}

void QueueManagerDestroy2(QueueManager *qm) {
    while (true) {
        Queue *t = qm->t;
        if (t != NULL) {
            MTMemoryManagerUnitFree(qm->mbu, qm->t->mi);
            qm->t = NULL;
            qm->t = t->next;
        } else {
            break;
        }
    }
    sem_destroy(qm->sem);
    MTMemoryManagerAppointComleteInitUnit(qm->mbu);
}
