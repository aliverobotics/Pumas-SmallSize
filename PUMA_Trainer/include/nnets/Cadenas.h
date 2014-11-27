
//Class of strings
/*
 *  Cadenas.h
 *
 *  Created by Adalberto H. Llarena on 2/24/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CADENAS_
#define _CADENAS_

class Cadena
{
public:
	char Chars[255];

	Cadena()
	{
		for (int i=0;i<255;i++)
			Chars[i] = '\0';
	}

	~Cadena()
	{
	}

	void Print(void)
	{
		printf ("%s\n",Chars);
	}
};

//Gives the element from a string using separators

Cadena NextWord (const char line[255], int& indice)
{
	Cadena aString;

	int i=0;
	int k=indice;

	//saltamos los espacios en blanco
	while (line[k]==' ' || line[k]=='\t') k++;

	// obtenemos la palabra
	while (  (line[k]!=' ') && (line[k]!='\n') 
		  && (line[k]!='\0') && (line[k]!='\r')
		  && (line[k]!='\t') )
	{
		aString.Chars [i] = line[k];
		i++;
        k++;
	}

    while (line[k]==' ') k++;

	//agregamos el terminador
	aString.Chars [i]='\0';

    // revisamos el resultado
    //cout << aString.Chars << endl;

	indice = k;
	return aString;
}

//returns the string size

int Tamano (char aString[255])
{
	int i = 0;
	while ((aString[i]!='\0') && (aString[i]!='\n') 
		&& (aString[i]!='\r')) i++;
	return i;	
}

#endif
