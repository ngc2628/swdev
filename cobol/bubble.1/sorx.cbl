     
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
             05 jj pic 99 value 1.
             05 cmpidx pic 99 value 1.
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
           perform b00-bubble
           display 'start4'
           .
       a99-end section. 
           STOP RUN
           .
       b00-bubble section.   
           perform varying ii from 1 by 1 until 
             ii is greater than int-table-sz
             compute int-table-sz-1=int-table-sz - ii
             perform varying jj from 1 by 1 until 
               jj is greater than int-table-sz-1
               compute cmpidx=jj + 1
               if (int-table(jj) > int-table(cmpidx))
                 move int-table(jj) to int-table-tmp
                 move int-table(cmpidx) to int-table(jj)
                 move int-table-tmp to int-table(cmpidx)
               end-if
             end-perform
           end-perform. 
           move 1 to ii
           perform until ii > int-table-sz
             display ii,' ',int-table(ii)
             add 1 to ii
           end-perform
           .

      

           

