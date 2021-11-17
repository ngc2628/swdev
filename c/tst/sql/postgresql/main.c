
#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

int main(int argc, char **argv) {

  char *pgconninfo="host=hal9002.ssolk.net port=5432 user=siberia password=tiger dbname=mk ";
  PGconn *pgconn=0;
  PGresult *pgres=0;
  char *pgstatc=0;
  int nfields=0,ii=0,jj=0;
    
  /* If the user supplies a parameter on the command line, use it as the
   * conninfo string; otherwise default to setting dbname=postgres and using
   * environment variables or defaults for all other connection parameters.
  */
  if (argc > 1)
    pgconninfo=argv[1];
  
  /* Make a connection to the database */
  pgconn=PQconnectdb(pgconninfo);

  /*
   * Our test case here involves using a cursor, for which we must be inside
   * a transaction block.  We could do the whole thing with a single
   * PQexec() of "select * from pg_database", but that's too trivial to make
   * a good example.
  */

  pgres=PQexec(pgconn,"select * from tel;");
  printf("[%p]\n",pgres);
  PQclear(pgres);
  PQfinish(pgconn);
  return 0;



  /* Start a transaction block */
  pgres=PQexec(pgconn,"BEGIN");
  
  /*
   * Should PQclear PGresult whenever it is no longer needed to avoid memory
   * leaks
  */
  PQclear(pgres);

  /*
   * Fetch rows from pg_database, the system catalog of databases
  */
  pgres=PQexec(pgconn,"DECLARE myportal CURSOR FOR select * from pg_database");
  PQclear(pgres);

  pgres=PQexec(pgconn,"FETCH ALL in myportal");
  
  /* first, print out the attribute names */
  nfields=PQnfields(pgres);
  for (ii=0;ii<nfields;ii++)
    printf("%-15s", PQfname(pgres,ii));
  printf("\n\n");

  /* next, print out the rows */
  for (ii=0;ii<PQntuples(pgres);ii++) {
    for (jj=0;jj<nfields;jj++)
      printf("%-15s", PQgetvalue(pgres,ii,jj));
    printf("\n");
  }

  PQclear(pgres);

  /* close the portal ... we don't bother to check for errors ... */
  pgres=PQexec(pgconn,"CLOSE myportal");
  PQclear(pgres);

  /* end the transaction */
  pgres=PQexec(pgconn,"END");
  PQclear(pgres);

  /* close the connection to the database and cleanup */
  PQfinish(pgconn);

  return 0;
}