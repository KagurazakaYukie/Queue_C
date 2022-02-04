#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include "mtmm.h"

QueueManager *QueueManagerInit(MTMemoryManager *mm) {
	MemoryBigUnit *mbu=MTMemoryManagerBindingThread(mm,1);
	MemoryInfo *mi=SmallUnitAlloc(mm,mbu,mbu->snext,sizeof(QueueManager));
	mi->id=1;
	QueueManager *qm = (QueueManager *) mi->m;
	qm->t = NULL;
	qm->w = NULL;
	qm->Size = 0;
	qm->mbu=mbu;
	qm->msu=mbu->snext;
	qm->mi=mi;
	return qm;
}

Queue *QueueSetNote(Queue * q, void *v) {
	q->note = v;
	return q;
}

Queue *QueueManagerAdd(MTMemoryManager *mm,QueueManager * qm, void *b, void *data) {
	MemoryInfo *mi=SmallUnitAlloc(mm,qm->mbu,qm->msu,sizeof(Queue));
	mi->id=1;
	Queue *q = (Queue *) mi->m;
	q->mi=mi;
	q->data = data;
	q->ft = b;
	q->next = NULL;
	q->note=NULL;
	if (qm->t == NULL) {
		qm->t = q;
		qm->w = q;
	} else {
		qm->w->next = q;
		qm->w = q;
	}
	qm->Size++;
	return q;
}

Queue *QueueManagerAddQueue(QueueManager * qm, Queue * q) {
  if (qm->t == NULL) {
    qm->t = q;
    qm->w = q;
  } else {
    qm->w->next = q;
    qm->w = q;
  }
  qm->Size++;
  return q;
}

QueueManager *QueueManagerDelete(MTMemoryManager *mm,QueueManager * qm) {
  if (qm->t != NULL) {
    Queue *q = qm->t;
    if (qm->t->next != NULL) {
      qm->t = qm->t->next;
      if (qm->Size != 0) {
        qm->Size--;
      }
    } else {
      if (qm->t != NULL) {
        qm->t = NULL;
        if (qm->Size != 0) {
          qm->Size--;
        }
      }
    }
    if (q->data != NULL) {
      MTMemoryManagerFree(mm,q->data);
      q->data = NULL;
    }
    if (q->note != NULL) {
      MTMemoryManagerFree(mm,q->note);
      q->note = NULL;
    }
    MTMemoryManagerUnitFree(qm->mbu,q->mi);
    q = NULL;
  }
  return qm;
}

QueueManager *QueueManagerDelete2(MTMemoryManager *mm,QueueManager * qm) {
  if (qm->t != NULL) {
    Queue *q = qm->t;
    if (qm->t->next != NULL) {
      qm->t = qm->t->next;
      if (qm->Size != 0) {
        qm->Size--;
      }
    } else {
      if (qm->t != NULL) {
        qm->t = NULL;
        if (qm->Size != 0) {
          qm->Size--;
        }
      }
    }
    MTMemoryManagerUnitFree(qm->mbu,q->mi);
    q = NULL;
  }
  return qm;
}

Queue *QueueManagerOut(QueueManager * qm) {
  Queue *b;
  if (qm->t == NULL) {
    return NULL;
  } else {
    if (qm->t->next == NULL) {
      b = qm->t;
      qm->t = NULL;
      if (qm->Size != 0) {
        qm->Size--;
      }
    } else {
      b = qm->t;
      qm->t = qm->t->next;
      if (qm->Size != 0) {
        qm->Size--;
      }
    }
  }
  return b;
}

Queue *QueueInit(MTMemoryManager *mm,QueueManager *qm,void *ft, void *data) {
  MemoryInfo *mi=SmallUnitAlloc(mm,qm->mbu,qm->msu,sizeof(Queue));
  mi->id=1;
  Queue *q=(Queue*) mi->m;
  q->mi=mi;
  q->data = data;
  q->ft = ft;
  q->next = NULL;
  q->note = NULL;
  return q;
}

void QueueDestroy(MTMemoryManager *mm,QueueManager *qm,Queue * a) {
  if (a != NULL) {
    MTMemoryManagerFree(mm,a->data);
    MTMemoryManagerUnitFree(qm->mbu,a->mi);
    a = NULL;
  }
}

void QueueDestroy2(QueueManager *qm,Queue * a) {
  if (a != NULL) {
    MTMemoryManagerUnitFree(qm->mbu,a->mi);
    a = NULL;
  }
}

Queue *QueueManagerGet(QueueManager * qm) {
  if (qm->t == NULL) {
    return NULL;
  } else {
    return qm->t;
  }
}

void QueueManagerDestroy(MTMemoryManager *mm,QueueManager * qm) {
  while (true) {
    Queue *t = qm->t;
    if (t != NULL) {
      if (qm->t->data != NULL) {
        MTMemoryManagerFree(mm,qm->t->data);
        qm->t->data = NULL;
      }
      if (qm->t->note != NULL) {
        MTMemoryManagerFree(mm,qm->t->note);
        qm->t->note = NULL;
      }
      MTMemoryManagerUnitFree(qm->mbu,qm->t->mi);
      qm->t = NULL;
      qm->t = t->next;
    } else {
      break;
    }
  }
  MTMemoryManagerAppointComleteInitUnit(qm->mbu);
}

void QueueManagerDestroy2(QueueManager * qm) {
  while (true) {
    Queue *t = qm->t;
    if (t != NULL) {
      MTMemoryManagerUnitFree(qm->mbu,qm->t->mi);
      qm->t = NULL;
      qm->t = t->next;
    } else {
      break;
    }
  }
  MTMemoryManagerAppointComleteInitUnit(qm->mbu);
}
