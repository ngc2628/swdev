     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. sub1.

       DATA DIVISION.
       LINKAGE SECTION.
         01 p_id PIC 9(4).
         01 p_name PIC A(15).

       PROCEDURE DIVISION USING p_id, p_name.
       A-main SECTION.
         A-para-1.
           DISPLAY 'call sub1'
           MOVE 11 TO p_id
           EXIT PROGRAM
         .
             
