
      * printconsole
       IDENTIFICATION DIVISION.
       PROGRAM-ID. strings.
       
       DATA DIVISION.
       WORKING-STORAGE SECTION.
         01 v-ws.
           05 sspace PIC X(1) VALUE SPACE.
           05 sbleft PIC X(1) VALUE "[".
           05 sbright PIC X(1) VALUE "]".
           05 sbuf PIC X(1024) VALUE NULL.

           05 pgkeywords.
             10 pghost PIC X(16) VALUE "host=".
             10 pghostaddr PIC X(16) VALUE "hostaddr=".
             10 pgport PIC X(16) VALUE "port=".
             10 pguser PIC X(16) VALUE "user=".
             10 pgpassword PIC X(16) VALUE "password=".  
             10 pgdatabase PIC X(16) VALUE "dbname=".
           05 pgconnwords.
             10 pghost PIC X(32) VALUE "hal9006.fuzzball.dum".
             10 pghostaddr PIC X(16).
             10 pgport PIC X(8) VALUE "5432".
             10 pguser PIC X(32) VALUE "siberia".
             10 pgpassword PIC X(16) VALUE "tiger".
             10 pgdatabase PIC X(32) VALUE "mk".  

         LOCAL-STORAGE SECTION.
           
         LINKAGE SECTION.
         01 pgtxt USAGE IS POINTER VALUE NULL.
         01 pgstr PIC X(1024) VALUE SPACES.
         01 pgstate PIC S9(32) VALUE 0.
         01 bufp PIC X(1024) BASED VALUE SPACES.

       PROCEDURE DIVISION USING pgstate, pgstr, pgtxt.
       A-main SECTION.
         A-para-1. 
           DISPLAY 'strings'.
           GOBACK.
         
         ENTRY 'collectword' USING pgstate, pgstr, pgtxt.
      *     DISPLAY 'collect next word from answer'.
           SET ADDRESS OF bufp TO pgtxt.
           INITIALIZE pgstr.
           STRING bufp DELIMITED BY x'00' INTO pgstr.
           GOBACK.

         ENTRY 'iparenthesize' USING pgstate, pgstr.
      *     DISPLAY 'parenthesize string'.
           INITIALIZE pgstr.
           STRING 
             sbleft DELIMITED BY SIZE
             pgstate
             sbright DELIMITED BY SIZE
             INTO pgstr
           END-STRING. 
           GOBACK.

         ENTRY 'sparenthesize' USING pgstate, pgstr.
      *     DISPLAY 'parenthesize string'.
           INITIALIZE sbuf.
           STRING FUNCTION TRIM(pgstr) INTO sbuf.
           INITIALIZE pgstr.
           STRING 
             sbleft DELIMITED BY SIZE
             sbuf DELIMITED BY SPACE
             sbright DELIMITED BY SIZE
             INTO pgstr
           END-STRING. 
           GOBACK.

         ENTRY 'connectionstring' USING pgstate, pgstr.
      *     DISPLAY 'assemble connection string'.
           INITIALIZE pgstr.
           STRING
             pghost OF pgkeywords DELIMITED BY SPACE
             pghost OF pgconnwords DELIMITED BY SPACE
             sspace DELIMITED BY SIZE
             pgport OF pgkeywords DELIMITED BY SPACE
             pgport OF pgconnwords DELIMITED BY SPACE
             sspace DELIMITED BY SIZE
             pguser OF pgkeywords DELIMITED BY SPACE
             pguser OF pgconnwords DELIMITED BY SPACE
             sspace DELIMITED BY SIZE
             pgpassword OF pgkeywords DELIMITED BY SPACE
             pgpassword OF pgconnwords DELIMITED BY SPACE
             sspace DELIMITED BY SIZE
             pgdatabase OF pgkeywords DELIMITED BY SPACE
             pgdatabase OF pgconnwords DELIMITED BY SPACE
             INTO pgstr
           END-STRING.
           GOBACK.      

           