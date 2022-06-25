#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//#define debug

typedef struct word
{
		char chars[ 32 ];
		unsigned int chars_size;
} word;
static unsigned int words_total = 0;

typedef struct state
{
		word words[ 64 ];
		unsigned int words_size;
		unsigned int scope;
} state;
static unsigned int states_total = 0;

//

typedef struct keyword
{
		char chars[ 32 ];

		bool check;
		int check_pos;
		char check_char;
} keyword;
static keyword keywords[ 128 ];

//

int main( int argc, char* argv[] )
{

	// init

#ifdef debug
	argc = 2;
	argv[ 1 ] = "src.hept";
#endif

	if( argc <= 1 )
	{
		printf( "alter requires an input filename" );
		goto quit;
	} else if( argc >= 3 )
	{
		printf( "only one argument is required for alter" );
		goto quit;
	} else
	{
		if( strcmp( argv[ 1 ], "help" ) == 0 )
		{
			printf( "alter requires the filename of the input source\n\te.g. ./alter game.hept\n" );
			goto quit;
		} else
			printf( " _______\n/ alter \\\n\n// location:\n\t%s\n// input:\n\t%s\n", argv[ 0 ], argv[ 1 ] );
	}

	// read file

	FILE* file_read = fopen( argv[ 1 ], "r" );

	if( file_read == NULL )
	{
		printf( "alter cannot find the file" );
		goto quit;
	}

	// extension

	static char ext[ 16 ];
	static unsigned int ext_ptr = 0;
	for( int a = 0; a < strlen( argv[ 1 ] ); ++a )
	{
		static bool e = false;
		static char c;
		c = argv[ 1 ][ a ];
		if( e ) ext[ ext_ptr++ ] = c;
		if( c == '.' ) e = true;
	}
	printf( "// extension:\n\t%s\n", ext );
	printf( "// looking for \"%s.alter\"...\n", ext );

	// alter template

	char* alter_filename = strcat( strdup( ext ), ".alter" );
	FILE* alter_read = fopen( alter_filename, "r" );

	if( alter_read == NULL )
	{
		printf( "!!!!!!!\nalter cannot find the \"%s.alter\" file\n!!!!!!!", ext );
		goto quit;
	}

	do {
		static char read_char;

		read_char = fgetc( alter_read );
		if( feof( alter_read ) ) break;

		//
		
		

	} while( 1 );

	// process

	static state state_list[ 128 ];
	static unsigned int state_ptr = 0, word_ptr = 0, char_ptr = 0;

	static int scope = 0, paren = 0;

	do {
		static bool state_new = false, word_new = false;
		static char read_char;

		read_char = fgetc( file_read );
		if( feof( file_read ) ) break;

		//

		if( isalnum( read_char ) || read_char == '.' )
		{
			// char new
			state_list[ state_ptr ].words[ word_ptr ].chars[ char_ptr ] = read_char;
			state_list[ state_ptr ].words[ word_ptr ].chars_size++;
			char_ptr++;
		} else
		{
			switch( read_char )
			{
				case '{':
					{
						scope++;
						break;
					}

				case '}':
					{
						scope--;
						break;
					}

				case ';':
				case '\n':
					{
						if( state_list[ state_ptr ].words_size ) state_new = true;
						break;
					}
			}

			if( state_list[ state_ptr ].words[ word_ptr ].chars_size ) word_new = true;
		}

		//

		if( word_new )
		{
			words_total++;
			state_list[ state_ptr ].words_size++;
			word_ptr++;
			char_ptr = 0;
			word_new = false;
		}

		if( state_new )
		{
			states_total++;
			state_list[ state_ptr ].scope = scope;
			state_ptr++;
			word_ptr = 0;
			char_ptr = 0;
			state_new = false;
		}
	} while( 1 );

	printf( "(words: %i)\n", words_total );

	for( int s = 0; s < state_ptr; ++s )
	{
		for( int t = 0; t < state_list[ s ].scope; ++t )
		{
			printf( "\t" );
		}
		for( int w = 0; w < state_list[ s ].words_size; ++w )
		{
			for( int c = 0; c < state_list[ s ].words[ w ].chars_size; ++c )
			{
				printf( "%c", state_list[ s ].words[ w ].chars[ c ] );
			}
			printf( " " );
		}
		printf( "\n" );
	}

	//

quit:
	return 0;
}