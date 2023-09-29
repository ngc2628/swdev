     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. caller.
       
       ENVIRONMENT DIVISION.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
         01 v-ws.
           05 ii PIC 9(4).
           05 jj PIC 9(4).
           05 idx PIC 9(4).
           05 table-data-4-3.
             COPY coor3d.
           
       PROCEDURE DIVISION.
       A-main SECTION.
         A-para-1.
           PERFORM B-init-table
           CALL 'callee' USING table-data-4-3
           STOP RUN
         .
       
       B-init-table SECTION.
         B-para-1.
           PERFORM VARYING ii FROM 1 BY 1 UNTIL ii=5
             STRING
               'row#'
               ii DELIMITED BY SIZE
               INTO r-header(ii)
             END-STRING
             PERFORM VARYING jj FROM 1 BY 1 UNTIL jj=4
               COMPUTE idx=3 * (ii - 1) + (jj - 1)
               COMPUTE t-val(ii,jj)=idx
             END-PERFORM
           END-PERFORM
         .
