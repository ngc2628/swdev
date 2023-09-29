     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. loop.
       
       ENVIRONMENT DIVISION.
       
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       LOCAL-STORAGE SECTION.
         01 v-local.
           05  m_id  PIC 9(4) value 1841.
           05  m_name  PIC A(15) value 'tiger'.
       
       PROCEDURE DIVISION.
       A-main SECTION.
         A-para-1.
           DISPLAY "id, name " m_id, " ", m_name
           CALL 'sub1' USING v-local
           DISPLAY "id, name " m_id, " ", m_name
           CALL 'sub2' USING v-local
           STOP RUN.
         .

      * cobc -v -x sub1.cob chk11.cob
