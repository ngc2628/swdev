
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

struct mmat {
  int rows,cols;
  double **matrix;
};

extern int mmatalloc(struct mmat *,int,int);
extern int mmatfree(struct mmat *);
extern double mmatget(struct mmat *,int,int);
extern double mmatset(struct mmat *,int,int,double);