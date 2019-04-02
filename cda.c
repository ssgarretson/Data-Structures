/* Implementation of Circular Dynamic Array Class
 * Written by Sam Garretson */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cda.h"

struct cda {
  int size, capacity, front, back, debugFlag;
  void **arr;
  void (*display)(void *, FILE *);
  void (*free)(void *);
};

static void changeCDAcapacity(CDA *items, int newCapacity);   // Grow or shrink CDA
static void insertCDAfrontHelper(CDA *items, void *value);
static void insertCDAbackHelper(CDA *items, void *value);

/*************** public methods *************/

/* Constructor */
CDA *newCDA(void) {
  CDA *items = malloc(sizeof(CDA));
  assert(items != 0);
  items->arr = malloc(sizeof(void *));
  assert(items->arr != 0);
  items->capacity = 1;
  items->size = 0;
  items->front = 0;
  items->back = -1;
  items->debugFlag = 0;
  return items;
}

/* Set the Dispaly Function for Your DA */
void setCDAdisplay(CDA *items, void (*d)(void *,FILE *)) {
  items->display = d;
  return;
}

/* Set the Free Function for your DA */
void setCDAfree(CDA *items, void (*f)(void *)) {
  items->free = f;
  return;
}

void insertCDA(CDA *items,int index,void *value) {
  assert(index >= 0);
  assert(index <= items->size);
  if (index == 0) insertCDAfrontHelper(items, value);
  else if (index == items->size) insertCDAbackHelper(items, value);
  else {
    if (items->size == items->capacity)
  		changeCDAcapacity(items, items->capacity * 2);
    items->size++;
    if( index < (items->size/2)) {
      items->front = (items->front - 1 + items->capacity) % items->capacity;
      for (int i = 0; i < index; i++)
        setCDA(items, i, getCDA(items, i + 1));
    } else {
      items->back = (items->back + 1) % items->capacity;
      for (int i = items->size - 1; i > index; i--)
        setCDA(items, i, getCDA(items, i - 1));
    }
    setCDA(items, index, value);
  }
}


void *removeCDA(CDA *items,int index) {
  assert(index >= 0);
  assert(index < items->size);
  assert(items->size > 0);
  void *removedValue = getCDA(items, index);
  if (index == 0)
    	items->front = (items->front + 1) % items->capacity;
  else if (index == items->size - 1) {
	   if (items->back == -1) items->back = items->front;
     items->back = (items->back - 1 + items->capacity) % items->capacity;
  } else {
    if (index < (items->size/2)) {
        for (int i = index; i > 0; --i)
          setCDA(items, i, getCDA(items, i - 1));
        items->front = (items->front + 1) % items->capacity;
    } else {
        for (int i = index; i < items->size-1; i++)
          setCDA(items, i, getCDA(items, i + 1));
        items->back = (items->back - 1 + items->capacity) % items->capacity;
    }
  }
  items->size--;
  if (((double)items->size / items-> capacity) * 100 < 25) {
    if (items->size == 0) changeCDAcapacity(items, 1);
    else changeCDAcapacity(items, items->capacity / 2);
  }
  return removedValue;
}

/* Moves all items in the donor array to the recipient array */
void unionCDA(CDA *recipient,CDA *donor) {
  int i;
	for (i = 0; i < donor->size; i++)
		insertCDAbackHelper(recipient, getCDA(donor, i));

	// Donor array
	free(donor->arr);
	donor->arr = malloc(sizeof(void *));
	donor->capacity = 1;
	donor->size     = 0;
	donor->front    = 0;
	donor->back     = 0;

	return;
}

/* Return the value at given index */
void *getCDA(CDA *items,int index) {
  assert(index >= 0);
  assert(index < items->size);
  return items->arr[(items->front + index + items->capacity) % items->capacity];
}

void *setCDA(CDA *items,int index,void *value) {
  assert(index >= -1);
  assert(index <= items->size);
  if (index == items->size) {
    insertCDAbackHelper(items, value);
    return NULL;
  } else if (index == -1) {
    insertCDAfrontHelper(items, value);
    return NULL;
  } else {
    void *replacedValue = getCDA(items, index);
  	items->arr[(items->front + index + items->capacity) % items->capacity] = value;
		return replacedValue;
  }
  return NULL;
}

/* Return the number of items stored in array */
int  sizeCDA(CDA *items) {
  return items->size;
}

void displayCDA(CDA *items,FILE *fp) {
  fprintf(fp, "(");
  int i;
  for (i = 0; i < items->size; i++) {
    if (items->display == NULL)
      fprintf(fp, "@%p", items->arr[i]);
    else
      items->display(getCDA(items, i), fp);
    if (i < items->size - 1)
      fprintf(fp, ",");
  }
  if (items->debugFlag > 0) {
    if (items->size > 0)
      fprintf(fp, ",");
    fprintf(fp, "(%d)", (items->capacity - items->size));
  }
  fprintf(fp, ")");
}

int  debugCDA(CDA *items,int level) {
  int prevFlag = items->debugFlag;
  items->debugFlag = level;
  return prevFlag;
}

void freeCDA(CDA *items) {
  for(int i = 0; i < items->size; i++) {
    items->free(getCDA(items, i));
  }
  free(items->arr);
  free(items);
}
/*************** private methods *************/

/* Insert the item to the left
 * If full, double the size */
void insertCDAfrontHelper(CDA *items, void *value) {
	if (items->size == items->capacity)
		changeCDAcapacity(items, items->capacity * 2);
	items->front = items->size == 0 ? 0 : (items->front - 1 + items->capacity) % items->capacity;
	items->arr[items->front] = value;
	items->size++;
}

/* Insert the item to the right
 * If full, double the size */
void insertCDAbackHelper(CDA *items, void *value) {
	if (items->size == items->capacity)
		changeCDAcapacity(items, items->capacity * 2);
	items->back = items->size == 0 ? 0 : (items->back + 1) % items->capacity;
	items->arr[items->back] = value;
	items->size++;
}

/* Used to change the Capacity of the array if it
 * gets too big, or small */
static void changeCDAcapacity(CDA *items, int newCapacity) {
	void **newArray = malloc(newCapacity * sizeof(void *));
	int i;
  //printf("start cap change\n");
	for (i = 0; i < items->size; i++)
		newArray[i] = getCDA(items, i);
  //printf("finished cap change\n");
	free(items->arr);
	items->arr      = newArray;
  items->front    = 0;
	items->back     = items->size - 1;
	items->capacity = newCapacity;
	return;
}

/*************** end-of-file *************/
