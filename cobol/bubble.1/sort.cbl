     
       IDENTIFICATION DIVISION.
         PROGRAM-ID. sort.
       
       ENVIRONMENT DIVISION.
         INPUT-OUTPUT SECTION.       
       
       DATA DIVISION.
         FILE SECTION.
         WORKING-STORAGE SECTION.
         LOCAL-STORAGE SECTION.
           01 v-ws.
             05 arg pic x(256) value spaces.
             05 ii pic 99 value 1.
             05 ii-1 pic 99 value 1.
             05 jj pic 99 value 1.
             05 jj-1 pic 99 value 1.
             05 cmpidx pic 99 value 1.
             05 idx1 pic 99 value 0.
             05 idx2 pic 99 value 0.
             05 int-table-sz pic 99 value 20.
             05 int-table-sz-1 pic 99 value 0.
             05 int-table pic s999 occurs 20. 
             05 int-table-tmp pic s999 value 0.
                
       PROCEDURE DIVISION.
       a00-start section.
         display 'start1'
         MOVE SPACES TO arg 
         ACCEPT arg FROM ARGUMENT-VALUE
         IF NOT arg=SPACES then
           DISPLAY 'arg ' arg
         end-if 
         display 'start2'
         move 89 to int-table(1)
         move 18 to int-table(2)
         move 77 to int-table(3)
         move 44 to int-table(4)
         move 55 to int-table(5)
         move 33 to int-table(6)
         move 66 to int-table(7)
         move 28 to int-table(8)
         move 11 to int-table(9)
         move 99 to int-table(10)
         move 40 to int-table(11)
         move 12 to int-table(12)
         move 87 to int-table(13)
         move 20 to int-table(14)
         move 38 to int-table(15)
         move 48 to int-table(16)
         move 78 to int-table(17)
         move 68 to int-table(18)
         move 58 to int-table(19)
         move 23 to int-table(20)
         display 'start3'
         perform b01-bubble
         display 'start4'
         .
       a99-end. 
         STOP RUN
         .
       b00-bubble section.   
         compute ii = int-table-sz
         perform until ii = 2
           move 1 to jj
           compute int-table-sz-1 = ii - 1
           perform until jj > int-table-sz-1
             compute jj-1 = jj + 1
             if int-table(jj) > int-table(jj-1) then
               move int-table(jj) to int-table-tmp
               move int-table(jj-1) to int-table(jj)
               move int-table-tmp to int-table(jj-1)
             end-if
             compute jj = jj + 1
           end-perform
           compute ii = ii - 1
         end-perform
         perform d00-dsp-table
       .
       b01-bubble section.
         move 1 to ii
         perform until int-table-sz < ii  
           compute jj = ii + 1
           perform until int-table-sz < jj
             if int-table(jj) < int-table(ii) then
               move ii to idx1
               move jj to idx2
               perform c00-swap
             end-if
             add 1 to jj
           end-perform
           display 'loop #',ii
           perform d00-dsp-table
           display " "
           add 1 to ii
         end-perform
         display 'result'    
         perform d00-dsp-table
       .
       c00-swap section.
         move int-table(idx1) to int-table-tmp
         move int-table(idx2) to int-table(idx1)
         move int-table-tmp to int-table(idx2)  
       .
       d00-dsp-table section.
         move 1 to idx1
         perform until int-table-sz < idx1
           display idx1,' ',int-table(idx1)
           add 1 to idx1
         end-perform
       .  


