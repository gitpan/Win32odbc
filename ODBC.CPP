/*
        +==========================================================+
        |                                                          |
        |              ODBC extension for Win32 Perl               |
        |              -----------------------------               |
        |                                                          |
        |            by Dave Roth <rothd@roth.net>                 |
        |                                                          |
        |                  version v961025                         |
        |                                                          |
        |    Copyright (c) 1996 Dave Roth. All rights reserved.    |
        |   This program is free software; you can redistribute    |
        | it and/or modify it under the same terms as Perl itself. |
        |                                                          |
        +==========================================================+


          based on original code by Dan DeMaggio (dmag@umich.edu)

   Use under GNU General Public License or Larry Wall's "Artistic License"
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>

#include <SQL.H>
#include <SQLEXT.H>
#include <ODBCINST.H>
#include <EXTERN.h>
#include "perl.h"
#include "XSub.h"
#include "odbc.h"	
#include "constant.h"

/*----------------------- P E R L   F U N C T I O N S -------------------*/


XS(XS_WIN32__ODBC_Constant)
{
	dXSARGS;
	if (items != 2)
	{
		croak("Usage: Win32::ODBC::Constant(name, arg)\n");
    }
	{
		char* name = (char*)SvPV(ST(0),na);
		ST(0) = sv_newmortal();
		sv_setiv(ST(0), constant(pPerl, name));
	}
	XSRETURN(1);
}


/*----------------------- M I S C   F U N C T I O N S -------------------*/

 	/*
 		Allocate memory for a new connection and set up the structure
	*/

ODBC_TYPE *NewODBC(){
	ODBC_TYPE *h;

	if (!(h = new ODBC_TYPE)){
		ODBCError("Could not allocate memory of an ODBC connection\n", 0, "NewODBC", "1");
		h = 0;
	}else{

		h->Prev = ODBCList;
		h->Next = ODBCList->Next;
		if(h->Next != 0){
			h->Next->Prev = h;
		}
		if(h->Prev != 0){
			h->Prev->Next = h;
		}
		h->szCommand = 0;
		h->szDSN = 0;
		h->conn = ODBC_Conn_Number++;
		ODBCTotal++;
		CleanODBC(h);
	}
	return h;
}

	/*
		Clean up an ODBC structure.
	*/

ODBC_TYPE *CleanODBC(ODBC_TYPE *h){
	h->hstmt = SQL_NULL_HSTMT;
	h->henv  = SQL_NULL_HENV;
	h->hdbc  = SQL_NULL_HDBC;
	h->iMaxBufSize = DEFAULT_DATA_BUF_SIZE;
	h->numcols = 0;

	h->uStmtCloseType = DEFAULT_STMT_CLOSE_TYPE;
	h->iDebug = 0;
	h->dMaxRows = 0;
	h->dNumOfRows = 0;
	strcpy(h->szUserDSN, "");
	if (h->szDSN){
		delete [] h->szDSN;
		h->szDSN = 0;
	}
	if (h->szCommand){
		delete [] h->szCommand;
		h->szCommand = 0;
	}
	CleanError(h);
	return h;
}

 	/*
		Deallocates memory used for an ODBC structure.
		If the master list (ODBCList) is passed to this it will do nothing unless
		there are no other ODBC structures remaining.
	*/

void FreeODBC(ODBC_TYPE *h){	
	if (h->hstmt){
		SQLFreeStmt(h->hstmt, SQL_DROP);
		h->hstmt = SQL_NULL_HSTMT;
	}
	if (h->hdbc){
		SQLDisconnect(h->hdbc);
		SQLFreeConnect(h->hdbc);
		h->hdbc  = SQL_NULL_HENV;
	}		
	if (h == ODBCList){
			//	Release the Environment ONLY for the Master. All others will use
			//	this environment anyway.
		if (h->henv != SQL_NULL_HENV && ODBCTotal == 1){
			SQLFreeEnv(h->henv);
			h->henv = SQL_NULL_HENV;
			ODBCTotal--;
		}
	}else{
		h->henv = SQL_NULL_HENV;
		ODBCTotal--;
	}
	
	CleanODBC(h);
	return;
}

	/*
		Reset (clean) an ODBC structures error state information
	*/
void CleanError(ODBC_TYPE *h){
	if (h){
		strcpy(h->szError, "");
		strcpy((char *)h->szSqlState, "");
		strcpy(h->szFunction, "");
		strcpy(h->szFunctionLevel, "");
		h->error = 0;
		h->EOR = 0;
	}
	return;
}

	/*
		Reset an ODBC Stmt. (alloc memory if needed, free it up if needed)
	*/	  

RETCODE ResetStmt(ODBC_TYPE *h){
	RETCODE	iReturnCode, iTemp = 0;
  			//	If the SQLFreeStmt() failed, should we reallocate the stmt? 
			//	For now let's just return the error code and skip the reallocation.
	if (h->hstmt != SQL_NULL_HSTMT){
		iReturnCode = SQLFreeStmt(h->hstmt, h->uStmtCloseType);
		if (iReturnCode == SQL_SUCCESS){
				//	We will need to realloc the hstmt ONLY if we DROPPED it!
			if(h->uStmtCloseType == SQL_DROP){
				h->hstmt = SQL_NULL_HSTMT;
			}
		}else{
			_NT_ODBC_Error(h, "ResetStmt", "1");
		}
	}
	if (h->hstmt == SQL_NULL_HSTMT){
		if ((iReturnCode = SQLAllocStmt(h->hdbc, &h->hstmt)) != SQL_SUCCESS){
			_NT_ODBC_Error(h, "ResetStmt", "2");
		}
	}
	return iReturnCode;
}
	
	/*
		Process an ODBC structures error state from the ODBC API.
	  	This is called when an error is encountered via the ODBC API.
	*/

void _NT_ODBC_Error(ODBC_TYPE * h, char *szFunction, char *szFunctionLevel){
	SDWORD cbErrorMsg;

	strcpy((char *)h->szSqlState, "");
	strcpy(h->szError, "");
	SQLError(h->henv, h->hdbc, h->hstmt, (UCHAR *)h->szSqlState, (long *)&(h->error), (unsigned char *)h->szError, ODBC_BUFF_SIZE, (short *)&cbErrorMsg);

		//	Next couple of lines should be NOT needed. If there is no error, then
		//	we should not have come here in the first place. If, however, there
		//	is state information that may be relevant (SQL_SUCCESS_WITH_INFO)
	if (!h->error){
		h->error = 911;
	}
	strcpy(h->szFunction, szFunction);
	strcpy(h->szFunctionLevel, szFunctionLevel);
}

	/*
		Process an ODBC structures error state from Win32::ODBC.
		This is called when an error is encountered NOT from the ODBC API
		but from this body of code.
	*/

ODBC_TYPE *ODBCError(char *szString, ODBC_TYPE *h, char *szFunction, char * szFunctionLevel){
	if(h == 0){
		h = ODBCList;
	}
	h->error = 911;
	strcpy(h->szError, szString);
	strcpy(h->szFunction, szFunction);
	strcpy(h->szFunctionLevel, szFunctionLevel);
	return h;
}

	/*
		Map a column name (field name) to a column number that exists in a
		resulting dataset.
	*/

int	ColNameToNum(ODBC_TYPE *h, char *szName){
	int	iResult = 0;
	int	x;
	char	szBuff[ODBC_BUFF_SIZE];
	DWORD	dBuffLen = 0;

	for(x=1; x<=h->numcols; x++){
		SQLColAttributes(h->hstmt, x, SQL_COLUMN_NAME, szBuff, ODBC_BUFF_SIZE, (short *)&dBuffLen, NULL);
		if(!stricmp(szName, szBuff)){
			iResult = x;
			break;
		}
	}
	return iResult;
}


	/*
		Check if the specified connection is valid and return the pointer
		to the ODBC structure.
		Return a pointer to the master list (ODBCList) and genereate an error if
		there is no valid connection.
	*/
ODBC_TYPE * _NT_ODBC_Verify(int iODBC){
	ODBC_TYPE *hTemp;

	ODBC_errornum = 0;
	hTemp = ODBCList;
		//	Walk down the linked list looking for the ODBC Connection
	while (hTemp != 0){
		if (hTemp->conn == iODBC){
			if (hTemp->EOR){
					//	If EOR (End Of Records) then clear any errors. EOR is really not an error.
				CleanError(hTemp);
			}
			return(hTemp);			
		}
		hTemp = (ODBC_TYPE *)hTemp->Next;
	}
	ODBCError("No such ODBC connection.", 0, "_NT_ODBC_Verify", "1");
	return (ODBCList);
}

	/*
		Delete an ODBC structure. This will call other routines to release
		allocated memory and reset ODBC state information.
	*/
