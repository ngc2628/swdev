     
       IDENTIFICATION DIVISION.
         PROGRAM-ID. num53 IS INITIAL.
       
       ENVIRONMENT DIVISION.
       
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       LOCAL-STORAGE SECTION.
         01 v-loc.
           05 strbuf PIC X(32) VALUE SPACES.
           05 strbeg PIC 9(2) VALUE 1.
           05 strend PIC 9(2) VALUE 32.
           05 strlen PIC 9(2) VALUE 0.
           05 strdec PIC 9(2) VALUE 0.
           05 ii PIC 9(2) VALUE 0.
           05 charcode PIC 9(3) VALUE 0.
           05 chkcharcode PIC 9(3) VALUE 0.
           05 doloop PIC 9 VALUE 0.
           
       LINKAGE SECTION.
         01 number53.
           COPY 'num53.cpy'.
           
       PROCEDURE DIVISION USING number53.
         
       A-main SECTION. 
         A-para-1.
           DISPLAY 'num53'
           GOBACK
         .

         ENTRY 'trim-int53' USING number53.
           MOVE SPACES TO numstr53
           STRING int53 DELIMITED BY SIZE INTO strbuf
           PERFORM B-numstr_attr
           MOVE strbuf(strbeg:strlen) TO numstr53
           GOBACK
         .

         ENTRY 'trim-float53' USING number53.
           MOVE SPACES TO numstr53
           STRING float53 DELIMITED BY SIZE INTO strbuf
           PERFORM B-numstr_attr 
           MOVE strbuf(strbeg:strlen) TO numstr53
           GOBACK
         .

         ENTRY 'strlen-num53' USING number53.
           MOVE FUNCTION LENGTH(FUNCTION TRIM(numstr53)) TO int53
           MOVE numstr53(1:int53) TO numstr53
           GOBACK RETURNING int53
         .  

       B-numstr_attr SECTION.
         B-para-1.
           MOVE 1 TO strbeg
           MOVE 1 TO doloop
           PERFORM VARYING ii FROM 1 BY 1 UNTIL ii=32 OR doloop=0
             MOVE FUNCTION ORD(strbuf(ii:1)) TO charcode
             IF charcode>49 AND charcode<59 OR charcode=47
               MOVE 0 TO doloop
             ELSE
               ADD 1 TO strbeg
             END-IF
           END-PERFORM
           MOVE 1 TO doloop
           PERFORM VARYING ii FROM strbeg BY 1 UNTIL ii=32 OR doloop=0
             MOVE FUNCTION ORD(strbuf(ii:1)) TO charcode
             IF charcode=47
               MOVE ii to strdec
               MOVE 0 TO doloop
             END-IF
           END-PERFORM
           MOVE 32 TO strend
           MOVE 1 TO doloop
           PERFORM VARYING ii FROM 32 BY -1 UNTIL ii=1 OR doloop=0
             MOVE FUNCTION ORD(strbuf(ii:1)) TO charcode
             IF strdec=0
               MOVE 48 TO chkcharcode
             ELSE
               MOVE 49 TO chkcharcode
             END-IF
             IF charcode>chkcharcode AND charcode<59 OR charcode=47
               MOVE 0 TO doloop
             ELSE
               SUBTRACT 1 FROM strend
             END-IF
           END-PERFORM
           COMPUTE strlen = 1 + strend - strbeg
         .















