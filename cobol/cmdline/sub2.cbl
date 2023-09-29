
      * sub2
       IDENTIFICATION DIVISION.
       PROGRAM-ID. sub2.
       
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       LOCAL-STORAGE SECTION.
         01  m_cnt PIC 9(2) value 0.
       LINKAGE SECTION.
         01 person-data.
           COPY nameid.

       PROCEDURE DIVISION USING person-data.
       A-main SECTION.
         A-para-1.
           DISPLAY 'call sub2'
      *     PERFORM LOOP1
      *     PERFORM LOOP2
           MOVE 3 TO m_cnt
           PERFORM UNTIL m_cnt=0 
             COMPUTE m_cnt = m_cnt - 1
             DISPLAY m_cnt
           END-PERFORM
           PERFORM 3 TIMES
             DISPLAY m_cnt
             COMPUTE m_cnt = m_cnt + 1
           END-PERFORM
           EXIT PROGRAM
         . 

       B-loop1 SECTION.
         B-para-1.
           DISPLAY 'ko'. 

       C-loop1 SECTION.
         C-para-1.
           DISPLAY 'ok'.


        