int DeleteConn(int conn){
	ODBC_TYPE	*h, *hTemp;
	int	iAll = 0;
	int	iResult = 0;
	
	if (!conn){
		iAll = 1;
	}
	if (conn > 0 || iAll == 1){
		h = (ODBC_TYPE *) ODBCList->Next;
		while(h){
			hTemp = (ODBC_TYPE *)h->Next;
			if (iAll){
				conn = h->conn;
			}
			if (h->conn == conn){
				FreeODBC(h);
				if (h->Prev){
					(h->Prev)->Next = h->Next;
				}
				if (h->Next){
					(h->Next)->Prev = h->Prev;
				}
				delete h;
				iResult++;
			}
			h = hTemp;
		}
	}
	return iResult;
}




	/*
		Map an Stmt close type string to the actual Stmt close type value.
	*/

char *MapCloseType(UWORD uCloseType){
	char *szType;

	switch(uCloseType){
		case SQL_DROP:
			szType = "SQL_DROP";
			break;

		case SQL_CLOSE:
			szType = "SQL_CLOSE";
			break;

		case SQL_UNBIND:
			szType = "SQL_UNBIND";
			break;

		case SQL_RESET_PARAMS:
			szType = "SQL_RESET_PARAMS";

		default:
			szType = 0;
	}
	return szType;
}

/*------------------- P E R L   O D B C   F U N C T I O N S ---------------*/

		/*
			ODBC_Connect
			Connects to and ODBC Data Source Name (DSN).
		*/
XS(XS_WIN32__ODBC_Connect) // ODBC_Connect(Connection string: input) returns connection #
{
	dXSARGS;

	char      szDSN[DSN_LENGTH]; // string to hold datasource name
	ODBC_TYPE * h;
	int         con_num; //connection #
	char		*szIn;
	int			iTemp = 0;

	RETCODE retcode;           // Misc ODBC sh!t
	UCHAR   buff[ODBC_BUFF_SIZE];
	SDWORD  bufflenout;
	int     lenn = 0;

	if(items < 1 || !(items & 1)){
			//	We need at least 1 (DSN) entry. If more then we need them in pairs of
			//	two, hence (items & 1) make sure we have an odd number of entries...
			//	(dsn) + (ConnetOption, Value) [ + (ConnectOption, Value)] ...
		CROAK("usage: ($Connection, $Err, $ErrText) = ODBC_Connect($DSN [, $ConnectOption , $Value] ...)\n");
	}
	szIn = SvPV(ST(0), na); 

	if(strcspn(szIn, "[]{}(),;?*=!@") < strlen(szIn)){
		strncpy(szDSN, szIn, DSN_LENGTH - 1);
		szDSN[DSN_LENGTH - 1] = '\0';
	}else{
			//	Let's assume that the DSN will not exceed DSN_LENGTH
		strcpy(szDSN, "DSN=");	
		strcat(szDSN, szIn); // Data Source string
		strcat(szDSN, ";");
	}

	PUSHMARK(sp);

 		 	//	Allocate new ODBC connection
	if (!(h = NewODBC())){
		h = ODBCError("Could not allocate memory of an ODBC connection\n", 0, "ODBC_Connect", "1");
	}else{
		strcpy(h->szUserDSN, szIn);

		h->henv = ODBCList->henv;

	 	if (!h->error){
			retcode = SQLAllocConnect(h->henv, &h->hdbc);
			if (retcode != SQL_SUCCESS)
			{
				_NT_ODBC_Error(h, "ODBC_Connect", "2");
				DeleteConn(h->conn);
			}
		}
		if (!h->error){
				//	If any pre-connect SQLConnectOptions are specified, do them now...
			if (items > 1){
				items - 1;
				UWORD   uType;
    			UDWORD  udValue;
				char	szError[100];

				while (iTemp > 1){
					uType= SvIV(ST(iTemp - 1));
    				if (SvIOKp(ST(iTemp)) || SvNOKp(ST(iTemp))){
    					udValue = SvIV(ST(iTemp));
					}else{
						udValue = (UDWORD) SvPV(ST(iTemp), na);
					}
			        retcode = SQLSetConnectOption(h->hdbc, uType, udValue);
			        if (retcode != SQL_SUCCESS){
						sprintf(szError, "2a: Connect Item number %i", (iTemp/2));
			 			_NT_ODBC_Error(h, "ODBC_Connect", szError);
						break;
			        }
					iTemp -= 2;
				}
			}
		}
		if (!h->error){
/*
			retcode = SQLSetConnectOption(h->hdbc, SQL_LOGIN_TIMEOUT, (UDWORD)LOGIN_TIMEOUT_VALUE);
 			if (retcode != SQL_SUCCESS){
					// This should produce an error of 0!
				_NT_ODBC_Error(h, "ODBC_Connect", "3");
				h->error = 0;
			}	  
*/
			retcode = SQLDriverConnect(h->hdbc, (HWND) NULL, (unsigned char *)szDSN, strlen(szDSN), buff, ODBC_BUFF_SIZE, (short *)&bufflenout, SQL_DRIVER_NOPROMPT);
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			{
				_NT_ODBC_Error(h, "ODBC_Connect", "4");
				strcpy(ODBCList->szError, h->szError);
				strcpy((char *)ODBCList->szSqlState, (char *)h->szSqlState);
				strcpy(ODBCList->szFunction, h->szFunction);
				strcpy(ODBCList->szFunctionLevel, h->szFunctionLevel);
					//	If connection fails it does not genereate an error num. Hmmm...
				if(! h->error){
					h->error = 911;
				}
				ODBCList->error = h->error;
				DeleteConn(h->conn);
				h = ODBCList;
			}else{
				if (h->szDSN = new char [strlen((const char *)buff) + 1]){
					strcpy(h->szDSN, (char *) buff);
				}
				if (retcode == SQL_SUCCESS_WITH_INFO){
					_NT_ODBC_Error(h, "ODBC_Connect", "5");
					h->error = 0;
				}
			}
		}
		if (!h->error){
			retcode = ResetStmt(h);
			if (retcode != SQL_SUCCESS){
				DeleteConn(h->conn);
			}
		}
	}
	if (!h->error){ // everything is happy
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)h->conn)));
			//	Report the szError ONLY because it may contain state info.
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
} 


