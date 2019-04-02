/* Writen by Sam Garretson
 * Implementation of Queue class */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cda.h"
#include "queue.h"

// FIFO
struct queue {
	CDA *q;
	int debugFlag;
	void (*display)(void *, FILE *);
	void (*free)(void *);
};

/*************** public interface *************/

/* Constructor */
QUEUE *newQUEUE() {
	QUEUE *items = malloc(sizeof(QUEUE));
	assert(items != 0);
	items->q = newCDA();
	assert(items->q != 0);
	items->debugFlag = 0;
	return items;
}

void  setQUEUEdisplay(QUEUE *items,void (*d)(void *,FILE *)) {
	items->display = d;
	setCDAdisplay(items->q, d);
}

void  setQUEUEfree(QUEUE *items,void (*f)(void *)) {
	items->free = f;
	setCDAfree(items->q, f);
}

void  enqueue(QUEUE *items,void *value) {
	insertCDA(items->q, sizeCDA(items->q), value);
}

void *dequeue(QUEUE *items) {
	assert(sizeCDA(items->q) > 0);
	return removeCDA(items->q, 0);
}

void *peekQUEUE(QUEUE *items) {
	assert(sizeCDA(items->q) > 0);
	return getCDA(items->q, 0);
}

void  displayQUEUE(QUEUE *items,FILE *fp) {
	if (items->debugFlag == 1) {
		int old = debugCDA(items->q, 0);
		displayCDA(items->q, fp);
		debugCDA(items->q, old);
	} else if (items->debugFlag == 2) {
		int old = debugCDA(items->q, 1);
		displayCDA(items->q, fp);
		debugCDA(items->q, old);
	} else {
		fprintf(fp, "<");
		for(int i = 0; i < sizeCDA(items->q); i++) {
			items->display(getCDA(items->q, i), fp);
			if (i < sizeCDA(items->q)-1)
				fprintf(fp, ",");
			}
		fprintf(fp, ">");
		}
}
int   debugQUEUE(QUEUE *items,int level) {
	int prevFlag = items->debugFlag;
	items->debugFlag = level;
	return prevFlag;
}

void  freeQUEUE(QUEUE *items) {
	freeCDA(items->q);
	free(items);
}

int   sizeQUEUE(QUEUE *items) {
	return sizeCDA(items->q);
}
