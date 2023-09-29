     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. callee.
       
       ENVIRONMENT DIVISION.         
       DATA DIVISION.
       WORKING-STORAGE SECTION.
         01 ii PIC 9(4).
         01 jj PIC 9(4).
           
       LINKAGE SECTION.
         01 v-ws.
           05 table-data-4-3.
             COPY coor3d.

       PROCEDURE DIVISION USING v-ws.
       A-main SECTION.
         A-para-1.
           PERFORM B-display-table
           STOP RUN
         .
   
       B-display-table SECTION.
         B-para-1.
           PERFORM VARYING ii FROM 1 BY 1 UNTIL ii=4
             display 'col#',ii NO ADVANCING
           END-PERFORM  
           DISPLAY ' '
           PERFORM VARYING ii FROM 1 BY 1 UNTIL ii=5
             DISPLAY r-header(ii) NO ADVANCING
             PERFORM VARYING jj FROM 1 BY 1 UNTIL jj=4
               DISPLAY t-val(ii,jj), ' ' NO ADVANCING
             END-PERFORM
             DISPLAY ' '
           END-PERFORM
         .

      * cobc -v -x sub1.cob chk11.cob
