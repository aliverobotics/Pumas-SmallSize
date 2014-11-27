/** \file Persistence.h
 * \author fjrg76@hotmail.com
 *
 * \todo
*/

#ifndef __PERSISTENCE_H
#define __PERSISTENCE_H

#include "fstream.h"
#include "string.h"

#define READ_FILE	0
#define WRITE_FILE	1

/**	Write and read to a file in order to build and use the profile
 *	for the vision system's persistence.
 *
 *	It's intended to use ONLY one instance of this class,
 *	because makes no sense to have two of more of it
*/
class Persistence{
  private:
	char *title;
	char *string;
	char *temp;
	char start;
	char delimiter;
	fstream fb;
	char filename[255];

  public:
	Persistence( char, char );
	int Open( char *, int );
	void Close( void );
	void Write( char *tit, char *str );
	int Read( char *tit, char *str );
	void Tokenize( char *, char );
	int nextToken( char * );
	void setTag( char * );
	void addToken( char * );
	void lastToken( char *);
	int seekTag( char * );
	
};

#endif // __PERSISTENCE_H