     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. sub1.

       DATA DIVISION.
       LINKAGE SECTION.
         01 person-data.
           COPY nameid.

       PROCEDURE DIVISION USING person-data.
       A-main SECTION.
         A-para-1.
           DISPLAY 'call sub1'
           MOVE 11 TO m_id
           EXIT PROGRAM
         .      
