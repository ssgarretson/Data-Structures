/* Writen by Sam Garretson
 * Implementation of Stack class */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "da.h"
#include "stack.h"

// LIFO
struct stack {
	DA *s;
	int debugFlag;
	void (*display)(void *,FILE *);
	void (*free)(void *);
};

/*************** public methods *************/

/* Constructor */
STACK *newSTACK() {
	STACK *items = malloc(sizeof(STACK));
	assert(items != 0);
	items->s = newDA();
	assert(items->s != 0);
	items->debugFlag = 0;
	return items;
}

void  setSTACKdisplay(STACK *items,void (*d)(void *,FILE *)) {
	items->display = d;
	setDAdisplay(items->s, d);
}

void  setSTACKfree(STACK *items,void (*f)(void *)) {
	items->free = f;
	setDAfree(items->s, f);
}

void  push(STACK *items,void *value) {
	insertDA(items->s, sizeDA(items->s), value);
}

void *pop(STACK *items) {
	assert(sizeDA(items->s) > 0);
	return removeDA(items->s, sizeDA(items->s)-1);
}

void *peekSTACK(STACK *items) {
	assert(sizeDA(items->s) > 0);
	return getDA(items->s, sizeDA(items->s)-1);
}

void  displaySTACK(STACK *items,FILE *fp) {
	if (items->debugFlag == 1) {
		int old = debugDA(items->s, 0);
		displayDA(items->s, fp);
		debugDA(items->s, old);
	} else if (items->debugFlag == 2) {
		int old = debugDA(items->s, 1);
		displayDA(items->s, fp);
		debugDA(items->s, old);
	} else {
			fprintf(fp, "|");
			for(int i = sizeDA(items->s) - 1; i >= 0; i--) {
				items->display(getDA(items->s, i), fp);
				if (i > 0)
					fprintf(fp, ",");
				}
				fprintf(fp, "|");
	}
}

int   debugSTACK(STACK *items,int level) {
	int prevFLag = items->debugFlag;
	items->debugFlag = level;
	return prevFLag;
}


void  freeSTACK(STACK *items) {
	freeDA(items->s);
	free(items);
}

int   sizeSTACK(STACK *items) {
	return sizeDA(items->s);
}
