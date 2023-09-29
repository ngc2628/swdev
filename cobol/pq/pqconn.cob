
       IDENTIFICATION DIVISION.
       PROGRAM-ID. pqconn.

       ENVIRONMENT DIVISION.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
         01 pgconn USAGE IS POINTER VALUE NULL.
         01 pgres USAGE IS POINTER VALUE NULL.
         01 pgtxt USAGE IS POINTER VALUE NULL.
         01 pgstate PIC S9(32) VALUE 0.
         
         01 v-ws.
           05 sspace PIC X(1) VALUE SPACE.
           05 sbleft PIC X(1) VALUE "[".
           05 sbright PIC X(1) VALUE "]".
           05 buf PIC X(1024) VALUE SPACES.
           77 bufp PIC X(1024) BASED VALUE SPACES.

           

       LOCAL-STORAGE SECTION.
         01 v-ls.
           05 nrows PIC 999 VALUE 0.
           05 ncols PIC 999 VALUE 0.
           05 ii PIC 999 VALUE 0.
           05 jj PIC 999 VALUE 0.
           05 lenval PIC 9999 VALUE 0.
           05 fres PIC S9(32)V9(6) VALUE 0.0 USAGE IS COMP-3.

       PROCEDURE DIVISION.
       A-main SECTION.
         A-para-1.
           PERFORM B-printconn.
           CALL 'connectionstring' USING BY VALUE pgstate
             BY REFERENCE buf.
           DISPLAY FUNCTION TRIM(buf) UPON SYSOUT.

      *     ALLOCATE 1024 CHARACTERS INITIALIZED RETURNING pgtxt.
      *     SET ADDRESS OF bufp TO pgtxt.

           CALL 'PQconnectdb' USING BY REFERENCE buf
             RETURNING pgconn.
           PERFORM B-printconn.
           CALL 'PQstatus' USING BY VALUE pgconn
             RETURNING pgstate.
           PERFORM C-printstatenl.
                    
           ACCEPT OMITTED.

           INITIALIZE buf.
           STRING
             'select * from tel' DELIMITED BY SIZE
             INTO buf
           END-STRING.
           CALL 'PQexec' USING BY VALUE pgconn, BY REFERENCE buf
             RETURNING pgres.
      *     CALL 'PQstatus' USING BY VALUE pgconn
      *       RETURNING pgstate.
      *     PERFORM printstatenl.
           
           CALL 'PQdb' USING BY VALUE pgconn
             RETURNING pgtxt.
      *     CALL 'PQstatus' USING BY VALUE pgconn
      *       RETURNING pgstate.
      *     PERFORM printstatenl.
           CALL 'collectword' USING BY CONTENT pgstate
             BY REFERENCE buf BY REFERENCE pgtxt.
           CALL 'sparenthesize' USING BY CONTENT pgstate
             BY REFERENCE buf.
           DISPLAY 'database ', FUNCTION TRIM(buf) UPON SYSOUT.
                      
           INITIALIZE buf.
           STRING
             'select * from tel' DELIMITED BY SIZE INTO buf
           END-STRING.
           CALL 'PQexec' USING BY VALUE pgconn, BY REFERENCE buf
             RETURNING pgres.

           CALL 'PQresultStatus' USING BY VALUE pgres
             RETURNING pgstate.
           PERFORM D-printstate.
           CALL 'PQresStatus' USING BY VALUE pgstate
             RETURNING pgtxt.
           CALL 'collectword' USING BY CONTENT pgstate
             BY REFERENCE buf BY REFERENCE pgtxt.
           CALL 'sparenthesize' USING BY CONTENT pgstate
             BY REFERENCE buf.
           DISPLAY FUNCTION TRIM(buf) UPON SYSOUT.
           
           CALL 'PQntuples' USING BY VALUE pgres RETURNING nrows.
           CALL 'PQnfields' USING BY VALUE pgres RETURNING ncols.
           PERFORM UNTIL ii=nrows
             MOVE 0 TO jj
             PERFORM UNTIL jj=ncols
               CALL 'PQgetvalue' USING BY VALUE pgres BY VALUE ii
                 BY VALUE jj RETURNING pgtxt
               CALL 'collectword' USING BY CONTENT pgstate
                 BY REFERENCE buf BY REFERENCE pgtxt
               CALL 'sparenthesize' USING BY CONTENT pgstate
                 BY REFERENCE buf
               DISPLAY FUNCTION TRIM(buf), sspace NO ADVANCING
               COMPUTE jj = jj + 1
             END-PERFORM
             DISPLAY sspace
             COMPUTE ii = ii + 1
           END-PERFORM.

           CALL 'PQclear' USING BY VALUE pgres.
           CALL 'PQfinish' USING BY VALUE pgconn.
           GOBACK
         .

       B-printconn SECTION.
         B-para-1.
           INITIALIZE buf.
           STRING
             "connection is" DELIMITED BY SIZE
             sspace DELIMITED BY SIZE
             sbleft DELIMITED BY SIZE
             INTO buf
           END-STRING.
           DISPLAY FUNCTION TRIM(buf) WITH NO ADVANCING.
           INITIALIZE buf.
           DISPLAY pgconn WITH NO ADVANCING.
           STRING
             sbright DELIMITED BY SIZE
             INTO buf
           END-STRING.
           DISPLAY FUNCTION TRIM(buf)
         .

       C-printstatenl SECTION.
         C-para-1.
           PERFORM D-printstate.
           DISPLAY ' '
         .

       D-printstate SECTION.
         D-para-1.
           INITIALIZE buf.
           CALL 'iparenthesize' USING BY CONTENT pgstate
             BY REFERENCE buf.
           DISPLAY 'state ', FUNCTION TRIM(buf) NO ADVANCING
             UPON SYSOUT
         .







