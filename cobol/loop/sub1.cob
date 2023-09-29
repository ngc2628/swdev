     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. sub1.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
       LOCAL-STORAGE SECTION.
         01 v-loc.
           05 ii PIC 9(32).
       LINKAGE SECTION.
         01 v-lnk.
           05 p_id PIC 9(4) VALUE 0.
           05 p_name PIC A(15).

       PROCEDURE DIVISION USING v-lnk.
       A-main SECTION.
         A-para-1.
           DISPLAY 'call sub1'
           MOVE 11 TO p_id
           COMPUTE ii = ii + 1
           DISPLAY ii
           EXIT PROGRAM
         .      