XS(XS_WIN32__ODBC_Execute) // ODBC_Execute($connection, $sql_text) returns (0,@fieldnames) or (err, errtext)
{
	dXSARGS;
	ODBC_TYPE * h;
	int        con_num; // Connection #
	RETCODE retcode;          //ODBC gunk
	UCHAR  buff2[ODBC_BUFF_SIZE];
	SDWORD bufflenout;
	int lenn;
	UWORD  x;
	char * szSQL; 
	int len;

	if(items < 2){
		CROAK("usage: ($err,@fields) = ODBC_Execute($connection, $sql_text)\nprint \"Oops: $field[0]\" if ($err);\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	
	PUSHMARK(sp);

	if (h != ODBCList){
		szSQL = SvPV(ST(1), na);            // get SQL string
		CleanError(h);
		if (ResetStmt(h) == SQL_SUCCESS){
			if (h->szCommand){
				delete [] h->szCommand;
				h->szCommand = 0;
			}
			if (h->szCommand = new char [strlen(szSQL) + 1]){
				strcpy(h->szCommand, szSQL);
			}
		}
		if (!h->error){
			retcode = SQLExecDirect(h->hstmt, (unsigned char*)szSQL, strlen(szSQL));
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO){
				_NT_ODBC_Error(h, "ODBC_Execute", "1");
			}else{
					//	Set up our MaxRows Fetch() simulator
				SQLGetStmtOption(h->hstmt, SQL_MAX_ROWS, &h->dMaxRows);
				h->dNumOfRows = 0;
			}
		}
	}
	if (!h->error){ // everything is happy
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		retcode = SQLNumResultCols(h->hstmt, (short *)&h->numcols);
		for(x=1; x<=h->numcols; x++){
			SQLColAttributes(h->hstmt, x, SQL_COLUMN_NAME, buff2, ODBC_BUFF_SIZE, (short *)&bufflenout, NULL);
			XPUSHs(sv_2mortal(newSVpv((char *)buff2, strlen((const char*)buff2))));
		}
	}else{										
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
}


		/*
			ODBC_Fetch
			Fetch a row from the current dataset.
		*/
XS(XS_WIN32__ODBC_Fetch) // ODBC_Fetch($connection) returns (0,@dataelements) or ($err,$errtext)
{
	dXSARGS;
	ODBC_TYPE * h;
	int        con_num;

	RETCODE retcode;         // yet more ODBC garbage
	UWORD	uType = SQL_FETCH_NEXT;
	SDWORD	sdRow = 1;
	DWORD	dRowSetSize = 1;
	UWORD	*rgfRowStatus = 0;
	UDWORD	udCRow = 0;
	int		iTemp;	

	if(items < 1 || items > 3){
		CROAK("usage: ($err,@col) = ODBC_Fetch($connection [, $Row [, $FetchType]])\n0die \"Oops: $col[0]\" if ($err);\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
		//	NOTICE: We do not perform a CleanODBC(h) here because a dirty
		//	h will tell us if there is an error such as EOR (end of records).
 	if (items > 1){
		sdRow = SvIV(ST(1));
		uType = SQL_FETCH_RELATIVE;
	}
	if (items > 2){
		uType = SvIV(ST(2));
	}

	PUSHMARK(sp);

	if (h != ODBCList){
		if (items > 1){
			SQLGetStmtOption(h->hstmt, SQL_ROWSET_SIZE, &dRowSetSize);
			if (!(rgfRowStatus = new UWORD [dRowSetSize])){
				ODBCError("Can not allocate memory for row status results", h, "ODBC_Fetch", "1b");
			}
			retcode = SQLExtendedFetch(h->hstmt, uType, sdRow, &udCRow, rgfRowStatus);
		}else{
			retcode = SQLFetch(h->hstmt);
		}
		if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
			_NT_ODBC_Error(h, "ODBC_Fetch", "1");
		}
		if (retcode == SQL_NO_DATA_FOUND){
			ODBCError("No data records remain.", h, "ODBC_Fetch", "2");
			h->EOR = 1;
		}
		if(retcode == SQL_SUCCESS_WITH_INFO){
			_NT_ODBC_Error(h, "ODBC_Fetch", "3");			
		}
	}
/*
		//	This is for backward compatibility so that if you are just minding your
		//	own business and only want the default rowset size (1) but have specified
		//	a SQL_MAX_ROWS then all works well. This will simulate what we need.
	if((!h->error) && h->dMaxRows && (dRowSetSize == 1)){
		h->dNumOfRows += udCRow;
		if(h->dNumOfRows > h->dMaxRows){
			ODBCError("No data records remain.", h, "ODBC_Fetch","4");
			h->EOR = 1;
		}
	}
*/
	if (! h->error){
			 // everything is happy
		XPUSHs(sv_2mortal(newSVnv((double)0)));	
		if(items > 1){
			for(iTemp = 0; iTemp < udCRow; iTemp++){
				XPUSHs(sv_2mortal(newSVnv((double)rgfRowStatus[iTemp])));	
			}
		}else{
			XPUSHs(sv_2mortal(newSVnv((double)1)));	
		}
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	if (rgfRowStatus){
		delete [] rgfRowStatus;
	}
	PUTBACK;
} //ODBC_Fetch



XS(XS_WIN32__ODBC_GetError)
{
	dXSARGS;
	ODBC_TYPE *h;

	h = _NT_ODBC_Verify(SvIV(ST(0)));

	PUSHMARK(sp);
	XPUSHs(sv_2mortal(newSVnv((double)h->error)));
	XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	XPUSHs(sv_2mortal(newSVpv((char *)h->szSqlState, strlen(( const char *) h->szSqlState))));
	if (h->iDebug){
		XPUSHs(sv_2mortal(newSVpv((char *)h->szFunction, strlen(( const char *) h->szFunction))));
		XPUSHs(sv_2mortal(newSVpv((char *)h->szFunctionLevel, strlen(( const char *) h->szFunctionLevel))));
	}
		
	PUTBACK;
}
		  

XS(XS_WIN32__ODBC_Disconnect) // usage: ODBC_Disconnect($conn) or ODBC_Disconnect() for all
{
	dXSARGS;
	ODBC_TYPE *h;
	int		iResult = 0;
	int		conn;
	
	if(items < 1){
		conn = 0;
	}else{
		conn = SvIV(ST(0));
	}
	PUSHMARK(sp);
	if (!(iResult = DeleteConn(conn))){
		h = ODBCError("No such connection", 0, "ODBC_Disconnect", "1");
		if(h->iDebug){
			MessageBeep(MB_OK);
		}
	}
	if (iResult){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)iResult)));
	}else{
				//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
		PUTBACK;
}

XS(XS_WIN32__ODBC_TableList) // ODBC_TableList($connection) returns (0,@TableNames) or ($err,$errtext)
{
	dXSARGS;
	ODBC_TYPE * h;
	int        con_num, iTemp;
	UCHAR  buff2[ODBC_BUFF_SIZE];
	SDWORD bufflenout;
	UWORD  x;

	RETCODE retcode;
	UCHAR	*szQualifier, *szOwner, *szName, *szType;
	SWORD	sQLen, sOLen, sNLen, sTLen = 0;
	UWORD	uTemp = 0;

	if(items != 5){
		CROAK("usage: ($Err,@ColumnNames) = ODBC_TableList($connection, $Qualifier, $Owner, $TableName, $TableType)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	
	if (szQualifier = (unsigned char *) SvPV(ST(1), na)){
		if (!(sQLen = strlen((char *)szQualifier))){
			szQualifier = 0;
		}
	}

	if (szOwner = (unsigned char *) SvPV(ST(2), na)){
		if (!(sOLen = strlen((char *)szOwner))){
			szOwner = 0;
		}
	}

	if (szName = (unsigned char *) SvPV(ST(3), na)){
		if (!(sNLen = strlen((char *)szName))){
			szName = 0;
		}
	}

	if (szType = (unsigned char *) SvPV(ST(4), na)){
		if (!(sTLen = strlen((char *)szType))){
			szType = 0;
		}
	}
	
	PUSHMARK(sp);

	if (h != ODBCList){
		CleanError(h);
		retcode = ResetStmt(h);
		if (h->szCommand){
			delete [] h->szCommand;
			h->szCommand = 0;
		}
				/*	The funny odd thing about this next line is that when we allocate
					memory of h->szCommand it will be based on a sprintf(). The format
					string for sprintf() will have references to szQualifier, szOwner, etc.
					IF any of those strings are 0 then the %s's in the format string may be
					replaced with "(null)" or some other indicator of a null pointer. Therefore
					in the event of szQualifier (or whatever) == 0 we will provide an arbitrary
					number of bytes for allocation (like 10).
				*/
							
		if (h->szCommand = new char [strlen((const char *) TABLE_COMMAND_STRING)
							+ (szQualifier ? strlen((const char *)szQualifier):10)
							+ (szOwner ? strlen((const char *)szOwner):10)
							+ (szName ? strlen((const char *)szName):10)
							+ (szType ? strlen((const char *)szType):10) + 1]){
			sprintf(h->szCommand, TABLE_COMMAND_STRING, szQualifier, szOwner, szName, szType);
		}
		
		retcode = SQLTables(h->hstmt, szQualifier, sQLen, szOwner, sOLen, szName, sNLen, szType, sTLen);
		if(retcode != SQL_SUCCESS){
			_NT_ODBC_Error(h, "ODBC_TableList", "1");
		}
	}
	if (!h->error){ 			
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		retcode = SQLNumResultCols(h->hstmt, (short *)&h->numcols);
		for(x=1; x<=h->numcols; x++){
			*buff2 = '\0';
			SQLColAttributes(h->hstmt, x, SQL_COLUMN_NAME, buff2, ODBC_BUFF_SIZE, (short *)&bufflenout, NULL);
		   		/*	
					We need to convert the columns to uppercase since different
					ODBC drivers impliment this differently (Access uses upperc, MS SQL server uses lowerc)
					We should probably figure out a more reasonable solution.
				*/
			for(iTemp = strlen((char *)buff2) - 1; iTemp >= 0; iTemp--){
				buff2[iTemp] = (char) toupper(buff2[iTemp]);
			}
			XPUSHs(sv_2mortal(newSVpv((char *)buff2, strlen((const char*)buff2))));
		}
			//	Set up our MaxRows Fetch() simulator
		SQLGetStmtOption(h->hstmt, SQL_MAX_ROWS, &h->dMaxRows);
		h->dNumOfRows = 0;
	}else{
		//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
}

/*
	This next chunk of code (XS_WIN32__ODBC_MoreResults) was 
	graciously donated by Brian Dunfordshore <Brian_Dunfordshore@bridge.com>.
	Thanks Brian!
	96.07.10
*/
XS(XS_WIN32__ODBC_MoreResults) 	// usage: ODBC_MoreResults($conn)
{
	dXSARGS;
 	RETCODE retcode;         // yet more ODBC garbage
 	ODBC_TYPE *h;
 
 	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
 	PUSHMARK(sp);
 	if (h != ODBCList){
 		retcode = SQLMoreResults(h->hstmt);
 		if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
 			_NT_ODBC_Error(h, "ODBC_MoreResults", "1");
 		}
 		if (retcode == SQL_NO_DATA_FOUND){
 			h->EOR = 1;
 			strcpy(h->szError, "No data records remain.");
 		}
 	}
 	if (!h->error){		 // everything is happy
 		XPUSHs(sv_2mortal(newSVnv((double)0)));
 		XPUSHs(sv_2mortal(newSVnv((double)1)));	  //	Return a TRUE
 	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
 		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
 		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
 	}
	PUTBACK;
}

XS(XS_WIN32__ODBC_GetMaxBufSize) 
{
	dXSARGS;
	ODBC_TYPE * h;
	long	iSize;		

	if(items != 1){
		CROAK("usage: ($Err, $Size) = ODBC_GetMaxBufSize($Connection)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	PUSHMARK(sp);

	if (h != ODBCList){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)h->iMaxBufSize)));
	}else{										
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;

}

XS(XS_WIN32__ODBC_SetMaxBufSize) 
{
	dXSARGS;
	ODBC_TYPE * h;
	int		con_num;
	long	iSize;		
	
	if(items != 2){
		CROAK("usage: ($Err, $Size) = ODBC_SetMaxBufSize($Connection, $NewSize)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	PUSHMARK(sp);

	if (h != ODBCList){
		iSize = SvIV(ST(1));
		iSize = ((iSize <= 0)? 0:iSize);
		iSize = ((iSize >= MAX_DATA_BUF_SIZE)? MAX_DATA_BUF_SIZE:iSize);
		h->iMaxBufSize = iSize;
	
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)h->iMaxBufSize)));
	}else{										
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}


	PUTBACK;
}

