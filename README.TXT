
        +==========================================================+
        |                                                          |
        |              ODBC extension for Win32 Perl               |
        |              -----------------------------               |
        |                                                          |
        |             by Dave Roth <dave@roth.net> 				   |
        |            or <roth@panda.dnr.state.mi.us>               |
        |                                                          |
        |                  version v960528 (hack)                  |
        |                                                          |
        |                                                          |
        |                     ODBC.PM package                      |
        |                     ---------------                      |
        |                     Release v960528                      |
        |                                                          |
        |    Copyright (c) 1996 Dave Roth. All rights reserved.    |
        |   This program is free software; you can redistribute    |
        | it and/or modify it under the same terms as Perl itself. |
        |                                                          |
        +==========================================================+


          based on original code by Dan DeMaggio (dmag@umich.edu)

 Use under GNU General Public License or Larry Wall's "Artistic License"

 This is a hack of Dan DeMaggio's (dmag@umich.edu) NTXS.C ODBC implimentation.
 I have recoded and restructred much of it including most of the ODBC.PM
 package but its core is still based on Dan's code (thanks Dan!).

ODBC.PM HISTORY:
---------------
96.04.07 Joseph L. Casadonte Jr. <joc@netaxs.com>
         -Modified Data() to accept multiple field names
          and return an array or scalar.
         -Confirm Win95 compliance.
         -Various bug fixes.
96.04.10 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
         -Fixed the RowCount() to default to the current connection.
96.04.15 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>		
		 -Changed version numbers to a date format.
96.05.07 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
         -Fixed Data(): If returning an array, the array consisted of
          the requested fieldnames + the values. Now only returns the
          values. Thanks to Dan Westerlund <westerlund@dkraft.se>!
96.05.11 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
		 -Corrected SetStmtCloseType(): it was using the CloseType as
		  if it was a ConnectionNumber instead.
96.05.28 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
         -Added ODBCColAttributes(): returns attribute for a column 
		  in a data set.

ODBC.PLL HISTORY:
----------------
96.04.10 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
         -Fixed a "memory bug": we were using SQL_CLOSE when closing
          an ODBC statement. This kept the cursor alive in memory so
          it can be cached in the event the same SQL statement is issued.
          We are now using SQL_DROP. This may lessen speed if the same
          SQL statements are issued again and again per connection.
         -Cleaned up some code.
96.04.12 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
         -Added G/SetStmtCloseType() functions.
         -Added some constants.
96.04.13 ** We now are trying to include a version for builds up to and
		 including 105 and another for builds 106 and greater (for now).
96.04.15 Jutta M. Klebe <jmk@exc.bybyte.de>
         -Fix Bug in ODBCFetchRow(): when retrieving a field with a NULL 
          value the value from the previous field was reported.
		 -Changed version numbers to a date format.
96.04.22 Jutta M. Klebe <jmk@exc.bybyte.de>
         -Fix the SDWORD wrap-around bug in ODBCFetchRow(): when a column of
		  size  2147483647 adding 1 (for a NULL byte in szBuf) yields
		  (-2147483648) (not easy to "net UCHAR (-2147483648)"!!)
96.04.22 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
		 -Inspired by Jutta, I have increased the max limit for SetMaxBufSize()
		  to 2,147,483,647 bytes.
96.05.03 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
		 -Set the lowest allocated char array to be 20 bytes in ODBCFetchRow().
		  Evidentally sometimes the ODBC manager will report too few chars 
		  that are needed to represent an autonumber field.
96.05.08 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
		 -Convert all results from ODBCTableList() to uppercase since different
		  ODBC drivers impliment this differently (some uppercase some lower).
		  Thanks again to Jutta M. Klebe <jmk@exc.bybyte.de>
		  *** This patch is open for suggestions!!! ***
96.05.13 Mike Knister <knister@sierra.net>
		 -Change SQL_COLUMN_LENGTH to SQL_COLUMN_DISPLAY_SIZE in the SQLColAttributes()
		  line withing ODBC_Fetch(). This was returning the number of bytes needed
		  to represent the C data type for the particular column. We need it to
		  return the number of bytes needed to represent an ASCII version of the value
		  held within the data type.
96.05.28 Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>
         -Added the ColAttributes() method.
		 -Added Version() method.
		  
        

Following in tradition...
*****************************************************************************
*                                                                           *
*  Use under GNU General Public License or Larry Wall's "Artistic License"  *
*                                                                           *
*****************************************************************************

        ----------------------------------------------------------------
NOTICE: I do not guarantee ANYTHING  with this package. If you use it
        you are doing so AT YOUR OWN RISK! I may or may not support this
        depending on my time schedule and I am neither an SQL or ODBC
        guru so do not ask me questions regarding them!
        ----------------------------------------------------------------


I compiled this using MSVC++ 2.2 on an Intel machine. I do not have access to
other platforms to compile on so I will not be doing so. If someone else does,
all the best!


