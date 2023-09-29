
package tstjdbc;
//STEP 1. Import required packages
import java.sql.*;

public class ConnJdbc {

  static final String DB_HOST_AND_PORT="hal9004:3306";
  static final String DB_NAME="mk";
  // JDBC driver name, database URL, user+pass
  static final String JDBC_DRIVER="com.mysql.cj.jdbc.Driver";
  static final String JDBC_URL="jdbc:mysql://"+DB_HOST_AND_PORT+"/"+DB_NAME;
  static final String DB_USER="siberia";
  static final String DB_PASS="tiger";

  public static void main(String[] args) {

    String slog=null;
    Connection conn=null;
    Statement stmt=null;
    try {
      //STEP 2: Register JDBC driver
      Class.forName(JDBC_DRIVER);

      //STEP 3: Open a connection
      slog="Connecting to database "+DB_NAME+" on "+DB_HOST_AND_PORT+" using user "+DB_USER;
      System.out.println(slog);
      conn=DriverManager.getConnection(JDBC_URL,DB_USER,DB_PASS);

      //STEP 4: Execute a query
      System.out.println("Creating statement...");
      stmt=conn.createStatement();
      String sql="SELECT nickname,firstname,lastname FROM name";
      ResultSet rs=stmt.executeQuery(sql);

      //STEP 5: Extract data from result set
      while(rs.next()) {
        //Retrieve by column name
        String nick=rs.getString("nickname");
        String fname=rs.getString("firstname");
        String lname=rs.getString("lastname");
        //Display values
        System.out.print("nick: "+nick);
        System.out.println(", name: "+fname+" "+lname);
      }

      //STEP 6: Clean-up environment
      rs.close();
      stmt.close();
      conn.close();
    }
    catch (SQLException se) {
      //Handle errors for JDBC
      se.printStackTrace();
    }
    catch (Exception e) {
      //Handle errors for Class.forName
      e.printStackTrace();
    }
    finally {
      //finally block used to close resources
      try {
        if (stmt!=null)
          stmt.close();
      }
      catch (SQLException se2) {
      } // nothing we can do
      try{
        if(conn!=null)
          conn.close();
      }
      catch (SQLException se) {
        se.printStackTrace();
      }
    } //end try-catch-finally

    System.out.println("Goodbye!");

  } //end main

} // end ConnJdbc



