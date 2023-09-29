     
       identification division.
         program-id. sort.
       
       environment division.
         input-output section.
         file-control.
           select tableinp assign to fname
           organization is line sequential.      
       
       data division.
         file section.
         fd tableinp.
           01 table-record pic a(255) value spaces.
         working-storage section.
         local-storage section.
           01 v-ws.
             05 fname pic a(128) value spaces.
             05 iseof pic 9 value 0.
             05 cntrec pic 9(6) value 0.
             05 ii pic 99 value 1.
             05 jj pic 99 value 1.
             05 cmpres pic S9 value 0.
             05 idx1 pic 99 value 0.
             05 idx2 pic 99 value 0.
             05 t-val-tmp pic 9(16).9(15) value 0.0.
             05 number53.
               COPY num53.
             05 table-data-1d.
               copy coor1d.
                             
       procedure division.
       a00-start section.
         accept fname from argument-value
         if not fname=spaces then
           display 'using file  ', fname
         else
           go to a99-end
         end-if
         perform f00-table-length
         perform g00-read-file
         perform b00-bubble
         perform e00-dsp-table
         .
       a99-end. 
         stop run
       .

       b00-bubble section.
         move 1 to ii
         perform until t-length < ii  
           tmpobj=t-val(ii)
           compute jj = ii - 1
           perform until jj <= 0
             move ii to idx1
             move jj to idx2
             perform c00-cmp
             if cmpres < 1 then
               perform d00-swap
             end-if
             add 1 to jj
           end-perform
           add 1 to ii
         end-perform
       .

       b99-exit.
         exit
       .

       c00-cmp section.
         if t-val(idx1) < t-val(idx2) then
           move -1 to cmpres
         else if t-val(idx2) < t-val(idx1) then
           move 1 to cmpres
         else
           move 0 to cmpres
         end-if
       .

       c99-exit.
         exit
       .

       d00-swap section.
         move t-val(idx1) to t-val-tmp
         move t-val(idx2) to t-val(idx1)
         move t-val-tmp to t-val(idx2)
       .

       d99-exit.
         exit
       .

       e00-dsp-table section.
         move 1 to idx1
         perform until t-length < idx1
           move t-val(idx1) to float53
           call 'trim-float53' using number53
           display idx1,' ',function trim(numstr53)
           add 1 to idx1
         end-perform
       .  

       e99-exit.
         exit
       .

       f00-table-length section.
         move 0 to cntrec
         open input tableinp
         perform until iseof=1
           read tableinp into table-record
           at end
             move 1 to iseof
           not at end
             if not table-record = spaces then
               add 1 to cntrec
             end-if
           end-read
         end-perform
         close tableinp
         subtract 1 from cntrec
         move cntrec to t-length
       . 

       f99-exit.
         exit
       . 

       g00-read-file section.
         move 0 to cntrec
         move 0 to iseof
         open input tableinp
         perform until iseof=1 or cntrec > t-length
           read tableinp into table-record
           at end
             move 1 to iseof
           not at end
             if table-record = spaces then
               continue 
             end-if
             if cntrec = 0 then
               move table-record to c-header
             else
               unstring table-record delimited by all spaces into
                 r-header(cntrec),t-val(cntrec)
               end-unstring
             end-if
             add 1 to cntrec
           end-read
         end-perform
         close tableinp
       .

       g99-exit.
         exit
       .

