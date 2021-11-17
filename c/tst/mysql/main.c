
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>

int bailout(int ln,const char *msg) {

  printf ("%d : %s\n",ln,msg);
  return 0;

}

int main(int argc,char **argv) {

  MYSQL *conn=mysql_init(0);

  if (!conn)
    return bailout(__LINE__,mysql_error(conn));
  
  conn=mysql_real_connect(conn,"hal9004","siberia","tiger",0,0,0,0); 
  
  if (!conn)
    return bailout(__LINE__,mysql_error(conn));

  int ii=0,jj=0,nrows=0,nfields=0,queryko=0;
  double zero=.0,one=1.;
  char query[256];
  memset(&query[0],0,256);
  MYSQL_RES *sqlres=0;
  MYSQL_ROW sqlrow;
  
  strcpy(query,"use mk;");
  queryko=mysql_query(conn,query);
  if (queryko)
    return bailout(__LINE__,mysql_error(conn));
  
  strcpy(query,"show tables;");
  queryko=mysql_query(conn,query);
  if (queryko)
    return bailout(__LINE__,mysql_error(conn));
  sqlres=mysql_store_result(conn);
  if (!sqlres)
    return bailout(__LINE__,mysql_error(conn));
  nrows=sqlres->row_count;
  if (nrows==0) {
    mysql_free_result(sqlres);
    return bailout(__LINE__,mysql_error(conn));
  }
  nfields=mysql_num_fields(sqlres);
  for (ii=0;ii<nrows;ii++) {
    sqlrow=mysql_fetch_row(sqlres);
    for (jj=0;jj<nfields;jj++)
      printf("[%d,%d] : [%s]\n",ii,jj,sqlrow[jj]);
  }
  mysql_free_result(sqlres);

  strcpy(query,"select nickname,comment from name;");
  queryko=mysql_query(conn,query);
  if (queryko)
    return bailout(__LINE__,mysql_error(conn));
  sqlres=mysql_store_result(conn);
  if (!sqlres)
    return bailout(__LINE__,mysql_error(conn));
  nrows=sqlres->row_count;
  if (nrows==0) {
    mysql_free_result(sqlres);
    return bailout(__LINE__,mysql_error(conn));
  }
  nfields=mysql_num_fields(sqlres);
  for (ii=0;ii<nrows;ii++) {
    sqlrow=mysql_fetch_row(sqlres);
    for (jj=0;jj<nfields;jj++)
      printf("[%d,%d] : [%s]\n",ii,jj,sqlrow[jj]);
  }
  mysql_free_result(sqlres);

  mysql_close(conn);

  return 0;

}