XS(XS_WIN32__ODBC_GetConnections) 
{
	dXSARGS;
	ODBC_TYPE *h = ODBCList;

	if(items != 0){
		CROAK("usage: (@ConnectionList) = ODBC_GetConnections()\n");
	}
	PUSHMARK(sp);
	h = h->Next;
	while (h){
		XPUSHs(sv_2mortal(newSVnv((double)h->conn)));
		h = h->Next;
	}
	PUTBACK;
}

XS(XS_WIN32__ODBC_GetDSN) 
{
	dXSARGS;
	ODBC_TYPE *h;
	char	*szDSN = 0;
	char	*szTemp = 0;
	DWORD	dSize = 1024;
	char	*szKeys= 0;
	char    *szValues = 0;
	char	*szPointer = 0;

	if(items > 2 || items < 1){
		CROAK("usage: ($Err, $DSN) = ODBC_GetDSN($Connection [, $DSN])\n");
	}
	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);

	if (items > 1){
		szDSN = SvPV(ST(1), na);
		if (! strlen(szDSN)){
			szDSN = 0;
		}
	}
	if (! szDSN){
		szDSN = h->szDSN;
		while(szDSN){
			if (szDSN[0] == ';'){
				szDSN++;
			}
			if (! strnicmp("DSN=", szDSN, 4)){
				szDSN += 4 * sizeof(char);
				break;
			}
			szDSN = strchr(szDSN, ';');
		}
	}

	PUSHMARK(sp);

	szTemp = szDSN;
	if (! (szDSN = new char [strlen(szTemp) + 1])){
		szDSN = 0;
		ODBCError("Could not allocate memory for DSN comparison", h, "ODBC_GetDSN", "1");
	}else{

		strcpy(szDSN, szTemp);
		szTemp = strchr(szDSN, ';');
		if (szTemp){
			*szTemp = '\0';
		}
		if (szKeys = new char [dSize + 1]){	
			SQLGetPrivateProfileString(	szDSN, 0, "", szKeys, dSize, "ODBC.INI");
			if (strcmp(szKeys, "")){
				szPointer = szKeys;
				if (szValues = new char [dSize + 1]){	
					XPUSHs(sv_2mortal(newSVnv((double)0)));
					while(*szPointer){
						SQLGetPrivateProfileString(	szDSN, szPointer, "", szValues, dSize, "ODBC.INI");
						XPUSHs(sv_2mortal(newSVpv(szPointer, strlen(szPointer))));
						XPUSHs(sv_2mortal(newSVpv(szValues, strlen(szValues))));
						szPointer += strlen(szPointer) + 1;
					}
					if(szValues) delete [] szValues;
				}else{
					ODBCError("Could not allocate enough memory", h, "ODBC_GetDSN", "2");
				}
			}else{
				ODBCError("Not a valid DSN", h, "ODBC_GetDSN", "3");
			}
			if(szKeys) delete [] szKeys;
		}else{
			ODBCError("Could not allocate enough memory", h, "ODBC_GetDSN", "4");
		}
	}

	if (h->error){
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	if (szDSN){
		delete [] szDSN;
	}
	
	PUTBACK;
}

XS(XS_WIN32__ODBC_DataSources) 
{
	dXSARGS;
	ODBC_TYPE *h;
	UCHAR	szDSN[SQL_MAX_DSN_LENGTH + 1];
	SWORD	pcbDSN;
	UCHAR	szDesc[DS_DESCRIPTION_LENGTH];
	SWORD	pcbDesc;	
	char	*szRequestedDSN = 0;
	RETCODE	retcode;

	if(items > 1){
		CROAK("usage: ($Err, $DSN) = ODBC_DataSources([$DSN])\n");
	}
	
	h = ODBCList;

	if (items){
		szRequestedDSN = SvPV(ST(0), na);
		if (!strlen(szRequestedDSN)){
			szRequestedDSN = 0;
		}
	}

	PUSHMARK(sp);

	*szDSN = *szDesc = '\0';
	retcode= SQLDataSources(h->henv, SQL_FETCH_FIRST, szDSN, SQL_MAX_DSN_LENGTH + 1, &pcbDSN, szDesc, DS_DESCRIPTION_LENGTH, &pcbDesc);
	if(retcode == SQL_SUCCESS){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		
		while (retcode == SQL_SUCCESS){
			if ((szRequestedDSN && (!stricmp((const char *)szDSN, szRequestedDSN))) || !szRequestedDSN){
				XPUSHs(sv_2mortal(newSVpv((char *)szDSN, strlen((char*)szDSN))));
				XPUSHs(sv_2mortal(newSVpv((char *)szDesc, strlen((char*)szDesc))));
			}
			*szDSN = *szDesc = '\0';
			retcode= SQLDataSources(h->henv, SQL_FETCH_NEXT, szDSN, SQL_MAX_DSN_LENGTH + 1, &pcbDSN, szDesc, DS_DESCRIPTION_LENGTH, &pcbDesc);
		}

	}else{
		h = ODBCError("No such ODBC connection.", 0, "ODBC_DataSources", "1");
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
}

XS(XS_WIN32__ODBC_Drivers) 
{
	dXSARGS;
	ODBC_TYPE *h;
	UCHAR	szAttr[DS_DESCRIPTION_LENGTH + 1];
	SWORD	cbAttr;
	UCHAR	szDesc[DS_DESCRIPTION_LENGTH];
	SWORD	cbDesc;	
	char	*szTemp;
	RETCODE	retcode;

	if(items > 0){
		CROAK("usage: ($Err, $DSN) = ODBC_Drivers()\n");
	}
	
	h = ODBCList;
	PUSHMARK(sp);
	
	*szDesc = *szAttr = '\0';
	retcode = SQLDrivers(h->henv, SQL_FETCH_FIRST, szDesc, DS_DESCRIPTION_LENGTH, &cbDesc, szAttr, DS_DESCRIPTION_LENGTH, &cbAttr);
	if(retcode == SQL_SUCCESS){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		
		while (retcode == SQL_SUCCESS){
			szTemp = (char *) szAttr;
			while(szTemp[0] != '\0'){
				szTemp = strchr(szTemp, '\0');
				*szTemp++ = ';';
			}
			XPUSHs(sv_2mortal(newSVpv((char *)szDesc, strlen((char*)szDesc))));
			XPUSHs(sv_2mortal(newSVpv((char *)szAttr, strlen((char*)szAttr))));
			*szDesc = *szAttr = '\0';
			retcode = SQLDrivers(h->henv, SQL_FETCH_NEXT, szDesc, DS_DESCRIPTION_LENGTH, &cbDesc, szAttr, DS_DESCRIPTION_LENGTH, &cbAttr);
		}
	}else{
		h = ODBCError("No such ODBC connection.", 0, "ODBC_Drivers", "1");
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
}

XS(XS_WIN32__ODBC_RowCount) 
{
	dXSARGS;
	ODBC_TYPE *h;
	SDWORD	sdRows = 0L;	
	RETCODE	retcode;

	if(items != 1){
		CROAK("usage: ($Err, $NumOfRows) = ODBC_RowCount($Connection)\n");
	}
	
	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	PUSHMARK(sp);
	
	retcode = SQLRowCount(h->hstmt, &sdRows);
	if(retcode == SQL_SUCCESS){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)sdRows)));
	}else{
		h = ODBCError("No such ODBC connection.", 0, "ODBC_RowCount", "1");
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
}


