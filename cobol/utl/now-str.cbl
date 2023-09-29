     
       IDENTIFICATION DIVISION.
         PROGRAM-ID. now-str IS INITIAL.
       
       ENVIRONMENT DIVISION.
       
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       LOCAL-STORAGE SECTION.

       LINKAGE SECTION.
         01 datetime-data.
           COPY 'datetimedata'.

       PROCEDURE DIVISION USING datetime-data.

       A-main SECTION.
         A-para-1. 
           MOVE FUNCTION CURRENT-DATE TO datetime-data
           STRING 
             m_year2 DELIMITED BY SIZE
             "-"
             m_month DELIMITED BY SIZE
             "-"
             m_day DELIMITED BY SIZE
             " "
             m_hour DELIMITED BY SIZE
             ":"
             m_minute DELIMITED BY SIZE
             ":"
             m_second DELIMITED BY SIZE
             INTO datetime-str
           EXIT PROGRAM
         .

