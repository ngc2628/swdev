
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

struct mmat {
  int rows,cols;
  double *matrix;
};

int mmatalloc(struct mmat *,int,int);
int mmatfree(struct mmat *);
double mmatget(struct mmat *,int,int);
double mmatset(struct mmat *,int,int,double);