XS(XS_WIN32__ODBC_Info) 
{
	dXSARGS;

	if(items > 0){
		CROAK("usage: ($ExtName, $Version, $Date, $Author, $CompileDate, $Credits) = Info()\n");
	}
	
	PUSHMARK(sp);
	
	XPUSHs(sv_2mortal(newSVpv(VERNAME, strlen(VERNAME))));
	XPUSHs(sv_2mortal(newSVpv(VERSION, strlen(VERSION))));
	XPUSHs(sv_2mortal(newSVpv(VERDATE, strlen(VERDATE))));
	XPUSHs(sv_2mortal(newSVpv(VERAUTH, strlen(VERAUTH))));
	XPUSHs(sv_2mortal(newSVpv(__DATE__, strlen(__DATE__))));
	XPUSHs(sv_2mortal(newSVpv(__TIME__, strlen(__TIME__))));
	XPUSHs(sv_2mortal(newSVpv(VERCRED, strlen(VERCRED))));

	PUTBACK;
}

XS(XS_WIN32__ODBC_GetStmtCloseType) 
{
	dXSARGS;
	ODBC_TYPE * h;
	long	iSize;
	char	*szType;

	if(items != 1){
		CROAK("usage: ($Err, $Type) = ODBC_GetStmtCloseType($Connection)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	PUSHMARK(sp);
	
	if (!(szType = MapCloseType(h->uStmtCloseType))){
		ODBCError("Invalid Statment Close Type", h, "ODBC_GetStmtCloseType", "1");
	}			
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVpv(szType, strlen(szType))));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
}

XS(XS_WIN32__ODBC_SetStmtCloseType) 
{
	dXSARGS;
	ODBC_TYPE * h;
	char	*szType;
	UWORD	uType;		
	
	if(items != 2){
		CROAK("usage: ($Err, $Type) = ODBC_SetStmtCloseType($Connection, $Type)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	uType = SvIV(ST(1));
	PUSHMARK(sp);

	switch(uType){
		case SQL_DROP:
		case SQL_CLOSE:
		case SQL_UNBIND:
		case SQL_RESET_PARAMS:
			h->uStmtCloseType = uType;
			if (!(szType = MapCloseType(h->uStmtCloseType))){
				ODBCError("Invalid Statment Close Type", h, "ODBC_SetStmtCloseType", "1");
			}
			break;

		default:
			ODBCError("Not a valid Stmt Close Type", h, "ODBC_SetStmtCloseType", "2");
	}					
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVpv(szType, strlen(szType))));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}


	PUTBACK;
}


XS(XS_WIN32__ODBC_SetConnectOption)
{
	dXSARGS;
	ODBC_TYPE * h;
    UWORD   uType;
    UDWORD  udValue;
    RETCODE rResult = 0;
	
    if(items != 3){
        CROAK("usage: ($Err, $Type) = ODBC_SetConnectOption($Connection, $Type, $Value)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    uType= SvIV(ST(1));
    if (SvIOKp(ST(2)) || SvNOKp(ST(2))){
    	udValue = SvIV(ST(2));
	}else{
		udValue = (UDWORD) SvPV(ST(2), na);
	}
	PUSHMARK(sp);

    if(!h->error){
        rResult = SQLSetConnectOption(h->hdbc, uType, udValue);
        if (rResult != SQL_SUCCESS){
 			_NT_ODBC_Error(h, "ODBC_SetConnectOption", "1");
        }
    }

	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)1)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}


	PUTBACK;
}

