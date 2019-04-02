#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "integer.h"
#include "da.h"

void srandom(unsigned int);
long int random(void);

static void
display(DA *p,char *intro)
    {
    printf("%s",intro);
    if (sizeDA(p) < 200)
        {
        displayDA(p,stdout);
        }
    else
        {
        int k = 100;
        int j;
        printf("(displaying first and last %d elements)\n",k);
        for (j = 0; j < k; ++j)
            {
            displayINTEGER(getDA(p,j),stdout);
            printf(" ");
            }
        printf("... ");
        for (j = 0; j < k; ++j)
            {
            displayINTEGER(getDA(p,sizeDA(p)-k+j),stdout);
            if (j < k - 1) printf(" ");
            }
        printf("\n");
        }
    printf("\n");
    }

int
main(void)
    {
    srandom(14);
    int i,r,rounds=2,size=1994;
    printf("INTEGER test of DA, unions\n");
    printf("test size is %d\n",size);
    DA *p = newDA();
    setDAdisplay(p,displayINTEGER);
    setDAfree(p,freeINTEGER);
    debugDA(p,1);
    DA *q = newDA();
    setDAdisplay(q,displayINTEGER);
    setDAfree(q,freeINTEGER);
    debugDA(q,1);
    display(p,"p items: ");
    display(q,"p items: ");
    for (r = 0; r < rounds; ++r)
        {
        int j,k;
        for (i = 0; i < size; ++i)
            {
            j = random() % size;
            k = random() % (sizeDA(p) + 1);
            insertDA(p,k,newINTEGER(j));
            j = random() % size;
            k = random() % (sizeDA(q) + 1);
            insertDA(q,k,newINTEGER(j));
            }
        for (i = 0; i < size - random() % size / 2; ++i)
            {
            unionDA(p,q);
            unionDA(p,q);
            }
        display(p,"p items: ");
        display(q,"q items: ");
        printf("p size is %d\n",sizeDA(p));
        printf("q size is %d\n",sizeDA(q));
        }
    freeDA(p);
    freeDA(q);
    return 0;
    }
