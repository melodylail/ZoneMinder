//
// ZoneMinder Streaming Server, $Date$, $Revision$
// Copyright (C) 2002  Philip Coombes
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 

#include "zm.h"

int main( int argc, const char *argv[] )
{
	int id = 1;
	unsigned long idle = 5000;
	unsigned long refresh = 50;
	int event = 0;
	char *path = ".";

	const char *query = getenv( "QUERY_STRING" );
	if ( query )
	{
		char temp_query[256];
		strcpy( temp_query, query );
		char *q_ptr = temp_query;
		char *parms[8]; // Shouldn't be more than this
		int parm_no = 0;
		while( parms[parm_no] = strtok( q_ptr, "&" ) )
		{
			parm_no++;
			q_ptr = NULL;
		}
	
		for ( int p = 0; p < parm_no; p++ )
		{
			char *name = strtok( parms[p], "=" );
			char *value = strtok( NULL, "=" );
			if ( !strcmp( name, "refresh" ) )
				refresh = atol( value );
			else if ( !strcmp( name, "idle" ) )
				idle = atol( value );
			else if ( !strcmp( name, "monitor" ) )
				id = atoi( value );
			else if ( !strcmp( name, "event" ) )
				event = atoi( value );
			else if ( !strcmp( name, "path" ) )
				path = value;
		}
	}

	dbg_name = "zms";

	DbgInit();

	if ( !mysql_init( &dbconn ) )
	{
		fprintf( stderr, "Can't initialise structure: %s\n", mysql_error( &dbconn ) );
		exit( mysql_errno( &dbconn ) );
	}
	if ( !mysql_connect( &dbconn, "", ZM_DB_USERB, ZM_DB_PASSB ) )
	{
		fprintf( stderr, "Can't connect to server: %s\n", mysql_error( &dbconn ) );
		exit( mysql_errno( &dbconn ) );
	}
	if ( mysql_select_db( &dbconn, ZM_DATABASE ) )
	{
		fprintf( stderr, "Can't select database: %s\n", mysql_error( &dbconn ) );
		exit( mysql_errno( &dbconn ) );
	}

	if ( !event )
	{
		Monitor *monitor = Monitor::Load( id );

		if ( monitor )
			monitor->StreamImages( idle, refresh, stdout );
	}
	else
	{
		Event::StreamEvent( path, event, refresh, stdout );
	}
	return( 0 );
}
