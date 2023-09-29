     
       IDENTIFICATION DIVISION.
       PROGRAM-ID. cmdline.
       
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.       
       
       DATA DIVISION.
       FILE SECTION.
       WORKING-STORAGE SECTION.
       LOCAL-STORAGE SECTION.
         01 v-ws.
           COPY nameid.
           10 arg PIC X(32) VALUE SPACES.
                
       PROCEDURE DIVISION.
       A-main SECTION.
         A-para-1.
           DISPLAY "id, name " m_id, " ", m_name
           CALL 'sub1' USING m_id, m_name
           DISPLAY "id, name " m_id, " ", m_name
           CALL 'sub2' USING m_id, m_name
           PERFORM B-arg WITH TEST AFTER UNTIL arg=SPACES
           STOP RUN
         .
       
       B-ARG SECTION.
         B-para-1.
           MOVE SPACES TO arg 
           ACCEPT arg FROM ARGUMENT-VALUE
           IF NOT arg=SPACES DISPLAY 'arg ' arg 
         .

      

           

