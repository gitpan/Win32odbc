

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

#define	TABLE_COMMAND_STRING	"SQLTables(\"%s\", \"%s\", \"%s\", \"%s\")"

#define	DEFAULT_STMT_CLOSE_TYPE	SQL_DROP

struct	ODBC_Conn{
	int		conn;						//	connection number
	HENV	henv;  						//	Store our ODBC henv, hdbc, hstmt
	HDBC	hdbc;
	HSTMT	hstmt;
	UWORD	uStmtCloseType;				//  Type of closing to perform on a FreeStmt()
	long	iMaxBufSize;				//	Max memory buffer size for this connection.
	int		numcols;  					//	for storing btwn Execute and Fetch
	DWORD	dMaxRows;					//	Max # of rows we can retrieve
	DWORD	dNumOfRows;					//	Number of rows already retrieved
	char	szError[ODBC_BUFF_SIZE];	//	Last Error Message
	UCHAR	szSqlState[SQL_STATE_SIZE];	//	Last ODBC SQL State
	char	szFunction[50];				//	What Function generated the error?
	char	szFunctionLevel[10];		//	What level within the Function?
	int		iDebug;						//	Is debugging active?
	long	error;						//	Last error number
	int		EOR;						//	End of Records (no more left)
	char	szUserDSN[DSN_LENGTH];		//	DSN for this connection(Specified by the user);
	char	*szDSN;						//	DSN for this connection (specified by ODBC);
	char	*szCommand;					//	Last issued SQL or other command.
	ODBC_Conn *Prev;					//	Prev ODBC in the chain!
	ODBC_Conn *Next;					//	Next ODBC in the chain!
} typedef ODBC_TYPE;		  		

ODBC_TYPE ODBCListStart;
ODBC_TYPE	*ODBCList = &ODBCListStart;
int		ODBC_Conn_Number = 0;  
int		ODBCTotal = 1;

char	ODBC_errorstring[ODBC_BUFF_SIZE];
int		ODBC_errornum;

HINSTANCE	ghDLL = 0;
int			giThread = 0;


ODBC_TYPE *NewODBC();
ODBC_TYPE *CleanODBC(ODBC_TYPE *h);
ODBC_TYPE * _NT_ODBC_Verify(int iODBC);
ODBC_TYPE *ODBCError(char *szString, ODBC_TYPE *h, char *szFunction, char *szFunctionLevel);
void _NT_ODBC_Error(ODBC_TYPE * h, char *szFunction, char *szFunctionLevel);
void FreeODBC(ODBC_TYPE *h);
RETCODE ResetStmt(ODBC_TYPE *h);
char *MapCloseType(UWORD uCloseType);
void CleanError(ODBC_TYPE *h);
int	ColNameToNum(ODBC_TYPE *h, char *szName);
BOOL WINAPI DllMain(HINSTANCE  hinstDLL, DWORD fdwReason, LPVOID  lpvReserved);

void heapdump( void );


#define	VERNAME		"ODBC extension for Win32 Perl"
#define VERSION		"v961025"
#define VERDATE		__DATE__
#define VERAUTH     "Dave Roth <rothd@roth.net>"
#define VERCRED		"Copyright (c) 1996 Dave Roth.\nBased on original code by Dan DeMaggio <dmag@umich.edu>."	
					 

