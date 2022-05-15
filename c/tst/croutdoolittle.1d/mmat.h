
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>

struct mmat {
  int rows,cols;
  double *matrix;
};

int ludecomposition(int,const struct mmat *,struct mmat *,int *,double *);
int lubacksubstitution(int,struct mmat *,int *,double *,double *);
int mmatalloc(struct mmat *,int,int);
int mmatfree(struct mmat *);
double mmatget(const struct mmat *,int,int);
double mmatset(struct mmat *,int,int,double);
int mmatreset(struct mmat *,int);
int mmatcopy(struct mmat *,const struct mmat *);
int mmattranspose(struct mmat *);
int mmatmult(struct mmat *,const struct mmat *);
double mmatdet(struct mmat *);
int mmatinvert(struct mmat *);