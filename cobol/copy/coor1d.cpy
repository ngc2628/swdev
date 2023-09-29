
      * table struct table-data-1d.
           10 t-length pic 9(3).
           10 c-header pic x(6).
           10 t-row occurs 1 to 1000 times depending on t-length.
             15 r-header pic x(32).
             15 t-col.
               20 t-val pic 9(16).9(15) value 0.0.

