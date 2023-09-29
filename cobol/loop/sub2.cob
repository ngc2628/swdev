
      * sub2
       IDENTIFICATION DIVISION.
       PROGRAM-ID. sub2.
       
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       LOCAL-STORAGE SECTION.
         01 v-loc.
           05 m_cnt PIC 9(2) value 0.
       LINKAGE SECTION.
         01 v-lnk.
           05 p_id PIC 9(4).
           05 p_name PIC A(15).

       PROCEDURE DIVISION USING v-lnk.

       A-main SECTION.
         A-para-1.
           DISPLAY 'call sub2'
           PERFORM B-loop1
           PERFORM C-loop2
           MOVE p_id TO m_cnt
           PERFORM UNTIL m_cnt=0 
             COMPUTE m_cnt = m_cnt - 1
             DISPLAY m_cnt
           END-PERFORM
           PERFORM 7 TIMES
             DISPLAY m_cnt
             COMPUTE m_cnt = m_cnt + 1
           END-PERFORM
           EXIT PROGRAM
         . 

       B-loop1 SECTION.
         B-para-1.
           DISPLAY 'ko'. 

       C-loop2 SECTION.
         C-para-1.
           DISPLAY 'ok'.


        
