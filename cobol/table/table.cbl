     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. table.
       
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT tableinp ASSIGN TO fname
           ORGANIZATION IS LINE SEQUENTIAL.
       
       DATA DIVISION.
       FILE SECTION.
       FD tableinp.
         01 table-record PIC A(255) VALUE SPACES.
       WORKING-STORAGE SECTION.
         01 v-ws.
           05 fname PIC A(128) VALUE SPACES.
           05 iseof PIC 9 VALUE 0.
           05 cntrec PIC 9(6) VALUE 0.
           05 ii PIC 9(2).
           05 jj PIC 9(2).
           05 kk PIC 9(2).
           05 ll PIC 9(2).
           05 bufptr USAGE IS POINTER.
           05 number53.
             COPY num53.
           05 datetime-data.
             COPY datetimedata.
           05 table-data-3d.
             COPY tabledata.

       PROCEDURE DIVISION.
       A-main SECTION.
         A-para-1.
           CALL 'now-str' USING datetime-data  
           DISPLAY datetime-str " " LENGTH datetime-data " " 
             LENGTH m_year
           ACCEPT fname FROM ARGUMENT-VALUE
             ON EXCEPTION DISPLAY 'no filename provided'       
           END-ACCEPT
           IF NOT fname = SPACES THEN
             DISPLAY 'fname ', fname
           END-IF
           ALLOCATE 16 CHARACTERS RETURNING bufptr
      *     PERFORM B-preinit-table
      *     INITIALIZE xyz-table-data REPLACING ALPHANUMERIC DATA 
      *       BY 'XXXXX' NUMERIC DATA BY 1.
      *     PERFORM get-table
           PERFORM C-init-table
           PERFORM D-display-table
         .
         
         A-para-2.
           FREE bufptr
           STOP RUN
         .

       B-preinit-table SECTION.
         B-para-1.
           PERFORM VARYING ii FROM 1 BY 1 UNTIL ii=5
             COMPUTE kk = ii + 4
             STRING
               kk DELIMITED BY SIZE
               ':00'
               INTO r-header(ii)
             END-STRING 
             PERFORM VARYING jj FROM 1 BY 1 UNTIL jj=4
               COMPUTE t-val(ii,jj) = 1.35 * ( jj + ( ii * kk ) )
             END-PERFORM 
           END-PERFORM
           GO TO b-para-2
         .
         B-para-2.
           EXIT
         .

       C-init-table SECTION.
         C-para-1.
           IF fname = SPACES THEN
             GO TO C-para-2
           END-IF
           OPEN INPUT tableinp
           PERFORM UNTIL iseof=1
             READ tableinp INTO table-record
             AT END
               MOVE 1 TO iseof
             NOT AT END
               IF table-record = SPACES THEN
                 CONTINUE 
               END-IF
      *         DISPLAY FUNCTION TRIM(table-record)
               IF cntrec = 0 THEN
                 UNSTRING table-record DELIMITED BY ALL SPACES INTO
                   c-header(1),c-header(1),c-header(2),c-header(3)           
                 END-UNSTRING
               ELSE
                 UNSTRING table-record DELIMITED BY ALL SPACES INTO
                   r-header(cntrec)
                   t-val(cntrec,1),t-val(cntrec,2),t-val(cntrec,3)
                 END-UNSTRING
               END-IF
               ADD 1 TO cntrec
             END-READ
           END-PERFORM
           CLOSE tableinp 
           GO TO C-para-2
         .

         C-para-2.
           EXIT 
         .

       D-display-table SECTION.
         D-para-1.
           DISPLAY 
             '        ',c-header(1),' ',c-header(2),' ',c-header(3)
           PERFORM VARYING ii FROM 1 BY 1 UNTIL ii=6
             DISPLAY FUNCTION TRIM(r-header(ii)) NO ADVANCING
             DISPLAY '  ' NO ADVANCING
             PERFORM VARYING jj FROM 1 BY 1 UNTIL jj=4
               MOVE t-val(ii,jj) TO float53
               CALL 'trim-float53' USING number53
               DISPLAY FUNCTION TRIM(numstr53) NO ADVANCING
               DISPLAY '  ' NO ADVANCING
             END-PERFORM 
             DISPLAY ' '
           END-PERFORM
           DISPLAY ' '
           DISPLAY '<', bufptr, '>'
           GO TO D-para-2
         .
         D-para-2.
           EXIT
         .

      * cobc -v -x sub1.cob chk11.cob