What is the deal with this?
    - The number of ODBC connections are limited by memory and ODBC itself
      (have as many as you want!)

    - The working limit to the size of a field is 10240 bytes but you can
      increase that limit (if needed) to a max of 2147483647. You can always
      recompile to increase the max limit.

    - You can open a connection by either specifing a DSN or a connection
      string!

    - You can open and close the connections in any order!

    - Other things that I can not think of right now.... :)


What known problems does this thing have?
    - If the account that the process runs under does not have write permission
      on the default directory (for the process not the ODBC DSN) you will
      probably get a run time error durring an SQLConnection(). I don't think
      that this is a problem with the code, more like ODBC.
      This happens because some ODBC drivers need to write a temporary
      file. I noticed this using the MS Jet Engine (Access Driver).

    - This has not been neither optimized for speed nor optimized for memory
      consumption. This may run into memory bloat.




To test out this ODBC.PLL, install it then run the TEST.PL included with this
archive. You must first, however alter TEST.PL:
    - The second line of code ($DSN="xxx"), replace the xxx with a real
      DSN that is on your system.



** I use a directory structure of \Perl\lib for my library files and this doc is
** assuming you do too. You will, of course, have to compensate for deviations.


    T O   I N S T A L L   T H I S   B E A S T :
    =========================================

1) You will need to dump the ODBC.PM file into the \PERL\LIB\WIN32\ directory.

2) You need to copy either ODBC105.PLL or ODBC106.PLL (depending on your
   version of Win32 Perl's build number. Run:  perl -v   to check which build
   you have) into \PERL\LIB\AUTO\WIN32\ODBC\ODBC.PLL.\
   **** Notice that we are changing the name to ODBC.PLL!!!!

   ---------------------------------------------------------
If you already have another package \PERL\LIB\ODBC.PM (notice the path does NOT
include Win32) you do not need it for this!!!

To use this you need to define a DSN (Data Source Name) and have all the stuff
you need to use it (ie. proper ODBC drivers, database file, etc).

You are now ready to ODBC all over town!


    T O   U S E   T H I S   B E A S T :
    =================================

Your script will need to have the following line:

    use Win32::ODBC;

Then you need to create a data connection to your DSN...

    $Data = new Win32::ODBC("MyDSN");

NOTE: "MyDSN" should be either the DSN as defined in the ODBC Administrator

      -OR-

      It can be an honest-to-God DSN Connect string
                example: "DSN=My Database;UID=Brown Cow;PWD=Moo;"


You should check to see if $Data is indeed defined otherwise there has been an
error.

You can now send SQL queries and retrieve info to your heart's content!
See the description of functions below and also the test.pl to see how it
all works.

MAKE SURE that you close you connection when you are finished:
    $Data->Close();




    L I S T   O F   F U N C T I O N S :
    =================================

The ODBC.PM Package supports the following functions:


Catalog($Qualifier, $Owner, $Name, $Type)
    Tells ODBC to create a data set that contains table information about
    the DSN.
    The returned format is:
            [Qualifier] [Owner] [Name] [Type]
    returns: TRUE if error.

ColAttributes($Attribute, [@FieldNames])
	Returns the attribute $Attribute on each of the fields @FieldNames in
	the current record set.
	If @FieldNames is empty then all fields are assumed.
	returns: associative array of attributes.

Connection()
    Returns the connection number associated with the ODBC connection.
    returns: Number identifying an ODBC connection

Close()
    Closes the ODBC connection.
    returns: Nothing.

Data($ColName)
    Returns the contents of column name $ColName or the current row.
    returns: String.

DataSources()
    Returns an associative array of Data Sources and ODBC remarks about them.
    They are returned in the form of:
        $ArrayName{'DSN'}=Remark
    Where DSN is the Data Source Name and Remark is, well, the remark.
    returns: Associative array.

Drivers()
    Returns an associative array of ODBC Driverss and thier attributes.
    They are returned in the form of:
        $ArrayName{'DRIVER'}=Attrib1;Attrib2;Attrib3;...
    Where DRIVER is the ODBC Driver Name and AttribX are the driver defined
    attributes.
    returns: Associative array.

DumpData()
    Dumps to the screen the fieldnames and all records of the current data
    set. This is used primarily for debugging.
    returns: Nothing

Error()
    Returns the last encountered error.
    returns: String.

FetchRow()
    Retrieves the next record from the dataset.
    returns: TRUE

FieldNames()
    Returns an array of fieldnames found in the current data set. There is
    no guarantee on order.
    returns: Array of field names.

GetConnections()
    Returns an array of connection numbers showing what connections are
    currently open.
    returns: Array of connections

GetStmtCloseType([$Connection])
    Returns the type of closure that will be used everytime the hstmt is freed.
    See SetStmtClsoeType() for details.
    By default, the currect object's connection will be used. If $Connection
    is a valid connection number, then it will be used.
    returns: String.

GetDSN($Con)
    Returns the DSN that was used during the creation of the connection ($Con).
    If $Con is not passed then it assumes the current connection.
    returns: String.

GetMaxBufSize()
    This will report the current allocated limit for the MaxBufSize. For info
    see SetMaxBufSize().
    returns: Max number of bytes.

