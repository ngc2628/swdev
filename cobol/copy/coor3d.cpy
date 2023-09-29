
      * table struct table-data-3d.
           10 f-length PIC 9(3).
           10 c-header PIC X(6) OCCURS 3 TIMES.
           10 t-row OCCURS 1 TO 1000 TIMES DEPENDING ON f-length.
             15 r-header PIC X(32).
             15 t-col OCCURS 3 TIMES.
               20 t-val PIC 9(16).9(15) VALUE 0.0.

