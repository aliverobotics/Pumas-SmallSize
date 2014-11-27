/** \file Persistence.cpp
 * \author fjrg76@hotmail.com
 *
 * \todo
*/

#include "stdafx.h"

#ifndef __PERSISTENCE_CPP
#define __PERSISTENCE_CPP

#include <string.h>
#include "Persistence.h"

/**
 *
 * \param idc Character that will be added at the start of the id string, e.g. '#', '$', etc.
 * \see Write()
 * \param idd Character that will be deliming tokens
*/
Persistence::Persistence( char idc, char idd ){
	start = idc;
	delimiter = idd;
}

/** Opens a file for read/write operations 
 *
 * \param fn Name for the file. If doesn't exist then it creates it, if does overwrite it.
 * \return Zero if couldn't open it, non-zero otherwise
*/
int Persistence::Open( char *fn, int type ){
	strcpy( filename, fn);

	if( type == WRITE_FILE ){
		fb.open( filename, ios::in|ios::out|ios::trunc );
		if( fb.fail() )
			return 0;
	}
	//READ_FILE
	else{ 
		fb.open( filename, ios::in|ios::out );
		if( fb.fail() )
			return 0;
	}

	return 1;
}

/** Closes a file opened with the Open() method \see Open
 *
*/
void Persistence::Close( void ){
	fb.close();
}

/** Writes into the file two strings, title, or ID, and data.
 *
 * \param tit It's the name that will identified a field into the file. A '#" character
 * is added at the start of the string, so the parser can find the data.
 * \param str It's the data itself formatted into a null terminated string.
 * After calling Write( "MyData", "123, 456, 789" ); the file will look like this
 *
 * #MyData
 * 123, 456, 789
*/
void Persistence::Write( char *tit, char *str ){
	string = new char[strlen(str)+1];
	strcpy( string, str );

	title = new char[strlen(tit)+1];
	strcpy( title, tit );

	fb << "#" << tit << endl << str << endl;
}

void Persistence::setTag( char *str ){
	fb << start << str << endl;
}

void Persistence::addToken( char *str ){
	fb << str << delimiter;
}

void Persistence::lastToken( char *str ){
	fb << str << endl;
}

int Persistence::seekTag( char *tit ){
	char buf;
	char token[255];
	int error = 0;
	int cont;

	title = new char[strlen(tit)+1];
	strcpy( title, tit );

	fb.seekg( 0, ios::beg );
	while( !fb.eof() ){
		cont = 0;

		// gets the line with "#title"
		buf = fb.get();
		if( buf == start ){
			do{
				buf = fb.get();
				token[cont++] = buf;
			} while( buf != 0x0A );
			token[--cont] = 0x00;

			// retrieves in "str" the data asociated with "title"
			// and gets a string where tokens will be available for the user to parse
			if( !strcmp( token, tit ) ){
				string = new char[255];
				cont = 0;
				do{
					buf = fb.get();
					string[cont++] = buf;
				}while( buf != 0x0A );
				string[--cont] = 0x00;
				delete title;

				return 1;
			}
		}
	}
	delete title;
	return 0;
}












//void Persistence::

/** Parses a file in order to locate the data "str" in the field "tit"
 * 
 * \param tit The name of the field. \see Write
 * \param str String where the data (if founded) will be stored. It's null terminated.
 * This parameter MUST be long enough to store the entire string.
 *
 * \return Zero if there was no matching fields, non-zero if matched.
*/
int Persistence::Read( char *tit, char *str ){
	char buf;
	char token[255];
	int error = 0;
	int cont;

	title = new char[strlen(tit)+1];
	strcpy( title, tit );

	#ifdef comment
	if( fb.fail()){
		//cerr << "Error al abrir el archivo" << endl;
		return 0;
	}
	#endif
	
	fb.seekg( 0, ios::beg );
	while( !fb.eof() ){
		cont = 0;

		// gets the line with "#title"
		buf = fb.get();
		if( buf == '#' ){
			do{
				buf = fb.get();
				token[cont++] = buf;
			} while( buf != 0x0A );
			token[--cont] = 0x00;

			// retrieves in "str" the data asociated with "title"
			// and gets a string where tokens will be available for the user to parse
			if( !strcmp( token, tit ) ){
				cont = 0;
				do{
					buf = fb.get();
					str[cont++] = buf;
				}while( buf != 0x0A );
				str[--cont] = 0x00;

				delete title;
				return 1;
			}					
		}
	}
	delete title;
	return 0;
}


/** Initializes the parser to retrieve the tokens into a string
 *
 * This method initializes the parser in order to nextToken() method is able to extract
 * tokens. It's called once as long as nextToken() method doesn't return a null (which means
 * there are no more tokens to retrieve. \see nextToken().
 * \param s String null-terminated where the tokens are going to be extracted.
 * \param d Delimiter between tokens. The character ',' is used by default
*/
void Persistence::Tokenize( char *s, char d ){
	int len;

	len = strlen( s );
	string = new char[len];
	delimiter = d;
	strcpy( string, s );
}


/** Gets the next token in a string
 *
 * \param s String where the token will be stored.
 * \return The length of the token founded. If there were no tokens in the string, or it's 
 * a no valid one, this value has no sense.
 
*/
int Persistence::nextToken( char *s ){
	int cont=0;

	while( *string ){
		
		if( *string != delimiter )
			*(s+cont++) = *string++;
		else{
			*string++; // eats the delimiter
			*(s+cont++) = 0x00;
			return cont;
		}
	}
	if( !string )
		delete string;
	*(s+cont) = 0x00;
	return cont;
}

#endif // __PERSISTENCE_CPP