new($DSN)
    Creates a new ODBC connection based on $DSN.
    $DSN = Data Source Name or a proper ODBCDriverConnect string.
    returns: Not true if it failed.

RowCount($Connection)
    For UPDATE, INSERT, and DELETE statements the returned value is the number
    of rows affected by the request or -1 if the number of affected rows is
    not available.
    *** NOTE: This function is not supported by all ODBC drivers! Some
              drivers do support this but not for all statements (eg. it
              is supported for UPDATE, INSERT and DELETE commands but
              not for the SELECT command).
    *** NOTE: Many data sources cannot return the number of rows in a result
              set before fetching them; for maximum interoperability,
              applications should not rely on this behavior.
    returns: Number of affected rows or -1 if not supported by driver in the
             current context.

Run($Sql)
    This will execute the $Sql command and dump to the screen info about it.
    This is used primarily for debugging.
    returns: Nothing

SetMaxBufSize($Size)
    This will set the MaxBufSize for a particular connection.
    This will most likely never be needed but...
    The amount of memory that is allocated to retrieve a records field data
    is dynamic and changes when it need to be larger. I found that a memo
    field in an MS Access database ended up requesting 4 Gig of space. This was
    a bit much so there is an imposed limit (2,147,483,647 bytes) that can be 
    allocated for data retrieval. Since it is possible that someone has a 
    database with field data greater than 10240 you can use this function to 
    increase the limit up to a ceiling of 2147483647 (recompile if you need more).
    returns: Max number of bytes.

SetStmtCloseType($Type [, $Connection])
    This will set a particular hstmt close type for the connection. This is
    the same as ODBCFreeStmt(hstmt, $Type).
    By default, the currect object's connection will be used. If $Connection
    is a valid connection number, then it will be used.
    Types:
        SQL_CLOSE
        SQL_DROP
        SQL_UNBIND
        SQL_RESET_PARAMS
    Returns the newly set type.
    returns: String.

ShutDown()
    This will close the ODBC connection and dump to the screen info about it.
    This is used primarily for debugging.
    returns: Nothing

Sql($SQLString)
    Executes the SQL command $SQLString on a particular connection.
    returns: Error number if it failed

TableList($Qualifier, $Owner, $Name, $Type)
    Returns the catalog of tables that are availabe in the DSN.
    If you do not know what the parameters are just leave them "".
    returns: Array of table names.

Version(@Packages)
	Returns the version numbers for the requested packages ("ODBC.PM" or
	"ODBC.PLL"). If @Packages is empty then all version numbers will be
	returned.
	returns: Array of version numbers.

======================= Example of Use =======================================

    use Win32::ODBC;
    uses CGI;

    $Cgi = new CGI;

    $| = 1;

    $Template = "Template";

    print "Content-type: text/html\n\n<HTML>\n<HEAD>\n<TITLE>An ODBC Test</TITLE>\n</HEAD>\n<BODY>";

    ($DSN, $Table) = ($Cgi->param('Table') =~ /(\S*)\s*(\S.*)/);

    $Table =~ s/\s*(\S.*)(\s|\n|\x0a|\x0d)*$/$1/gi;
    $DSN =~ s/\s*(\S.*)(\s|\n|\x0a|\x0d)*$/$1/gi;
    ($Num = $Cgi->param('ID')) =~ s/[\s|-|\.]//gi;

    if ($DSN eq "" | $Table eq ""){
        $Message = "Your Table or DSN was invald. This may be because you have
                    not specified them or you have a bad browser. You may want to
                    try another browser.<p>";

        print "$Message\n</BODY>\n</HTML>";
        Log("Error: DSN or Table is blank");
        exit();
    }

    $O = new Win32::ODBC("$DSN");
    if (!$O){
        $Message = "The $DSB database is currently unavailable. Try again later.";
        print "$Message\n</BODY>\n</HTML>";
        Log("Error: Could not connect; ODBC error during connection");
        exit();
    }

    $Sql = "SELECT *
            FROM [$Table] ";

    if (! $O->Sql($Sql)){
        print "<p>\n<pre>";

        foreach $Key ($O->FieldNames){
            print $O->Data($Key), "  ";
            $Line .= "-" x length($Key) . "  ";
        }
        print "\n$Line\n";
        if ($O->FetchRow()){
            foreach $Key ($O->FieldNames){
                print $O->Data($Key), "  ";
            }

            $Result = "Successful";
        }else{
            $Result = "Unsuccessful";
        print "</pre>\n</BODY>\n</HTML>";
       }
    }else{
          $Message = "The database seems to be down. Try again later.";
        print "$Message\n</BODY>\n</HTML>";
          $Result = "Error: " . $O->error;
    }

    $O->Close();

    Log($Result);



sub Log{
    local($Result) = @_;
    if (open(LOG, ">> TEST.log")){
        print LOG $Cgi->remote_addr(), " \"", $Cgi->user_agent(), "\"  \"$DSN\" \"$Table\" \"$Num\" \"$Result\"\n";
        close(LOG);
    }
}



