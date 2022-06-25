#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define debug

typedef struct word
{
		char chars[ 32 ];
		unsigned int chars_size;
		unsigned int scope;
} word;
static unsigned int words_total = 0;

typedef struct state
{
		word words[ 64 ];
		unsigned int words_size;
} state;
static unsigned int states_total = 0;

int main( int argc, char* argv[] )
{
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
			printf( "\t _______\n\t/ alter \\\n\n\t// location:\n%s\n\t// input:\n%s\n\t// output:\n", argv[ 0 ], argv[ 1 ] );
	}

	//

	FILE* file_read = fopen( argv[ 1 ], "r" );

	static state state_list[ 128 ];
	static unsigned int state_ptr = 0, word_ptr = 0, char_ptr = 0;

	static int scope = 0, paren = 0;
	static char read_char;

	static bool state_new = false, word_new = false;

	do {
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
			state_list[ state_ptr ].words[ word_ptr ].scope = scope;
			state_list[ state_ptr ].words_size++;
			word_ptr++;
			char_ptr = 0;
			word_new = false;
		}

		if( state_new )
		{
			states_total++;
			state_ptr++;
			word_ptr = 0;
			char_ptr = 0;
			state_new = false;
		}
	} while( 1 );

	printf( "(words: %i)\n", words_total );

	for( int s = 0; s < state_ptr; ++s )
	{
		for( int w = 0; w < state_list[ s ].words_size; ++w )
		{
			/*for( int t = 0; t < state_list[s].words[ w ].scope; ++t )
			{
				printf( "\t" );
			}*/
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