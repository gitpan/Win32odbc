

#define	SYNTAX_ERROR	999
#define CROAK(xxxx)													\
				PUSHMARK(sp);										\
				XPUSHs(sv_2mortal(newSVnv((double)SYNTAX_ERROR)));	\
				XPUSHs(sv_2mortal(newSVpv(xxxx, strlen(xxxx))));	\
				PUTBACK;											\
				return;												\
																				\
																				\

#define TMPBUFSZ 512

#define SUCCESSRETURNED(x)	(x == ERROR_SUCCESS)
#define REGRETURN(x) XSRETURN_IV(SUCCESSRETURNED(x))

#define MAX_DATA_BUF_SIZE		2147483647	//	Largest value a SDWORD can have!	
#define DEFAULT_DATA_BUF_SIZE	10240
#define	DEFAULTCOLSIZE			20			//	Start with DEFAULTCOLSIZE number of chars per column.


#define	COMMAND_LENGTH			1024
#define	DSN_LENGTH				1024
#define	DS_DESCRIPTION_LENGTH	2048

#define ODBC_BUFF_SIZE 			1024
#define	SQL_STATE_SIZE			10

#define	QUERY_TIMEOUT_VALUE		120
#define	LOGIN_TIMEOUT_VALUE		20

#define	DEFAULT_STMT_CLOSE_TYPE	SQL_DROP

struct	ODBC_Conn{
	int		conn;						//	connection number
	HENV	henv;  						//	Store our ODBC henv, hdbc, hstmt
	HDBC	hdbc;
	HSTMT	hstmt;
	UWORD	uStmtCloseType;				//  Type of closing to perform on a FreeStmt()
	long	iMaxBufSize;				//	Max memory buffer size for this connection.
	int		numcols;  					//	for storing btwn Execute and Fetch
	char	szError[ODBC_BUFF_SIZE];	//	Last Error Message
	UCHAR	szSqlState[SQL_STATE_SIZE];	//	Last ODBC SQL State
	int		error;						//	Last error number
	int		EOR;						//	End of Records (no more left)
	char	szDSN[DSN_LENGTH];			//	DSN for this connection;
	char	szCommand[COMMAND_LENGTH];	//	Last issued SQL or other command.
	ODBC_Conn *Prev;					//	Prev ODBC in the chain!
	ODBC_Conn *Next;					//	Next ODBC in the chain!
} typedef ODBC_TYPE;		  		

ODBC_TYPE ODBCListStart;
ODBC_TYPE	*ODBCList = &ODBCListStart;
int		ODBC_Conn_Number = 0;

char	ODBC_errorstring[ODBC_BUFF_SIZE];
int		ODBC_errornum;


ODBC_TYPE *NewODBC();
ODBC_TYPE *CleanODBC(ODBC_TYPE *h);
ODBC_TYPE * _NT_ODBC_Verify(int iODBC);
ODBC_TYPE *ODBCError(char *szString, ODBC_TYPE *h);
void _NT_ODBC_Error(ODBC_TYPE * h);
void FreeODBC(ODBC_TYPE *h);
RETCODE ResetStmt(ODBC_TYPE *h);
char *MapCloseType(UWORD uCloseType);
void CleanError(ODBC_TYPE *h);
int	ColNameToNum(ODBC_TYPE *h, char *szName);


#define	VERNAME		"ODBC extension for Win32 Perl"
#define VERSION		"v960528"
#define VERDATE		__DATE__
#define VERAUTH     "Dave Roth <dave@roth.net> or <roth@panda.dnr.state.mi.us>"
#define VERCRED		"Copyright (c) 1996 Dave Roth.\nBased on original code by Dan DeMaggio <dmag@umich.edu>."	
					 

