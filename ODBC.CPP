/*
        +==========================================================+
        |                                                          |
        |              ODBC extension for Win32 Perl               |
        |              -----------------------------               |
        |                                                          |
        |            by Dave Roth (rothd@roth.net)             |
        |                                                          |
        |                  version v960722 (hack)                  |
        |                                                          |
        |    Copyright (c) 1996 Dave Roth. All rights reserved.    |
        |   This program is free software; you can redistribute    |
        | it and/or modify it under the same terms as Perl itself. |
        |                                                          |
        +==========================================================+


          based on original code by Dan DeMaggio (dmag@umich.edu)

   Use under GNU General Public License or Larry Wall's "Artistic License"
*/

#include <windows.h>
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <SQL.H>
#include <SQLEXT.H>
#include <EXTERN.h>
#include "perl.h"
#include "XSub.h"
#include "odbc.h"		   

/*----------------------- P E R L   F U N C T I O N S -------------------*/

// constant function for exporting NT definitions.
static long constant(CPerl* pPerl, char *name)
{
    errno = 0;
    switch (*name) {
	    case 'A':
			break;
    	case 'B':
			break;
    	case 'C':
			break;
    	case 'D':
			break;
    	case 'E':
			break;
	    case 'F':
			break;
	    case 'G':
			break;
	    case 'H':
			break;
	    case 'I':
			break;
	    case 'J':
			break;
	    case 'K':
			break;
	    case 'L':
			break;
	    case 'M':
			break;
	    case 'N':
			break;
	    case 'O':
			break;
	    case 'P':
			break;
	    case 'Q':
			break;
	    case 'R':
			break;
	    case 'S':
			if (strEQ(name, "SQL_CLOSE"))
#ifdef SQL_CLOSE
				return SQL_CLOSE;
#else
				goto not_there;
#endif
			if (strEQ(name, "SQL_DROP"))
#ifdef SQL_DROP
			    return SQL_DROP;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_ERROR"))
#ifdef SQL_ERROR
			    return SQL_ERROR;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_INVALID_HANDLE"))
#ifdef SQL_INVALID_HANDLE
			    return SQL_INVALID_HANDLE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_NO_DATA_FOUND"))
#ifdef SQL_NO_DATA_FOUND
			    return SQL_NO_DATA_FOUND;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_UNBIND"))
#ifdef SQL_UNBIND
			    return SQL_UNBIND;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_RESET_PARAMS"))
#ifdef SQL_RESET_PARAMS
			    return SQL_RESET_PARAMS;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_SUCCESS"))
#ifdef SQL_SUCCESS
			    return SQL_SUCCESS;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_SUCCESS_WITH_INFO"))
#ifdef SQL_SUCCESS_WITH_INFO
			    return SQL_SUCCESS_WITH_INFO;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_COUNT"))
#ifdef SQL_COLUMN_COUNT
			    return SQL_COLUMN_COUNT;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_NAME"))
#ifdef SQL_COLUMN_NAME
			    return SQL_COLUMN_NAME;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_TYPE"))
#ifdef SQL_COLUMN_TYPE
			    return SQL_COLUMN_TYPE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_LENGTH"))
#ifdef SQL_COLUMN_LENGTH
			    return SQL_COLUMN_LENGTH;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_PRECISION"))
#ifdef SQL_COLUMN_PRECISION
			    return SQL_COLUMN_PRECISION;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_SCALE"))
#ifdef SQL_COLUMN_SCALE
			    return SQL_COLUMN_SCALE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_DISPLAY_SIZE"))
#ifdef SQL_COLUMN_DISPLAY_SIZE
			    return SQL_COLUMN_DISPLAY_SIZE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_NULLABLE"))
#ifdef SQL_COLUMN_NULLABLE
			    return SQL_COLUMN_NULLABLE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_UNSIGNED"))
#ifdef SQL_COLUMN_UNSIGNED
			    return SQL_COLUMN_UNSIGNED;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_MONEY"))
#ifdef SQL_COLUMN_MONEY
			    return SQL_COLUMN_MONEY;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_UPDATABLE"))
#ifdef SQL_COLUMN_UPDATABLE
			    return SQL_COLUMN_UPDATABLE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_AUTO_INCREMENT"))
#ifdef SQL_COLUMN_AUTO_INCREMENT
			    return SQL_COLUMN_AUTO_INCREMENT;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_CASE_SENSITIVE"))
#ifdef SQL_COLUMN_CASE_SENSITIVE
			    return SQL_COLUMN_CASE_SENSITIVE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_SEARCHABLE"))
#ifdef SQL_COLUMN_SEARCHABLE
			    return SQL_COLUMN_SEARCHABLE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_TYPE_NAME"))
#ifdef SQL_COLUMN_TYPE_NAME
			    return SQL_COLUMN_TYPE_NAME;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_TABLE_NAME"))
#ifdef SQL_COLUMN_TABLE_NAME
			    return SQL_COLUMN_TABLE_NAME;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_OWNER_NAME"))
#ifdef SQL_COLUMN_OWNER_NAME
			    return SQL_COLUMN_OWNER_NAME;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_QUALIFIER_NAME"))
#ifdef SQL_COLUMN_QUALIFIER_NAME
			    return SQL_COLUMN_QUALIFIER_NAME;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_LABEL"))
#ifdef SQL_COLUMN_LABEL
			    return SQL_COLUMN_LABEL;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLATT_OPT_MAX"))
#ifdef SQL_COLATT_OPT_MAX
			    return SQL_COLATT_OPT_MAX;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_COLUMN_DRIVER_START"))
#ifdef SQL_COLUMN_DRIVER_START
			    return SQL_COLUMN_DRIVER_START;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_ATTR_READONLY"))
#ifdef SQL_ATTR_READONLY
			    return SQL_ATTR_READONLY;
#else
	    		goto not_there;
#endif

			if (strEQ(name, "SQL_ATTR_WRITE"))
#ifdef SQL_ATTR_WRITE
			    return SQL_ATTR_WRITE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_ATTR_READWRITE_UNKNOWN"))
#ifdef SQL_ATTR_READWRITE_UNKNOWN
			    return SQL_ATTR_READWRITE_UNKNOWN;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_UNSEARCHABLE"))
#ifdef SQL_UNSEARCHABLE
			    return SQL_UNSEARCHABLE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_LIKE_ONLY"))
#ifdef SQL_LIKE_ONLY
			    return SQL_LIKE_ONLY;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_LIKE_ONLY"))
#ifdef SQL_LIKE_ONLY
			    return SQL_LIKE_ONLY;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_ALL_EXCEPT_LIKE"))
#ifdef SQL_ALL_EXCEPT_LIKE
			    return SQL_ALL_EXCEPT_LIKE;
#else
	    		goto not_there;
#endif
			if (strEQ(name, "SQL_SEARCHABLE"))
#ifdef SQL_SEARCHABLE
			    return SQL_SEARCHABLE;
#else
	    		goto not_there;
#endif
 			break;
	    case 'T':
			break;
	    case 'U':
			break;
	    case 'V':
			break;
	    case 'W':
			break;
	    case 'X':
			break;
	    case 'Y':
			break;
	    case 'Z':
			break;
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}

XS(XS_WIN32__ODBC_Constant)
{
	dXSARGS;
	if (items != 2)
	{
		croak("Usage: Win23::ODBC::Constant(name, arg)\n");
    }
	{
		char* name = (char*)SvPV(ST(0),na);
		ST(0) = sv_newmortal();
		sv_setiv(ST(0), constant(pPerl, name));
	}
	XSRETURN(1);
}


/*----------------------- M I S C   F U N C T I O N S -------------------*/

	//	Set up (and clean) a new connection						   	
ODBC_TYPE *CleanODBC(ODBC_TYPE *h){
	h->hstmt = SQL_NULL_HSTMT;
	h->henv  = SQL_NULL_HENV;
	h->hdbc  = SQL_NULL_HDBC;
	h->iMaxBufSize = DEFAULT_DATA_BUF_SIZE;
	h->numcols = 0;
	h->uStmtCloseType = DEFAULT_STMT_CLOSE_TYPE;
	strcpy(h->szDSN, "");
	strcpy(h->szCommand, "");
	CleanError(h);
	return h;
}

	//	Allocate memory for a new connection
ODBC_TYPE *NewODBC(){
	ODBC_TYPE *h;

	if (!(h = new (ODBC_TYPE))){
		ODBCError("Could not allocate memory of an ODBC connection\n", 0);
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
		h->conn = ODBC_Conn_Number++;
		CleanODBC(h);
	}
	return h;
}

void CleanError(ODBC_TYPE *h){
	strcpy(h->szError, "");
	strcpy((char *)h->szSqlState, "");
	h->error = 0;
	h->EOR = 0;
	return;
}

RETCODE ResetStmt(ODBC_TYPE *h){
	RETCODE	iReturnCode, iTemp = 0;
  			//	If the SQLFreeStmt() failed, should we reallocate the stmt? 
			//	For now let's just return the error code and skip the reallocation.
	if (h->hstmt != SQL_NULL_HSTMT){
		iReturnCode = SQLFreeStmt(h->hstmt, h->uStmtCloseType);
		if (iReturnCode == SQL_SUCCESS){
			h->hstmt = SQL_NULL_HSTMT;
		}else{
			_NT_ODBC_Error(h);
		}
	}
	if (h->hstmt == SQL_NULL_HSTMT){
		if ((iReturnCode = SQLAllocStmt(h->hdbc, &h->hstmt)) != SQL_SUCCESS){
			_NT_ODBC_Error(h);
		}else{
			if (iTemp = SQLSetStmtOption(h->hstmt, SQL_QUERY_TIMEOUT, (UDWORD)QUERY_TIMEOUT_VALUE) != SQL_SUCCESS){
				_NT_ODBC_Error(h);
				h->error = 0;
			}
		}
	}
	return iReturnCode;
}
	


void _NT_ODBC_Error(ODBC_TYPE * h){
	SDWORD cbErrorMsg;

	SQLError(h->henv, h->hdbc, h->hstmt, (UCHAR *)h->szSqlState, (long *)&(h->error), (unsigned char *)h->szError, ODBC_BUFF_SIZE, (short *)&cbErrorMsg);
	if (!h->error) h->error++;
}

ODBC_TYPE *ODBCError(char *szString, ODBC_TYPE *h){
	if(h == 0){
		h = ODBCList;
	}
	h->error = 911;
	strcpy(h->szError, szString);
	return h;
}

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


	//	Check if the specified connection is valid
ODBC_TYPE * _NT_ODBC_Verify(int iODBC){
	ODBC_TYPE *hTemp;

	ODBC_errornum = 0;
	hTemp = ODBCList;
		//	Walk down the linked list looking for the ODBC Connection
	while (hTemp != 0){
		if (hTemp->conn == iODBC){
			if (hTemp->EOR){
				hTemp->error = 0;
				hTemp->EOR = 0;
				strcpy(hTemp->szError, "");
			}
			return(hTemp);			
		}
		hTemp = (ODBC_TYPE *)hTemp->Next;
	}
	ODBCError("No such ODBC connection.", 0);
	return (ODBCList);
}

	//	Remove allocated memory for a connection
void DeleteConn(int conn){
	ODBC_TYPE	*h, *hTemp;
	int	iAll = 0;
	
	if (!conn){
		iAll = 1;
	}
	if (conn > 0 || iAll == 1){
		h = (ODBC_TYPE *) ODBCList->Next;
		while(h != 0){
			hTemp = (ODBC_TYPE *)h->Next;
			if (iAll){
				conn = h->conn;
			}
			if (h->conn == conn){
				FreeODBC(h);
				if (h->Prev != 0){
					(h->Prev)->Next = h->Next;
				}
				if (h->Next !=0){
					(h->Next)->Prev = h->Prev;
				}
				delete h;
			}
			h = hTemp;
		}
	}
}

void FreeODBC(ODBC_TYPE *h){	
	if (h->hstmt) SQLFreeStmt(h->hstmt, SQL_DROP);
	if (h->hdbc) SQLDisconnect(h->hdbc);
	if (h->hdbc) SQLFreeConnect(h->hdbc);
	if (h->henv) SQLFreeEnv(h->henv);
	h->hstmt = SQL_NULL_HSTMT;
	h->henv  = SQL_NULL_HENV;
	h->hdbc  = SQL_NULL_HDBC;
	return;
}

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

	if(items != 1){
		CROAK("usage: ($Connection, $Err, $ErrText) = Connect(\"DSN\");  # DSN must already have been created via ODBC tools\n       ($Connection, $Err, $ErrText) = ODBC_Connect($Source); #$Source is a full DSN (\"DSN=XXX; User='YYY'; ...\"\n");
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
		h = ODBCError("Could not allocate memory of an ODBC connection\n", 0);
	}else{
		strcpy(h->szDSN, szIn);
		retcode = SQLAllocEnv(&(h->henv));
		if (retcode != SQL_SUCCESS){							   
			ODBCError("Ran out of memory allocating ODBC enviroment", h);
			DeleteConn(h->conn);			
		}
	 	if (!h->error){
			retcode = SQLAllocConnect(h->henv, &h->hdbc);
			if (retcode != SQL_SUCCESS)
			{
				_NT_ODBC_Error(h);
				SQLFreeEnv(h->henv);
				DeleteConn(h->conn);
			}
		}
		if (!h->error){
			retcode = SQLSetConnectOption(h->hdbc, SQL_LOGIN_TIMEOUT, (UDWORD)LOGIN_TIMEOUT_VALUE);
 			if (retcode != SQL_SUCCESS){
				_NT_ODBC_Error(h);
				h->error = 0;
			}	  
			retcode = SQLDriverConnect(h->hdbc, (HWND) NULL, (unsigned char *)szDSN, strlen(szDSN), buff, ODBC_BUFF_SIZE, (short *)&bufflenout, SQL_DRIVER_NOPROMPT);
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			{
				_NT_ODBC_Error(h);
				SQLFreeConnect(h->hdbc);
				SQLFreeEnv(h->henv);
				DeleteConn(h->conn);
			}
			strcpy(h->szDSN, (char *) buff);
			if (retcode == SQL_SUCCESS_WITH_INFO){
				_NT_ODBC_Error(h);
				h->error = 0;
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
		XPUSHs(sv_2mortal(newSVnv((double)h->conn)));
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, 0)));
	}else{
		XPUSHs(sv_2mortal(newSVnv((double)0)));
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
	
	PUSHMARK(sp);

	if (h != ODBCList){
		szSQL = SvPV(ST(1), na);            // get SQL string
		CleanError(h);
//		retcode = SQLFreeStmt(h->hstmt, SQL_CLOSE);
				//	Drop the SQL statement data stored (including cursors)
				//	-- this will purge any cached cursor associated with the statement 							
		if (ResetStmt(h) == SQL_SUCCESS){
			strncpy(h->szCommand, szSQL, COMMAND_LENGTH -1);
			h->szCommand[COMMAND_LENGTH - 1] = '\0';
		}
		if (!h->error){
			retcode = SQLExecDirect(h->hstmt, (unsigned char*)szSQL, strlen(szSQL));
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO){
				_NT_ODBC_Error(h);
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
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}
	PUTBACK;
}
	
XS(XS_WIN32__ODBC_Fetch) // ODBC_Fetch($connection) returns (0,@dataelements) or ($err,$errtext)
{
	dXSARGS;
	ODBC_TYPE * h;
	int        con_num;

	RETCODE retcode;         // yet more ODBC garbage
	UCHAR  buff2[ODBC_BUFF_SIZE];
	UCHAR	*szBuf = 0;
	SDWORD	iBuf = 0;
	SDWORD bufflenout;
	int lenn;
	UWORD  x;
	char * szSQL; 
	int len;

	if(items < 1){
		CROAK("usage: ($err,@col) = ODBC_Fetch($connection)\ndie \"Oops: $col[0]\" if ($err);\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));

	PUSHMARK(sp);

	if (h != ODBCList){
		retcode = SQLFetch(h->hstmt);
		if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
			_NT_ODBC_Error(h);
		}
		if (retcode == SQL_NO_DATA_FOUND){
			h->EOR = 1;
			strcpy(h->szError, "No data records remain.");
		}
	}
	if (!h->error){
			//	Allocate a default size to be used. This prevents the problem of ODBC 
			//	returning too small of a size for a regular numberical field.
			//											dave@roth.net	96.05.03	
		if (!(szBuf = new UCHAR [DEFAULTCOLSIZE])){
			sprintf((char *)buff2, "Could not allocate enough memory (%d bytes) for default column size.\n", DEFAULTCOLSIZE);
			ODBCError((char *)buff2, h);
		}
	}	

	if (!h->error){		 // everything is happy
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		for(x=1; x<=h->numcols; x++){
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
				if (iBuf < bufflenout){
					if (szBuf){
						delete szBuf;
						szBuf = 0;
						iBuf = 0;
					}
					if (bufflenout > h->iMaxBufSize){
						bufflenout = h->iMaxBufSize;
					}
								//	[bufflenout + 1]  -- Jutta M. Klebe <jmk@exc.bybyte.de>	96.04.21
					if (!(szBuf = new UCHAR [bufflenout + 1])){
						sprintf((char *)buff2, "Could not allocate enough memory (%d bytes) for column %d.\n", (bufflenout + 1), x);
						ODBCError((char *)buff2, h);
						XPUSHs(sv_2mortal(newSVnv((double)h->error)));
						XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
						PUTBACK;
						return;
					}else{
						iBuf = bufflenout;
					}
				}
			}
			memset( szBuf, 0, iBuf );		//	Thanks to "Jutta M. Klebe" <jmk@exc.bybyte.de>
			SQLGetData(h->hstmt, x, SQL_C_CHAR, szBuf, (SDWORD)iBuf + 1, &bufflenout); //	iBuf + 1 -- Jutta M. Klebe <jmk@exc.bybyte.de>	96.04.21
			XPUSHs(sv_2mortal(newSVpv((char *)szBuf, strlen((const char *)szBuf))));
		}
	}else{
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
	}

	if (szBuf){
		delete szBuf;
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
	PUTBACK;
}


XS(XS_WIN32__ODBC_Disconnect) // usage: ODBC_Disconnect($conn) or ODBC_Disconnect() for all
{
	dXSARGS;
	ODBC_TYPE *h, *hTemp;
	int		conn;
	
	if(items < 1){
		conn = 0;
	}else{
		conn = SvIV(ST(0));
	}
	PUSHMARK(sp);
	DeleteConn(conn);
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
		
		sprintf(h->szCommand, "SQLTables(\"%s\", \"%s\", \"%s\", \"%s\")", szQualifier, szOwner, szName, szType);
		
		retcode = SQLTables(h->hstmt, szQualifier, sQLen, szOwner, sOLen, szName, sNLen, szType, sTLen);
		if(retcode != SQL_SUCCESS){
			_NT_ODBC_Error(h);
		}
	}
	if (!h->error){ 			
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		retcode = SQLNumResultCols(h->hstmt, (short *)&h->numcols);
		for(x=1; x<=h->numcols; x++){
			*buff2 = '\0';
			SQLColAttributes(h->hstmt, x, SQL_COLUMN_NAME, buff2, ODBC_BUFF_SIZE, (short *)&bufflenout, NULL);

				//	We need to convert the columns to uppercase since different
				//	ODBC drivers impliment this differently (Access uses upperc, MS SQL server uses lowerc)
				//	We should probably figure out a more reasonable solution.
			for(iTemp = strlen((char *)buff2) - 1; iTemp >= 0; iTemp--){
				buff2[iTemp] = (char) toupper(buff2[iTemp]);
			}
			XPUSHs(sv_2mortal(newSVpv((char *)buff2, strlen((const char*)buff2))));
		}
	}else{
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
 
 	PUSHMARK(sp);
 	if (h != ODBCList){
 		retcode = SQLMoreResults(h->hstmt);
 		if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
 			_NT_ODBC_Error(h);
 		}
 		if (retcode == SQL_NO_DATA_FOUND){
 			h->EOR = 1;
 			strcpy(h->szError, "No data records remain.");
 		}
 	}
 	if (!h->error){		 // everything is happy
 		XPUSHs(sv_2mortal(newSVnv((double)0)));
 		XPUSHs(sv_2mortal(newSVnv((double)0)));
 	}else{
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
	PUSHMARK(sp);

	if (h != ODBCList){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)h->iMaxBufSize)));
	}else{
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
	PUSHMARK(sp);

	if (h != ODBCList){
		iSize = SvIV(ST(1));
		iSize = ((iSize <= 0)? 0:iSize);
		iSize = ((iSize >= MAX_DATA_BUF_SIZE)? MAX_DATA_BUF_SIZE:iSize);
		h->iMaxBufSize = iSize;
	
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)h->iMaxBufSize)));
	}else{
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

	if(items != 1){
		CROAK("usage: ($Err, $DSN) = ODBC_GetDSN($Connection)\n");
	}
	h = _NT_ODBC_Verify(SvIV(ST(0)));
	PUSHMARK(sp);
	if (h != ODBCList){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVpv((char *)h->szDSN, strlen((char*)h->szDSN))));
	}else{
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
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
	RETCODE	retcode;

	if(items > 0){
		CROAK("usage: ($Err, $DSN) = ODBC_DataSources()\n");
	}
	
	h = ODBCList;
	PUSHMARK(sp);
	*szDSN = *szDesc = '\0';
	retcode= SQLDataSources(h->henv, SQL_FETCH_FIRST, szDSN, SQL_MAX_DSN_LENGTH + 1, &pcbDSN, szDesc, DS_DESCRIPTION_LENGTH, &pcbDesc);
	if(retcode == SQL_SUCCESS){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		
		while (retcode == SQL_SUCCESS){
			XPUSHs(sv_2mortal(newSVpv((char *)szDSN, strlen((char*)szDSN))));
			XPUSHs(sv_2mortal(newSVpv((char *)szDesc, strlen((char*)szDesc))));
			*szDSN = *szDesc = '\0';
			retcode= SQLDataSources(h->henv, SQL_FETCH_NEXT, szDSN, SQL_MAX_DSN_LENGTH + 1, &pcbDSN, szDesc, DS_DESCRIPTION_LENGTH, &pcbDesc);
		}
	}else{
		h = ODBCError("No such ODBC connection.", 0);
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
		h = ODBCError("No such ODBC connection.", 0);
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
	PUSHMARK(sp);
	
	retcode = SQLRowCount(h->hstmt, &sdRows);
	if(retcode == SQL_SUCCESS){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVnv((double)sdRows)));
	}else{
		h = ODBCError("No such ODBC connection.", 0);
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
		CROAK("usage: ($Err, $Type) = ODBC_GetMaxBufSize($Connection)\n");
	}

	h = _NT_ODBC_Verify(SvIV(ST(0)));
	PUSHMARK(sp);
	
	if (!(szType = MapCloseType(h->uStmtCloseType))){
		ODBCError("Invalid Statment Close Type", h);
	}			
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVpv(szType, strlen(szType))));
	}else{
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
	uType = SvIV(ST(1));
	PUSHMARK(sp);

	switch(uType){
		case SQL_DROP:
		case SQL_CLOSE:
		case SQL_UNBIND:
		case SQL_RESET_PARAMS:
			h->uStmtCloseType = uType;
			if (!(szType = MapCloseType(h->uStmtCloseType))){
				ODBCError("Invalid Statment Close Type", h);
			}
			break;

		default:
			ODBCError("Not a valid Stmt Close Type", h);
	}					
	if (!h->error){
		XPUSHs(sv_2mortal(newSVnv((double)0)));
		XPUSHs(sv_2mortal(newSVpv(szType, strlen(szType))));
	}else{
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv(h->szError, strlen(h->szError))));
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
	XPUSHs(sv_2mortal(newSVnv((double)0)));
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
			iTemp = ODBC_BUFF_SIZE;
			while(iTemp){
				szBuff[--iTemp] = 0;
			}
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
				ODBCError("Unable to determine Column Attribute", h);
			}
		}else{
			ODBCError("Field name does not exist", h);
		}
	}
	 		//	Process only an error state. If this has been successfull the values have
			//	already been processed.
	if (h->error){		
		XPUSHs(sv_2mortal(newSVnv((double)h->error)));
		XPUSHs(sv_2mortal(newSVpv((char *)h->szError, strlen((char *) h->szError))));
	}
	PUTBACK;
}

		
XS(boot_Win32__ODBC)
{
	dXSARGS;
	char* file = __FILE__;
	int i;
	RETCODE	retcode;

			//	Start the ODBC Connection Chain!
	CleanODBC(ODBCList);
	ODBCList->conn = ODBC_Conn_Number++;
	ODBCList->Prev = 0;
	ODBCList->Next = 0;
	retcode = SQLAllocEnv(&ODBCList->henv);
	if (retcode != SQL_SUCCESS){							   
		ODBCError("Ran out of memory allocating ODBC enviroment", ODBCList);
	}

	newXS("Win32::ODBC::constant",				XS_WIN32__ODBC_Constant, file);
	newXS("Win32::ODBC::ODBCConnect",			XS_WIN32__ODBC_Connect,  file);
    newXS("Win32::ODBC::ODBCExecute",			XS_WIN32__ODBC_Execute, file);
    newXS("Win32::ODBC::ODBCFetch",				XS_WIN32__ODBC_Fetch, file);
    newXS("Win32::ODBC::ODBCDisconnect",		XS_WIN32__ODBC_Disconnect, file);
	newXS("Win32::ODBC::ODBCGetError",			XS_WIN32__ODBC_GetError, file);
	newXS("Win32::ODBC::ODBCTableList",			XS_WIN32__ODBC_TableList, file);
	newXS("Win32::ODBC::ODBCMoreResults",		XS_WIN32__ODBC_MoreResults, file);
	newXS("Win32::ODBC::ODBCGetMaxBufSize",		XS_WIN32__ODBC_GetMaxBufSize, file);
	newXS("Win32::ODBC::ODBCSetMaxBufSize",		XS_WIN32__ODBC_SetMaxBufSize, file);
	newXS("Win32::ODBC::ODBCGetConnections",	XS_WIN32__ODBC_GetConnections, file);
	newXS("Win32::ODBC::ODBCGetStmtCloseType",	XS_WIN32__ODBC_GetStmtCloseType, file);
	newXS("Win32::ODBC::ODBCSetStmtCloseType",	XS_WIN32__ODBC_SetStmtCloseType, file);
	newXS("Win32::ODBC::ODBCGetDSN",			XS_WIN32__ODBC_GetDSN, file);
	newXS("Win32::ODBC::ODBCDataSources",		XS_WIN32__ODBC_DataSources, file);
	newXS("Win32::ODBC::ODBCDrivers",			XS_WIN32__ODBC_Drivers, file);
	newXS("Win32::ODBC::ODBCRowCount",			XS_WIN32__ODBC_RowCount, file);
	newXS("Win32::ODBC::ODBCCleanError",		XS_WIN32__ODBC_CleanError, file);
	newXS("Win32::ODBC::Info",					XS_WIN32__ODBC_Info, file);
	newXS("Win32::ODBC::ODBCColAttributes",		XS_WIN32__ODBC_ColAttributes, file);									   
	ST(0) = &sv_yes;
	XSRETURN(1);

}



