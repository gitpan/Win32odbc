package Win32::ODBC;

# Win32::ODBC.pm
#       +==========================================================+
#       |                                                          |
#       |                     ODBC.PM package                      |
#       |                     ---------------                      |
#       |                    Release v96.05.28                     |
#       |                                                          |
#       |    Copyright (c) 1996 Dave Roth. All rights reserved.    |
#       |   This program is free software; you can redistribute    |
#       | it and/or modify it under the same terms as Perl itself. |
#       |                                                          |
#       +==========================================================+
#
#
#         based on original code by Dan DeMaggio (dmag@umich.edu)
#
#	Use under GNU General Public License or Larry Wall's "Artistic License"
#
#	Check the README.TXT file that comes with this package for details about
#	it's history.
#

require Exporter;
require DynaLoader;

$Win32::ODBC::Version = 960528;

@ISA= qw( Exporter DynaLoader );
    # Items to export into callers namespace by default. Note: do not export
    # names by default without a very good reason. Use EXPORT_OK instead.
    # Do not simply export all your public functions/methods/constants.
@EXPORT = qw(
            SQL_COLUMN_COUNT
            SQL_COLUMN_NAME
            SQL_COLUMN_TYPE
            SQL_COLUMN_LENGTH
            SQL_COLUMN_PRECISION
            SQL_COLUMN_SCALE
            SQL_COLUMN_DISPLAY_SIZE
            SQL_COLUMN_NULLABLE
            SQL_COLUMN_UNSIGNED
            SQL_COLUMN_MONEY
            SQL_COLUMN_UPDATABLE
            SQL_COLUMN_AUTO_INCREMENT
            SQL_COLUMN_CASE_SENSITIVE
            SQL_COLUMN_SEARCHABLE
            SQL_COLUMN_TYPE_NAME
            SQL_COLUMN_TABLE_NAME
            SQL_COLUMN_OWNER_NAME
            SQL_COLUMN_QUALIFIER_NAME
            SQL_COLUMN_LABEL
            SQL_COLATT_OPT_MAX
            SQL_COLUMN_DRIVER_START
            SQL_COLATT_OPT_MIN
            SQL_ATTR_READONLY
            SQL_ATTR_WRITE
            SQL_ATTR_READWRITE_UNKNOWN
            SQL_UNSEARCHABLE
            SQL_LIKE_ONLY
            SQL_ALL_EXCEPT_LIKE
            SQL_SEARCHABLE
            );

    # ----------------------------------
    # Notes:
    #   It is *NOT* Perl-DBI compliant.
    #   Not tested _extremely_ well.
    #   NEW LIMIT: 10K chars per column.
    # ----------------------------------


sub new
{
    my ($n, $Err, $Errtext, $self);
    my ($type) = shift;

    my ($DSN) = shift;
    ($n,$Err,$Errtext) = ODBCConnect($DSN);
    if (!$n){
        return (0, $Err, $Errtext);
    }
    $self = bless {};
    $self->{'connection'} = $n;
    $self->{'DSN'} = $DSN;
    $self;
}

####
#   Close this ODBC session (or all sessions)
####
sub Close
{
    my ($self) = shift;
    ODBCDisconnect($self->{'connection'});
    undef $self->{'DSN'};
    undef @{$self->{'fnames'}};
    undef %{$self->{'field'}};
    undef $self->{'error'};
    return undef;
}

####
#   Auto-Kill an instance of this module
####
sub DESTROY
{
    my ($self) = shift;
    Close($self);
}


sub Sql{
    return (sql(@_));
}

####
#   Submit an SQL Execute statement for processing
####
sub sql
{
    my ($self) = shift;
    my ($sql) = shift;
    my (@Results);

    undef %{$self->{'field'}};
    undef $self->{'error'};

    @Results = ODBCExecute($self->{'connection'},$sql);
    return updateResults($self, @Results);
}

####
#   Retrieve data from a particular field
####
sub Data{

        #   Change by JOC 06-APR-96
        #   Altered by Dave Roth <dave@roth.net> 96.05.07
    my($self) = shift;
    my(@Fields) = @_;
    my(@Result, $Result, $Field);

    @Fields = @{$self->{'fnames'}} if (! scalar(@Fields));
    foreach $Field (@Fields) {
        if (wantarray) {
            push(@Result, data($self, $Field));
        } else {
            $Result .= data($self, $Field);
        }
    }
    return wantarray ? @Result : $Result;
}

sub DataHash{
    my($self) = shift;
    my(@Result) = @_;
    my(%Result, $Element);
    @Result = @{$self->{'fnames'}} if (! scalar(@Result));
    foreach $Element (@Result) {
        $Result{$Element} = data($self, $Element);
    }

    return %Result;
}

####
#   Retrieve data from the data buffer
####
sub data
{  $_[0]->{'data'}->{$_[1]}; }