XS(XS_WIN32__ODBC_GetConnectOption)
{
	dXSARGS;
	ODBC_TYPE * h;
	UCHAR	ucValue[SQL_MAX_OPTION_STRING_LENGTH + 1];
	DWORD	*dValue = (DWORD *)ucValue;
    UWORD   uOption;
    RETCODE rResult = 0;
	
    if(items != 2){
        CROAK("usage: ($Err, $NumValue, $Value) = ODBC_GetConnectOption($Connection, $Type)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    uOption = SvIV(ST(1));
	PUSHMARK(sp);

    if(!h->error){
		memset(ucValue, 255, SQL_MAX_OPTION_STRING_LENGTH + 1);
        rResult = SQLGetConnectOption(h->hdbc, uOption, ucValue);
        if (rResult != SQL_SUCCESS){
 			_NT_ODBC_Error(h, "ODBC_GetConnectOption", "1");
        }
    }

	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		switch(uOption){
			case SQL_ACCESS_MODE:
			case SQL_AUTOCOMMIT:
			case SQL_LOGIN_TIMEOUT:
			case SQL_ODBC_CURSORS:		
			case SQL_OPT_TRACE:			
			case SQL_PACKET_SIZE: 			
			case SQL_QUIET_MODE:			
			case SQL_TRANSLATE_OPTION:
			case SQL_TXN_ISOLATION:		
					//	GetConnectOption returned a DWORD
				XPUSHs(sv_2mortal(newSVnv((double) *dValue)));
				break;

			default:
					//	GetConnectOption returned a string
				XPUSHs(sv_2mortal(newSVpv((char *)ucValue, strlen((char *) ucValue))));
				break;
		}
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	
	PUTBACK;
}

XS(XS_WIN32__ODBC_SetStmtOption)
{
	dXSARGS;
	ODBC_TYPE * h;
    UWORD   uType;
    UDWORD  udValue;
    RETCODE rResult = 0;
	
    if(items != 3){
        CROAK("usage: ($Err, $Type) = ODBC_SetStmtOption($Connection, $Type, $Value)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    uType= SvIV(ST(1));
    if (SvIOKp(ST(2)) || SvNOKp(ST(2))){
    	udValue = SvIV(ST(2));
	}else{
		udValue = (UDWORD) SvPV(ST(2), na);
	}
	PUSHMARK(sp);

    if(!h->error){
        rResult = SQLSetStmtOption(h->hstmt, uType, udValue);
        if (rResult != SQL_SUCCESS){
 			_NT_ODBC_Error(h, "ODBC_SetStmtOption", "1");
        }
    }

	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)1)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	PUTBACK;
}


XS(XS_WIN32__ODBC_GetStmtOption)
{
	dXSARGS;
	ODBC_TYPE * h;
	UCHAR	ucValue[SQL_MAX_OPTION_STRING_LENGTH + 1];
	DWORD	*dValue = (DWORD *)ucValue;
    UWORD   uOption;
    RETCODE rResult = 0;
	
    if(items != 2){
        CROAK("usage: ($Err, $NumValue, $Value) = ODBC_GetStmtOption($Connection, $Type)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    uOption = SvIV(ST(1));
	PUSHMARK(sp);

    if(!h->error){
		memset(ucValue, 255, SQL_MAX_OPTION_STRING_LENGTH + 1);
        rResult = SQLGetStmtOption(h->hstmt, uOption, ucValue);
        if (rResult != SQL_SUCCESS){
 			_NT_ODBC_Error(h, "ODBC_GetStmtOption", "1");
        }
    }

	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		switch(uOption){
					/*	Even though this function is basically the same
					  	as the ODBC_GetConnectOption the default on the
						select() is a DWORD since so few (actually as of 
						now NONE) of the options return strings. So the 
						list of strings returns (case: xxx) will be small.
					*/
			case 99999:			//	Something toatly unreasonable since I can't find a real string return.
					//	GetStmtOption returned a string
				XPUSHs(sv_2mortal(newSVpv((char *)ucValue, strlen((char *) ucValue))));
				break;

			default:
					//	GetStmtOption returned a DWORD
				XPUSHs(sv_2mortal(newSVnv((double) *dValue)));
				break;
		}
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	PUTBACK;
}

XS(XS_WIN32__ODBC_GetFunctions)
{
	dXSARGS;
	ODBC_TYPE * h;
	UWORD	uOutput[100];
    UWORD   uOption;
    RETCODE rResult = 0;
	int		iTemp, iTotal;
	
	iTemp = iTotal = 0;
    if(items < 1 || items > 101){
        CROAK("usage: ($Err, $NumValue, $Value) = ODBC_GetFunctions($Connection, ($Function1, $Function2 ... $Function100))\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	PUSHMARK(sp);

	items--;

		//	We decremented items, so if it's < 1 assume we want ALL functions!
	if (items < 1){
		uOption = SQL_API_ALL_FUNCTIONS;
		iTotal = 100;
		items = 1;
	}else{
		uOption = SvIV(ST(1));
	}

	while(items--){
		rResult = SQLGetFunctions(h->hdbc, uOption, &uOutput[iTemp]);
		if (rResult != SQL_SUCCESS){
			_NT_ODBC_Error(h, "ODBC_GetFunctions", "1");
			if(! strcmp((const char*) h->szSqlState, "00000")){
					//	The requested function number does not exist.	
				CleanError(h);
				uOutput[iTemp] = ~0;
			}else{
				items = 0;
			}
		}
		iTemp++;
		if (items){ 	//	If there are no more stack elements we will screw up
						//	trying to access ST(1 + iTemp)
			uOption = SvIV(ST(1 + iTemp));
		}
	}
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		if (!iTotal){
			 iTotal = iTemp;
		}
		for (iTemp = 0; iTemp < iTotal; iTemp++){
//			XPUSHs(sv_2mortal(newSVnv((double) iTemp)));
			XPUSHs(sv_2mortal(newSVnv((double) uOutput[iTemp])));
		}
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	
	PUTBACK;
}


XS(XS_WIN32__ODBC_Transact)
{
	dXSARGS;
	ODBC_TYPE * h;
    UWORD  uType;
    RETCODE rResult = 0;
	
    if(items != 2){
        CROAK("usage: ($Err, $Type) = ODBC_Transact($Connection, $Type)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    uType = SvIV(ST(1));
    PUSHMARK(sp);

	if(!h->error){
		if(uType == SQL_ROLLBACK || uType == SQL_COMMIT){
			rResult = SQLTransact(h->henv, h->hdbc, uType);
	        if (rResult != SQL_SUCCESS){
	 			_NT_ODBC_Error(h, "ODBC_Transact", "1");
	        }
		}else{
			ODBCError("Invalid Transaction Type", h, "ODBC_Transact", "2");
		}
    }

	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)1)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	PUTBACK;
}


XS(XS_WIN32__ODBC_ConfigDSN)
{
	dXSARGS;
	ODBC_TYPE * h;
    UWORD  uType;
	int		iStack = 0;
	char	*szDriver = 0;
	char	*szTemp = 0;
	char	*szTemp2 = 0;
	char	*szAttributes = 0;
    int		iResult = 0;
	int		iSize = 0;
	
    if(items < 3){
        CROAK("usage: ($Err, $Type) = ODBC_ConfigDSN($Connection, $Function, $Driver, @Attributes)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(iStack)));
	CleanError(h);
	iStack++;
    uType = SvIV(ST(iStack));
	iStack++;
	szDriver = SvPV(ST(iStack), na);
	if (strlen(szDriver) == 0){
		szDriver = 0;
	}
	iStack++;
	items -= iStack;		//	Reduce the # of items by 3 (compensate for connection, function and driver)

					//	Remember: when starting szAttributes = 0, not ""
	while (items--){
		szTemp = SvPV(ST(iStack), na);
		if(strcspn(szTemp, "[]{}(),?*!@;") < strlen(szTemp)){
			ODBCError("Illegal use of reserved characters []{}(),?*!@;", h, "ODBC_ConfigDSN", "1");
			break;
		}

		
		iStack++;
		iSize += strlen(szTemp) + 2;
		if (! (szTemp2 = new char [iSize])){
			ODBCError("Could not allocate memory for the attribute list", h, "ODBC_ConfigDSN", "2");
			items = 0;
		}else{
			if (szAttributes) {
				strcpy(szTemp2, szAttributes);
			}else{
				strcpy(szTemp2, "");
			}
			strcat(szTemp2, szTemp);
			strcat(szTemp2, ";");
			if (szAttributes){
				delete [] szAttributes;
			}
			szAttributes = szTemp2;
			szTemp2 = 0;
		}
	}

	PUSHMARK(sp);
		/*	
			As of now szAttributes contains the entire DSN
		*/

	if (! h->error){
		if(strcspn(szAttributes, "[]{}(),?*!@") < strlen(szAttributes)){
			ODBCError("Illegal use of reserved characters []{}(),?*!@", h, "ODBC_ConfigDSN", "3");
		}

				//	Format Attribute list for the ConfigDataSource function...
				//	attrib1=value1\0attrib2=value2\0attrib3=value3\0\0
		for (;iSize > -1; iSize--){
			if (szAttributes[iSize] == ';'){
				szAttributes[iSize] = '\0';
			}
		}
		iResult = SQLConfigDataSource(0, (UINT) uType, (LPCSTR) szDriver, (LPCSTR) szAttributes);
	}
	
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)iResult)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	if (szAttributes){
		delete [] szAttributes;
	}

	PUTBACK;
}


XS(XS_WIN32__ODBC_GetInfo)
{
	dXSARGS;
	ODBC_TYPE * h;
	UCHAR	*ucValue = 0;
	int		iSize = sizeof(DWORD) + 1;		//	Arbitrary size to allocate for result
	int		iFlag = 0;
	DWORD	*dValue;
	UWORD	*uValue;
    UWORD   uType;
	SWORD	swBytes = 0;
    RETCODE rResult = 0;
	
    if(items != 2){
        CROAK("usage: ($Err, $NumValue, $Value) = ODBC_GetInfo($Connection, $Type)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    uType = SvIV(ST(1));
	PUSHMARK(sp);

    if(!h->error){
		while (!iFlag++){
			if (ucValue = (UCHAR *) new char [iSize]){
					//	Set the memory set to $FF so we can tell if the return value
					//	is a string. It will have a \0 at the end of it.
				memset(ucValue, 255, iSize);	
				dValue = (DWORD *) ucValue;
				uValue = (UWORD *) ucValue;
		        rResult = SQLGetInfo(h->hdbc, uType, ucValue, (SWORD) iSize, &swBytes);
		        if (rResult != SQL_SUCCESS){
		 			_NT_ODBC_Error(h, "ODBC_GetInfo", "1");
		        }
			}else{
				ODBCError("Could not allocate memory for result string", h, "ODBC_GetInfo", "2");
				ucValue = 0;
			}
			if (iSize <= (int) swBytes){
				if(ucValue){delete [] ucValue;}
				iSize = (int) swBytes + 1;
				iFlag = 0;
				CleanError(h);
			}
		} while (iSize <= (int) swBytes);
    }

	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		if (ucValue[swBytes] == 0){
				//	GetInfo() returned a string!
			XPUSHs(sv_2mortal(newSVpv((char *) ucValue, (int) swBytes)));
		}else{
			switch (swBytes){	
				case 2:			//	We must have a 16 bit value
					XPUSHs(sv_2mortal(newSVnv((double) *uValue)));
					break;

				case 4:			//	We must have a 32 bit value
					XPUSHs(sv_2mortal(newSVnv((double) *dValue)));
					break;
			}
		}
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	if (ucValue){
		delete [] ucValue;
	}
	PUTBACK;
}

XS(XS_WIN32__ODBC_CleanError) 
{
	dXSARGS;
	ODBC_TYPE * h;
	
	if(items != 1){
		CROAK("usage: ($Err) = ODBC_CleanError($Connection)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	
	PUSHMARK(sp);
   
	CleanError(h);
	XPUSHs(sv_2mortal(newSVnv((double)0)));
	XPUSHs(sv_2mortal(newSVnv((double)1)));
	PUTBACK;
}

  
XS(XS_WIN32__ODBC_ColAttributes) 
{
	dXSARGS;
	ODBC_TYPE * h;
	UWORD	iCol = 0;
	UWORD	iType = 0;
	int		iTemp;
	UCHAR	*szName = 0;
	UCHAR	szBuff[ODBC_BUFF_SIZE];
	SWORD	dBuffLen = 0;
	SDWORD	dValue = 0;
	RETCODE	rResult;
												
	if(items != 3){
		CROAK("usage: ($Err) = ODBC_ColAttributes($Connection, $ColName, $Description)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	szName = (unsigned char *) SvPV(ST(1), na);
	iType = SvIV(ST(2));
	
	PUSHMARK(sp);
   
	CleanError(h);
	if (!h->error){
	    if (iCol = ColNameToNum(h, (char *)szName)){
			memset(szBuff, '\0', ODBC_BUFF_SIZE);
			rResult = SQLColAttributes(h->hstmt, iCol, iType, szBuff, ODBC_BUFF_SIZE, (short *) &dBuffLen, (SDWORD *) &dValue);
			if (rResult == SQL_SUCCESS || rResult == SQL_SUCCESS_WITH_INFO){
				if (dBuffLen){
					XPUSHs(sv_2mortal(newSVnv((double)0)));
					XPUSHs(sv_2mortal(newSVpv((char *)szBuff, dBuffLen)));
				}else{
					XPUSHs(sv_2mortal(newSVnv((double)0)));
					XPUSHs(sv_2mortal(newSVnv((double)dValue)));
				}
			}else{
				ODBCError("Unable to determine Column Attribute", h, "ODBC_ColAttributes", "1");
			}
		}else{
			ODBCError("Field name does not exist", h, "ODBC_ColAttributes", "2");
		}
	}
	 		//	Process only an error state. If this has been successfull the values have
			//	already been processed.
	if (h->error){	
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));		
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv((char *)h->szError, strlen((char *) h->szError))));
	}
	PUTBACK;
}


XS(XS_WIN32__ODBC_Debug) 
{
	dXSARGS;
	ODBC_TYPE * h;
	int	iDebug = 0;
												
	if(items < 1 || items > 2){
		CROAK("usage: $Debug = ODBC_Debug($Connection [, $Value])\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);

	if (items > 1 && !h->error){
		iDebug = SvIV(ST(1));
		h->iDebug = (iDebug)? 1:0;
	}
	
	PUSHMARK(sp);
   
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)h->iDebug)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv((char *)h->szError, strlen((char *) h->szError))));
	}

	PUTBACK;
}

XS(XS_WIN32__ODBC_SetPos)
{
	dXSARGS;
	ODBC_TYPE * h;
    UWORD	uRow = 1;
    UWORD	uOption = SQL_POSITION;
    UWORD	uLock = SQL_LOCK_NO_CHANGE;
    RETCODE rResult = 0;
	
    if(items < 2 || items > 4){
        CROAK("usage: ($Err, $Type) = ODBC_SetPos($Connection, $Row [, $Option, $Lock})\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    uRow 	= SvIV(ST(1));
	if (items > 2){
		uOption = SvIV(ST(2));
	}
	if (items > 3){
		uLock 	= SvIV(ST(3));
	}
    PUSHMARK(sp);

	if(!h->error){
		rResult = SQLSetPos(h->hstmt, uRow, uOption, uLock);
	    if (rResult != SQL_SUCCESS){
			_NT_ODBC_Error(h, "ODBC_SetPos", "1");
				//	Set h->EOR so that Fetch() knows the error is not because
				//	of itself.  What a hack!
			h->EOR = 1;
	    }
    }

	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)1)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	PUTBACK;
}

XS(XS_WIN32__ODBC_GetData)
{
	dXSARGS;
	ODBC_TYPE * h;
	RETCODE retcode;         // yet more ODBC garbage
	UCHAR  buff2[ODBC_BUFF_SIZE];
	UCHAR	*szBuf = 0;
	SDWORD	iBuf = DEFAULTCOLSIZE;
	SDWORD	bufflenout;
	SWORD	sSQLType;
	SWORD	sTemp;
	int lenn, iTotalPushed, iTemp;
	UWORD  x;
	int len;
	DWORD	dTemp;
	
    if(items != 1){
        CROAK("usage: ($Err, $Type) = ODBC_GetData($Connection)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    
    PUSHMARK(sp);
    
    if (!h->error && !h->EOR){
			//	Allocate a default size to be used. This prevents the problem of ODBC 
			//	returning too small of a size for a regular numberical field.
			//	ALSO it saves some allocation time, hopefully the average field
			//	will fit into this size.
			//											rothd@roth.net	96.05.03	
		if (!(szBuf = new UCHAR [iBuf])){
			sprintf((char *)buff2, "Could not allocate enough memory (%d bytes) for default column size.\n", DEFAULTCOLSIZE);
			ODBCError((char *)buff2, h, "ODBC_GetData", "1");
		}
	}	
	if (!h->error){		 // everything is happy
		iTotalPushed = 1;
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		for(x=1; x<=h->numcols; x++){
			if (h->error){
				break;
			}
						/*
							Needed to change SQL_COLUMN_LENGTH to SQL_COLUMN_DISPLAY_SIZE. The
							former found the # of bytes needed to represent the C data type
							in memory. The latter is used to find the # of bytes needed to store
							an ASCII version of the data. 
							Way big Thanks to Mike Knister <knister@sierra.net> 96.05.13
						*/
			if (SQLColAttributes(h->hstmt, x, SQL_COLUMN_DISPLAY_SIZE, NULL, NULL, NULL, &bufflenout) == SQL_SUCCESS){
						/*
							Removed the ++bufflenout to prevent the wrap-around bug
							(when bufflenout = 2147483647, adding 1 yields -2147483648. Bad, very bad.
							A whopping big thanks to Jutta M. Klebe <jmk@exc.bybyte.de>	96.04.21
						*/
						/*	Changed (iBuf < bufflenout) to (iBuf <= bufflenout). How silly! if we
							happen to have returning a field of size iBuf there will be no place 
							for the \0
								96.10.07 <rothd@roth.net>
						*/
				if (iBuf <= bufflenout){
					if (szBuf){	
						delete [] szBuf; 	//	Why is this causing a heap error R6018?
						szBuf = 0;
						iBuf = 0;
					}
					if (bufflenout > h->iMaxBufSize){
						bufflenout = h->iMaxBufSize + 1;
					}
								//	[bufflenout + 1]  -- Jutta M. Klebe <jmk@exc.bybyte.de>	96.04.21
					if (!(szBuf = new UCHAR [bufflenout + 1])){
						sprintf((char *)buff2, "Could not allocate enough memory (%d bytes) for column %d.\n", (bufflenout + 1), x);
						ODBCError((char *)buff2, h, "ODBC_GetData", "2");
						for(iTemp = 0; iTemp < iTotalPushed; iTemp++){
							POPs;
						}
						continue;
					}else{
						iBuf = bufflenout + 1; //	... + 1 -- Jutta M. Klebe <jmk@exc.bybyte.de>	96.04.21
					}
				}
			}
			memset( szBuf, 0xff, iBuf );		//	Thanks to "Jutta M. Klebe" <jmk@exc.bybyte.de>
			// if (SQLColAttributes(h->hstmt, x, SQL_COLUMN_DISPLAY_SIZE, NULL, NULL, NULL, &bufflenout) == SQL_SUCCESS){
			SQLGetData(h->hstmt, x, SQL_C_CHAR, szBuf, (SDWORD)iBuf, &bufflenout); 
			SQLDescribeCol(h->hstmt, x, 0, 0, &sTemp, &sSQLType,(UDWORD *) &bufflenout, &sTemp, &sTemp);
			dTemp = 0;
			if ((sSQLType == SQL_LONGVARBINARY) || (sSQLType == SQL_BINARY) || (sSQLType == SQL_VARBINARY)){
					//	If the data was binary we need to deal with if differently
					//	since it may have embedded NULLs
					//	A Binary data byte is represented by by two ASCII chars
					//	0-F. The two chars together make up an 8 bit number eg:1F
					//	We must take the first char, roll it 4 bits then take the
					//	next char, strip off the high 4 bits and OR it with the
					//	previous byte. We now have one data byte. Cool.
				for(iTemp = 0; szBuf[iTemp] != '\0'; iTemp =+2){
					dTemp++;
					//	Since the binary is represented as a text string (2x the size
					//	of the binary data) let's just assume that szBuf[iTemp] (on the odd numbers)
					//	will eventually hold the terminating NULL of the string.
					szBuf[iTemp] << 4;
					szBuf[iTemp] |= (szBuf[iTemp + 1] & 0x0F);
				}
			}else{
				dTemp = strlen((const char *)szBuf);
			}
			XPUSHs(sv_2mortal(newSVpv((char *)szBuf, dTemp)));
			iTotalPushed++;
		}
	}else{										
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	if (szBuf){
		delete [] szBuf;
	}
	PUTBACK;
} //ODBC_GetData

XS(XS_WIN32__ODBC_DropCursor)
{
	dXSARGS;
	ODBC_TYPE * h;
	UWORD		uCloseType;
    RETCODE 	retcode = 0;
	
    if(items != 1){
        CROAK("usage: ($Err, $Type) = ODBC_DropCursor($Connection)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
    PUSHMARK(sp);

	if(!h->error){
		uCloseType = h->uStmtCloseType;
		h->uStmtCloseType = SQL_DROP;
		retcode = ResetStmt(h);
		h->uStmtCloseType = uCloseType;
	}
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)1)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	PUTBACK;
}

XS(XS_WIN32__ODBC_SetCursorName)
{
	dXSARGS;
	ODBC_TYPE * h;
	char	*szName;
    RETCODE retcode = 0;
	
    if(items != 2){
        CROAK("usage: ($Err, $Type) = ODBC_SetCursorName($Connection, $Name)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);
	szName = SvPV(ST(1), na); 

	PUSHMARK(sp);

	if(!h->error){
		retcode = SQLSetCursorName(h->hstmt, (UCHAR *)szName, (SWORD)strlen(szName));
		if (retcode != SQL_SUCCESS){
			_NT_ODBC_Error(h, "ODBC_SetCursorName", "1");
		}
	}
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)1)));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	PUTBACK;
}		

XS(XS_WIN32__ODBC_GetCursorName)
{
	dXSARGS;
	ODBC_TYPE * h;
	char	*szName = 0;
	SWORD	sSize = 0;
    RETCODE retcode = 0;
	
    if(items != 1){
        CROAK("usage: ($Err, $Type) = ODBC_GetCursorName($Connection)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	CleanError(h);

	PUSHMARK(sp);

	if(!h->error){
		if(SQL_SUCCESS == SQLGetCursorName(h->hstmt, (UCHAR *)szName, sSize, &sSize)){
			if(!(szName = new char [++sSize])){
				ODBCError("Could not allocate memory", h, "ODBC_GetCursorName", "1");
			}else{
				if(SQL_SUCCESS != SQLGetCursorName(h->hstmt, (UCHAR *)szName, sSize, &sSize)){
					_NT_ODBC_Error(h, "ODBC_GetCursorName", "2");
				}
			}
		}else{
			_NT_ODBC_Error(h, "ODBC_GetCursorName", "3");
		}
	}
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVpv(szName, strlen(szName))));
	}else{
			//	Report the error
		XPUSHs(sv_2mortal(newSVnv((double)1)));	
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	if(szName){
		delete [] szName;
	}
	PUTBACK;
}		

XS(boot_Win32__ODBC)
{
	dXSARGS;
	char* file = __FILE__;
	int i;
	RETCODE	retcode = 1;

			//	Start the ODBC Connection Chain!
	CleanODBC(ODBCList);
	ODBCTotal=1;
	ODBCList->conn = ODBC_Conn_Number++;
	ODBCList->Prev = 0;
	ODBCList->Next = 0;

		//	Allocate the MASTER hEnv. This will be used for ALL connections.
	retcode = SQLAllocEnv(&ODBCList->henv);
	if (retcode != SQL_SUCCESS){							   
		ODBCError("Ran out of memory allocating ODBC enviroment", ODBCList, "ODBC_BootStrap", "1");
		retcode = 0;
	}else{
		retcode = 1;
	}

	newXS("Win32::ODBC::constant",				XS_WIN32__ODBC_Constant, file);
	newXS("Win32::ODBC::ODBCConnect",			XS_WIN32__ODBC_Connect,  file);
    newXS("Win32::ODBC::ODBCExecute",			XS_WIN32__ODBC_Execute, file);
    newXS("Win32::ODBC::ODBCFetch",				XS_WIN32__ODBC_Fetch, file);
    newXS("Win32::ODBC::ODBCDisconnect",		XS_WIN32__ODBC_Disconnect, file);
	newXS("Win32::ODBC::ODBCGetError",			XS_WIN32__ODBC_GetError, file);
	newXS("Win32::ODBC::ODBCTableList",			XS_WIN32__ODBC_TableList, file);
	newXS("Win32::ODBC::ODBCMoreResults",		XS_WIN32__ODBC_MoreResults, file);
	
	newXS("Win32::ODBC::ODBCGetConnections",	XS_WIN32__ODBC_GetConnections, file);
	newXS("Win32::ODBC::ODBCGetMaxBufSize",		XS_WIN32__ODBC_GetMaxBufSize, file);
	newXS("Win32::ODBC::ODBCSetMaxBufSize",		XS_WIN32__ODBC_SetMaxBufSize, file);
	newXS("Win32::ODBC::ODBCGetStmtCloseType",	XS_WIN32__ODBC_GetStmtCloseType, file);
	newXS("Win32::ODBC::ODBCSetStmtCloseType",	XS_WIN32__ODBC_SetStmtCloseType, file);
	
	newXS("Win32::ODBC::ODBCDataSources",		XS_WIN32__ODBC_DataSources, file);
	newXS("Win32::ODBC::ODBCDrivers",			XS_WIN32__ODBC_Drivers, file);
	
	newXS("Win32::ODBC::ODBCGetStmtOption",		XS_WIN32__ODBC_GetStmtOption, file);
	newXS("Win32::ODBC::ODBCSetStmtOption",		XS_WIN32__ODBC_SetStmtOption, file);
    newXS("Win32::ODBC::ODBCSetConnectOption",  XS_WIN32__ODBC_SetConnectOption, file);
    newXS("Win32::ODBC::ODBCGetConnectOption",  XS_WIN32__ODBC_GetConnectOption, file);
	
	newXS("Win32::ODBC::ODBCRowCount",			XS_WIN32__ODBC_RowCount, file);
	newXS("Win32::ODBC::ODBCCleanError",		XS_WIN32__ODBC_CleanError, file);
	newXS("Win32::ODBC::Info",					XS_WIN32__ODBC_Info, file);
	newXS("Win32::ODBC::ODBCColAttributes",		XS_WIN32__ODBC_ColAttributes, file);									   
	newXS("Win32::ODBC::ODBCConfigDSN",			XS_WIN32__ODBC_ConfigDSN, file);									   
	newXS("Win32::ODBC::ODBCGetFunctions",  	XS_WIN32__ODBC_GetFunctions, file);
	newXS("Win32::ODBC::ODBCTransact",			XS_WIN32__ODBC_Transact, file);
	newXS("Win32::ODBC::ODBCGetDSN",			XS_WIN32__ODBC_GetDSN, file);
	newXS("Win32::ODBC::ODBCGetInfo",			XS_WIN32__ODBC_GetInfo, file);
	newXS("Win32::ODBC::ODBCDebug",				XS_WIN32__ODBC_Debug, file);
	newXS("Win32::ODBC::ODBCSetPos",			XS_WIN32__ODBC_SetPos, file);
	newXS("Win32::ODBC::ODBCGetData",			XS_WIN32__ODBC_GetData, file);
	newXS("Win32::ODBC::ODBCDropCursor",		XS_WIN32__ODBC_DropCursor, file);
	newXS("Win32::ODBC::ODBCSetCursorName",		XS_WIN32__ODBC_SetCursorName, file);
	newXS("Win32::ODBC::ODBCGetCursorName",		XS_WIN32__ODBC_GetCursorName, file);
		
	ST(0) = &sv_yes;
	XSRETURN(retcode);

}			

/* ===============  DLL Specific  Functions  ===================  */
			  
BOOL WINAPI DllMain(HINSTANCE  hinstDLL, DWORD fdwReason, LPVOID  lpvReserved){
	BOOL	bResult = 1;

	switch(fdwReason){
		case DLL_PROCESS_ATTACH:
			ghDLL = hinstDLL;
			break;

		case DLL_THREAD_ATTACH:
			giThread++;
			break;
			 
		case DLL_THREAD_DETACH:
			giThread--;
			break;
			 	
		case DLL_PROCESS_DETACH:
			/*
				//	Detach and delete ALL ODBC connections.
			DeleteConn(0);		
				//	Finally delete the Master Enviroment and ODBCList.	
			FreeODBC(ODBCList);	
			*/
			if (ODBCList){
				if(ODBCList->iDebug){
					MessageBeep(MB_OK);
				}
			}
			break;

		default:
			break;
	}
	return bResult;
}

/*
	To do list:
		-Convert this beast to OOP.
		-Create classes of reusable hEnv, hDbc, hStmt so we do not
		 waste memory and connections.
		done -Configure/alter DSNs.
		done -Return an array of values that describe the total connection 
		 string for a DSN. Similar to DataSources($DSN) but return the 
		 connection string not just the driver string.
		-return values that can have embedded nulls (binary data).

*/
