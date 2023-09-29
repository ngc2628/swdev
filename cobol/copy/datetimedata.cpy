       
      * structure datetime  
           10 m_date.                                 
             15 m_year PIC 9(4).
             15 FILLER REDEFINES m_year.
               20 FILLER PIC 9(2).
               20 m_year2 PIC 9(2).
             15 m_month PIC 9(2).           
             15 m_day PIC 9(2).           
           10 m_time.                                 
             15 m_hour PIC 9(2).           
             15 m_minute PIC 9(2).           
             15 m_second PIC 9(2).
           10 datetime-str PIC X(19).