sub FetchRow{
    return (fetchrow(@_));
}
####
#   Put a row from an ODBC data set into data buffer
####
sub fetchrow{
    my ($self) = shift;
    my ($item, $num, $sqlcode, @Result);
        # Added by JOC 06-APR-96
        #   $num = 0;
    $num = 0;
    undef $self->{'data'};

    @Result = ODBCFetch($self->{'connection'});

    $sqlcode = shift(@Result);
    if ($sqlcode){
        $self->{'error'} = shift(@Result);
        return undef;
    }
    foreach $item (@Result){
        $item =~ s/ +$//; # HACK
        $self->{'data'}->{ ${$self->{'fnames'}}[$num] } = $item;
        $num++;
    }
        #   return is a hack to interface with a assoc array.
    return wantarray? (1, 1): 1;
}

####
#   Generic processing of data into associative arrays
####
sub updateResults{
    my ($self) = shift;
    my ($sqlcode);

    undef %{$self->{'field'}};
    undef $self->{'error'};

    @{$self->{'fnames'}} = @_;

    $sqlcode = shift(@{$self->{'fnames'}});
    if ($sqlcode){
        $self->{'error'} = ${$self->{'fnames'}}[0];
        return ($sqlcode);
    }
    foreach (0..$#{$self->{'fnames'}}){
        s/ +$//;
        $self->{'field'}->{${$self->{'fnames'}}[$_]} = $_;
    }
    return undef;
}

####
#   Process Errors returned from a call to ODBCxxxx().
#   It is assumed that the Win32::ODBC function returned the following structure:
#       ($ErrorNumber, $ResultText)
#           $ErrorNumber....0 = No Error
#                           >0 = Error Number
#           $ResultText.....if no error then this is the result.
#                           if error then this is the error text.
####
sub processError{
    my($self, $Error, @Result) = @_;
    if ($Error){
        $self->{'error'} = $Result[0];
        undef @Result;
    }
    return @Result;
}

####
#   Retrieve the catalog from the current DSN
#	NOTE: All Field names are uppercase!!!
####
sub Catalog
{
    my ($self) = shift;
    my ($Qualifier, $Owner, $Name, $Type) = @_;
    my (@Results) = ODBCTableList($self->{'connection'}, $Qualifier, $Owner, $Name, $Type);
    return updateResults($self, @Results);
}

####
#   Return an array of names from the catalog for the current DSN
#       TableList($Qualifier, $Owner, $Name, $Type)
#           Return: (array of names of tables)
#	NOTE: All Field names are uppercase!!!
####
sub TableList{
    my ($self) = shift;
    my (@Results) = @_;
    if (! scalar(@Results)){
        @Results = ("", "", "%", "'TABLE'");
    }

    if (Catalog($self, @Results)){
        return undef;
    }
    undef @Results;
    while (fetchrow($self)){
        push(@Results, data($self, "TABLE_NAME"));
    }
    return sort(@Results);
}

sub Error{
    return (error(@_));
}
####
#   Return the last recorded error message
####
sub error { $_[0]->{'error'}; }


sub FieldNames{
    return (fieldnames(@_));
}
####
#   Return an array of fieldnames extracted from the current dataset
####
sub fieldnames { $self = shift; return @{$self->{'fnames'}}; }

####
#   Prints out the current dataset (used mostly for testing)
####
sub DumpData {
    my($self) = @_; my($f, $goo);

        #   Changed by JOC 06-Apr-96
        #   print "\nDumping Data for connection: $conn->{'connection'}\n";
    print "\nDumping Data for connection: $self->{'connection'}\n";
    print "Error: \"";
    print $self->error;
    print "\"\n";
    if (!$self->error){
       foreach $f ($self->fieldnames){
            print $f . " ";
            $goo .= "-" x length($f);
            $goo .= " ";
        }
        print "\n$goo\n";
        while ($self->fetchrow){
            foreach $f ($self->fieldnames){
                print $self->data($f) . " ";
            }
            print "\n";
        }
    }
}

####
#   Submit an SQL statement and print data about it (used mostly for testing)
####
sub Run{
    my($self, $Sql) = @_;

    print "\nExcecuting connection $self->{'connection'}\nsql statement: \"$Sql\"\n";
    $self->sql($Sql);
    print "Error: \"";
    print $self->error;
    print "\"\n";
    print "--------------------\n\n";
}

####
#   Closes this connection. This is used mostly for testing. You should
#   probably use Close().
####
sub ShutDown{
    my($self) = @_;
    print "\nClosing connection $self->{'connection'}...";
    $self->Close();
    print "\nDone\n";
}

####
#   Return this connection number
####
sub Connection{
    my($self) = @_;
    return $self->{'connection'};
}

####
#   Returns the current connections that are in use.
####
sub GetConnections{
    return ODBCGetConnections();
}

####
#   Set the Max Buffer Size for this connection. This determines just how much
#   ram can be allocated when a fetch() is performed that requires a HUGE amount
#   of memory. The default max is 10k and the absolute max is 100k.
#   This will probably never be used but I put it in because I noticed a fetch()
#   of a MEMO field in an Access table was something like 4Gig. Maybe I did
#   something wrong, but after checking several times I decided to impliment
#   this limit thingie.
####
sub SetMaxBufSize{
    my($self, $Size) = @_;
    my(@Result) = ODBCSetMaxBufSize($self->{'connection'}, $Size);
    return (processError($self, @Result))[0];
}

