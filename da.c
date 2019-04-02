/* Implementation of Dynamic Array Class
 * Written by Sam Garretson */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "da.h"

struct da {
  int size, capacity;
  int debugFlag;
  void **arr;
  void (*display)(void *, FILE *);
  void (*free)(void *);
};

/**************** public methods **************/

/* Constructor */
DA *newDA(void) {
  DA *items = malloc(sizeof(DA));
  assert(items != 0);
  items->arr = malloc(sizeof(void *));
  assert(items->arr != 0);
  items->capacity = 1;
  items->size = 0;
  items->debugFlag = 0;
  return items;
}

/* Set the Dispaly Function for Your DA */
void setDAdisplay(DA *items,void (*d)(void *,FILE *)) {
  items->display = d;
  return;
}

/* Set the Free Function for your DA */
void setDAfree(DA *items,void (*f)(void *)) {
  items->free = f;
  return;
}

/* Insert the given Value at the given Index,
 * and shift all other values down.
 * If full, double the capacity */
void  insertDA(DA *items,int index,void *value) {
  if (items->size == items->capacity) {
    items->capacity *= 2;
    items->arr = realloc(items->arr, items->capacity * sizeof(void *));
    assert(items->arr != 0);
  }

  for (int i = items->size - 1; i >= index; --i)
    items->arr[i+1] = items->arr[i];
  items->arr[index] = value;
  items->size++;
  return;
}

/* Remove and Return the Value at the given Index
 * If the ratio of the size to the capacity drops below 25%,
 * the array shrinks by half */
void *removeDA(DA *items,int index) {
  assert(items->size > 0);
  void *removedValue = items->arr[index];
  items->size = items->size-1;
  for (int i = index; i < items->size; i++)
    items->arr[i] = items->arr[i+1];
  if (((double)items->size / items->capacity) * 100 < 25) {
    if (items->size == 0) items->capacity = 1;
    else items->capacity = items->capacity / 2;
		items->arr = realloc(items->arr, items->capacity * sizeof(void *));
		assert(items->arr != 0);
	}
  return removedValue;
}

/* Takes two arrays and moves all the items
 * in the donor array to the recipient array. */
void  unionDA(DA *recipient,DA *donor) {
  for (int i = 0; i < donor->size; i++)
    insertDA(recipient, recipient->size, getDA(donor, i));

  free(donor->arr);
  donor->arr = malloc(sizeof(void *));
  donor->capacity = 1;
  donor->size     = 0;

  return;
}

/* Return the value at given index */
void *getDA(DA *items, int index) {
	assert(index >= 0);
	assert(index < items->size);
	return items->arr[index];
}

/* Set the value at index */
void *setDA(DA *items, int index, void *value) {
	assert(index >= 0);
	assert(index <= items->size);
	if (index == items->size) {
		insertDA(items, index, value);
		return NULL;
  }
  void *removedValue = items->arr[index];
	if (index < items->size) {
		items->arr[index] = value;
		return removedValue;
	}
  free(removedValue);
	return NULL;
}

/* Return the number of items stored in array */
int sizeDA(DA *items) {
	return items->size;
}

/* Displays the DA
 * EX: [5,8,19,2,39] */
void  displayDA(DA *items,FILE *fp) {
  fprintf(fp, "[");
	int i;
	for (i = 0; i < items->size; i++) {
    if (items->display == NULL)
      fprintf(fp, "@%p", items->arr[i]);
   else
      items->display(items->arr[i], fp);
		if (i < items->size - 1)
			fprintf(fp, ",");
	}
  if (items->debugFlag > 0) {
    if (items->size > 0)
      fprintf(fp, ",");
    fprintf(fp, "[%d]", (items->capacity - items->size));
  }
	fprintf(fp, "]");
  //fprintf(fp, "size:%d, capacity:%d", items->size, items->capacity);
  return;
}

int debugDA(DA *items,int level) {
  int prevFlag = items->debugFlag;
  items->debugFlag = level;
  return prevFlag;
}
void freeDA(DA *items) {
  for(int i = 0; i < items->size; i++) {
    items->free(items->arr[i]);
  }
  free(items->arr);
  free(items);
}

/*************** end-of-file *************/