####
#   Returns the Max Buffer Size for this connection. See SetMaxBufSize().
####
sub GetMaxBufSize{
    my($self) = @_;
    my(@Result) = ODBCGetMaxBufSize($self->{'connection'});
    return (processError($self, @Result))[0];
}


####
#   Returns the DSN for this connection
####
sub GetDSN{
    my($self, $Con) = @_;
    my(@Result) = ODBCGetDSN((($Con)? $Con:$self->{'connection'}));
    return (processError($self, @Result))[0];
}

####
#   Returns an associative array of $XXX{'DSN'}=Description
####
sub DataSources{
    my($self) = @_;
    my(@Result) = ODBCDataSources();
    return (processError($self, @Result));
}

####
#   Returns an associative array of $XXX{'Driver Name'}=Driver Attributes
####
sub Drivers{
    my($self) = @_;
    my(@Result) = ODBCDrivers();
    return (processError($self, @Result));
}

####
#   Returns the number of Rows that were affected by the previous SQL command.
####
sub RowCount{
    my($self, $Connection) = @_;
    if (! ref($self)){$Connection = $self;}
    if (! $Connection){$Connection = $self->{'connection'};}
    my(@Result) = ODBCRowCount($Connection);
    return (processError($self, @Result));
}

####
#   Returns the Statement Close Type -- how does ODBC Close a statment.
#       Types:
#           SQL_DROP
#           SQL_CLOSE
#           SQL_UNBIND
#           SQL_RESET_PARAMS
####
sub GetStmtCloseType{
    my($self, $Connection) = @_;
    if (! ref($self)){$Connection = $self;}
    if (! $Connection){$Connection = $self->{'connection'};}
    my(@Result) = ODBCGetStmtCloseType($Connection);
    return (processError($self, @Result));
}

####
#   Sets the Statement Close Type -- how does ODBC Close a statment.
#       Types:
#           SQL_DROP
#           SQL_CLOSE
#           SQL_UNBIND
#           SQL_RESET_PARAMS
#   Returns the newly set value.
####
sub SetStmtCloseType{
    my($self, $Type, $Connection) = @_;
    if (! ref($self)){
        $Connection = $Type;
        $Type = $self;
    }
    if (! $Connection){$Connection = $self->{'connection'};}
    my(@Result) = ODBCSetStmtCloseType($Connection, $Type);
    return (processError($self, @Result));
}

sub ColAttributes{
    my($self, $Type, @Field) = @_;
    my(%Result, @Result, $Result, $Attrib, $Connection, $Temp);
    if (! ref($self)){
        $Type = $Field;
        $Field = $self;
    }
    $Connection = $self->{'connection'};
    if (! scalar(@Field)){ @Field = $self->fieldnames;}
    foreach $Temp (@Field){
        @Result = ODBCColAttributes($Connection, $Temp, $Type);
        ($Attrib) = processError($self, @Result);
        if (wantarray){
            $Result{$Temp} = $Attrib;
        }else{
            $Result .= "$Temp";
        }
    }
    return wantarray? %Result:$Result;
}


sub Version{
	my($self, @Packages) = @_;
	my($Temp, @Results);
	if (! ref($self)){
		push(@Packages, $self);
	}
	my($ExtName, $ExtVersion) = Info();
	if (! scalar(@Packages)){
		@Packages = ("ODBC.PM", "ODBC.PLL");
	}
	foreach $Temp (@Packages){
		if ($Temp =~ /pll/i){
			push(@Results, "ODBC.PM:$Win32::ODBC::Version");
		}elsif ($Temp =~ /pm/i){
			push(@Results, "ODBC.PLL:$ExtVersion");
		}
	}
	return @Results;
}
	


# ------------------ A U T O L O A D   F U N C T I O N ---------------------

sub AUTOLOAD {
    # This AUTOLOAD is used to 'autoload' constants from the constant()
    # XS function.  If a constant is not found then control is passed
    # to the AUTOLOAD in AutoLoader.

    my($constname);
    ($constname = $AUTOLOAD) =~ s/.*:://;
    #reset $! to zero to reset any current errors.
    $!=0;
    $val = constant($constname, @_ ? $_[0] : 0);

    if ($! != 0) {
    if ($! =~ /Invalid/) {
        $AutoLoader::AUTOLOAD = $AUTOLOAD;
        goto &AutoLoader::AUTOLOAD;
    }
    else {

            # Added by JOC 06-APR-96
            # $pack = 0;
        $pack = 0;
        ($pack,$file,$line) = caller;
            print "Your vendor has not defined Win32::ODBC macro $constname, used in $file at line $line.";
    }
    }
    eval "sub $AUTOLOAD { $val }";
    goto &$AUTOLOAD;
}

bootstrap Win32::ODBC;

# Preloaded methods go here.


# Autoload methods go after __END__, and are processed by the autosplit program.

1;
__END__



