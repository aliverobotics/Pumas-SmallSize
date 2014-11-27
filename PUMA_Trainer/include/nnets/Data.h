//Preprocessing and Data
/*
 *  Data.h
 *
 *  Created by Adalberto H. Llarena on 2/24/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
//class used to store and process data

#ifndef _Data_
#define _Data_

#define MAX_DEPTH_HISTO 256
#define MAX_HISTOS 30

class Histogram
{
public:
	int Data [MAX_DEPTH_HISTO]; //counting of data
	double PDF [MAX_DEPTH_HISTO]; //Probability Density Function
	int Depth;

	//constructor definition
	Histogram ()
	{
		Depth = MAX_DEPTH_HISTO;

		for (int i=0;i<MAX_DEPTH_HISTO;i++)
			this->Data [i] =0;
	}

    ~Histogram()
	{
	}
	
	void Clear_Histo (void)
	{
		for (int i=0;i<MAX_DEPTH_HISTO;i++)
			this->Data [i] =0;
			
	}
	
	void Print (void)
	{
		for (int i=0;i<MAX_DEPTH_HISTO;i++)
			printf ("Index %d = %d\n",i,this->Data [i]);
	}

	void Print_PDF (void)
	{
		for (int i=0;i<MAX_DEPTH_HISTO;i++)
			printf ("Index %d = %2.4f\n",i,this->PDF [i]);
	}
	
	void Inc (int i)
	{
		this->Data[i]++;
	}
	
	//counts the number of data
	int Count(void)
	{
	   int c=0;
	   for (int i=0;i<MAX_DEPTH_HISTO;i++)
			c=c+this->Data[i];
			
	   return c;
	}

	void Create_PDF(void)
	{
	   int total = this->Count();
	   int accumulated = 0;
	   for (int i=0;i<MAX_DEPTH_HISTO;i++)
	   {
	       accumulated = accumulated + this->Data[i];
	       this->PDF [i] = (double)accumulated/(double)total;
	   }
	}
	
	double Remap (int value)
	{
	   if (value>=MAX_DEPTH_HISTO)
	      return 1.0;
		  
	   return this->PDF[value];
	}
	
	int Anti_Remap (double value)
	{
		for (int i=0;i<MAX_DEPTH_HISTO;i++)
			if (this->PDF[i] >= value)
				return i;
				
		return MAX_DEPTH_HISTO-1;
	}
	
};

class Statistics
{
public:
   int Count;
   Histogram Data[MAX_HISTOS];
   
   Statistics()
   {
      Count = 0;
   }
   
   ~Statistics()
   {
   }   
   
	void Print (void)
	{
		for (int i=0;i<this->Count;i++)
		{
			printf ("Variable no %d: \n\n",i);
			this->Data[i].Print();
		}
	}
	
	void Print_PDF (void)
	{
		for (int i=0;i<this->Count;i++)
		{
			printf ("PDF Variable no %d: \n\n",i);
			this->Data[i].Print_PDF();
		}
	}
};

typedef enum {
    normalize =0,
	threshold =1,
	equalize =2,
	rawdata =3
	//used for indicating wich kind of preprocessing
	//we must do to the inputs of the network
	} Kind_Prepro;
	
class Data 
{
public:
	int NUM_COLUMNS;
	int NUM_ROWS; //Number of trains/test readed	
	//used to strore data
	double* Data_PTR; //to store data
	double* MaxMin_PTR; //to store max and min values
	double* Remap_PTR; //to store m and b for remapping
	Statistics *Stats_PTR; //to strore the statistics of data
	bool Trace;
	int Precision; //number of decimal places exclusive for writing
	int Places; //inetegr places for printing
	bool Reassigned_Prepro; //for avoiding freeing pointers when not owned
	
	Data()
	{
		this->NUM_COLUMNS = 0;
		this->NUM_ROWS = 0;
		this->Trace = false;
		this->Data_PTR=NULL;
		this->MaxMin_PTR=NULL;
		this->Remap_PTR=NULL;
		this->Stats_PTR=NULL;
		this->Precision = 2;
		this->Places = 5;
		this->Reassigned_Prepro = false;
	}

	Data(int aNumRow, int aNumColumn)
	{
		this->NUM_COLUMNS = aNumColumn;
		this->NUM_ROWS = aNumRow;
		this->Data_PTR=NULL;
		this->MaxMin_PTR=NULL;
		this->Remap_PTR=NULL;
		this->Stats_PTR=NULL;
		this->Get_Memory();
		this->Fill((double)0.0);
		this->Trace = false;
		this->Precision = 2;
	}
	
	void Free_PTRS(void)
	{
		if (this->Data_PTR!=NULL)
			//we free the memory
			free (this->Data_PTR);
			
		if (this->MaxMin_PTR!=NULL)
			//we free the memory
			free (this->MaxMin_PTR);	
	
		if (this->Remap_PTR!=NULL)
			//we free the memory
			free (this->Remap_PTR);	
			
		if (this->Stats_PTR!=NULL)
			//we free the memory
			free (this->Stats_PTR);	
						
		this->Data_PTR = NULL;
		this->MaxMin_PTR = NULL;
		this->Remap_PTR = NULL;
		this->Stats_PTR = NULL;
		this->NUM_COLUMNS = 0;
		this->NUM_ROWS = 0;
		this->Trace = false;
	}

	~Data()
	{
	   Free_PTRS();
	}
	
	//Prints the data
	void Print (void)
	{
		if (this->Data_PTR!=NULL)
			for (int i=0;i<this->NUM_ROWS;i++)
			{
				fprintf (stdout, "Data %4d: [",i+1);
				
				int j;
				
				//We print the inputs
				for (j=0;j<this->NUM_COLUMNS;j++)
					fprintf (stdout, "%3.3f ",
							 *(this->Data_PTR + i*(this->NUM_COLUMNS) + j));
		
				fprintf (stdout, "]\n");
			}
	}
	
	//Prints the mappings
	void Print_Prepro (void)
	{
		int j;
		
		if (this->MaxMin_PTR!=NULL)
		//we print maxmin values 
		for (j=0;j<this->NUM_COLUMNS;j++)
			printf ("Column : %d, Max: %f, Min %f\n", j,
				*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j),
				*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j));

		if (this->Remap_PTR!=NULL)
		//we print maxmin values 
		for (j=0;j<this->NUM_COLUMNS;j++)
			printf ("Column : %d, M: %f, B %f\n", j,
				*(this->Remap_PTR + 0*(this->NUM_COLUMNS) + j),
				*(this->Remap_PTR + 1*(this->NUM_COLUMNS) + j));	
	}
	
	//Fills the data with a value
	void Fill (double value)
	{
		if (this->Data_PTR!=NULL)
			for (int i=0;i<this->NUM_ROWS;i++)
				for (int j=0;j<this->NUM_COLUMNS;j++)
					 *(this->Data_PTR + i*(this->NUM_COLUMNS) + j) = value;
	}
	
	//Scans the data for a maximun and minimun values
	void Get_MaxMin (void)
	{
		int i, j;
		
		if (this->MaxMin_PTR)
			free (this->MaxMin_PTR);
		
		//we get memory
		this->MaxMin_PTR = (double*)malloc(
				2*(this->NUM_COLUMNS)*sizeof(double));
			  
		//we fill default values 
		for (j=0;j<this->NUM_COLUMNS;j++)
		{
			//maximun
			*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j) = (double)-1e+100;
			//miimun
			*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j) = (double)1e+100;
		}
					 
		for (i=0;i<this->NUM_ROWS;i++)
			for (j=0;j<this->NUM_COLUMNS;j++)
			{
				double value = *(this->Data_PTR + i*(this->NUM_COLUMNS) + j);
				//we find max value
				if (value>*(this->MaxMin_PTR + j))
					*(this->MaxMin_PTR + j) = value;
				//we find min value
				if (value<*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j))
					*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j) = value;
			}
			
		//if both limits are equal
		for (j=0;j<this->NUM_COLUMNS;j++)
			if (*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j)==
			    *(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j))
				//we add only a bit to maximun
				*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j) = 
				*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j) + 0.0001;
	}
	
	//Creates the mapping
	void Create_Mapping (double min, double max)
	{
		int j;
		double datamax, datamin;
		
		if (this->MaxMin_PTR==NULL)
			this->Get_MaxMin();
			
		if (this->Remap_PTR)
			free (this->Remap_PTR);
		
		//we get memory
		this->Remap_PTR = (double*)malloc(
				2*(this->NUM_COLUMNS)*sizeof(double));
			  
		//we fill default values 
		for (j=0;j<this->NUM_COLUMNS;j++)
		{
			//we are not post mapping from equalizing
			if (this->Stats_PTR==NULL) {
				datamax = *(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j);
				datamin = *(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j);
			}
			else {
				datamax = 1.0;  //because PDF goes from 0 to 1
				datamin = 0.0;
			}
			
			double m = (double)((max-min)/(datamax-datamin));
			double b = min-m*datamin;
			//m
			*(this->Remap_PTR + 0*(this->NUM_COLUMNS) + j) = m;
			//b
			*(this->Remap_PTR + 1*(this->NUM_COLUMNS) + j) = b;
		}
	}
	
	//Remaps one data vector to limits linnearly
	void Remap_Lin (double value[])
	{
		if (this->Remap_PTR)
		for (int j=0;j<this->NUM_COLUMNS;j++)
		{
			//if we are not post-scaling from eq. and directly from inputs
			if (this->Stats_PTR==NULL)
			{
				//we have to adjust it to the limits
				if (value[j]>*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j)) //>limits
					value[j]=*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j); //upper limit
					
				if (value[j]<*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j)) //<limits
					value[j]=*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j); //lower limit		
			}
			
			value[j]=			
			//m
			*(this->Remap_PTR + 0*(this->NUM_COLUMNS) + j) *
			//x
			value[j] +
			//b
			*(this->Remap_PTR + 1*(this->NUM_COLUMNS) + j);
		}
	}
	
	//Anti Remaps one data vector to limits linnearly
	void Anti_Remap_Lin (double value[])
	{
		if (this->Remap_PTR)
		for (int j=0;j<this->NUM_COLUMNS;j++)
		{
			//x = (y-b)/m
			value[j]=			
			//y
			(value[j] -
			//b
			*(this->Remap_PTR + 1*(this->NUM_COLUMNS) + j)) /
			//m
			*(this->Remap_PTR + 0*(this->NUM_COLUMNS) + j);
			
			//if we are not post-scaling from eq. and directly from inputs
			if (this->Stats_PTR==NULL)
			{
				//we have to adjust it to the limits
				if (value[j]>*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j)) //>limits
					value[j]=*(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + j); //upper limit
					
				if (value[j]<*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j)) //<limits
					value[j]=*(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + j); //lower limit		
			}		
		}
	}
	
	//gives the corresponding slice into the histogram of the column
	int Slice (double value, int column)
	{
		if (column>=this->NUM_COLUMNS)
			return -1;
			
		double max, min;
			
		max = *(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + column);
		min = *(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + column); 
	
		//we have to adjust it to the limits
		if (value>max) //>limits
			value=max; //upper limit
			
		if (value<min) //<limits
			value=min; //lower limit
					
		//we calculate the slice
		return (int)((value-min)/(max-min)*(MAX_DEPTH_HISTO-1));
	}
	
	//gives the corresponding x for a given slice
	double Anti_Slice (int slice, int column)
	{
		if (column>=this->NUM_COLUMNS)
			return -1;
			
		double max, min;
			
		max = *(this->MaxMin_PTR + 0*(this->NUM_COLUMNS) + column);
		min = *(this->MaxMin_PTR + 1*(this->NUM_COLUMNS) + column); 
	
		//we have to adjust it to the limits
		if (slice>(MAX_DEPTH_HISTO-1)) //>limits
			slice=(MAX_DEPTH_HISTO-1); //upper limit
			
		if (slice<0) //<limits
			slice=0; //lower limit
					
		//we calculate the center of slice
		return (double)((double)slice/(double)(MAX_DEPTH_HISTO-1)*(max-min)+min);
	}
	
	//Remaps one data vector according to probability density function
	void Remap_PDF (double value[])
	{
		if (this->Stats_PTR==NULL)
			return;
			
		for (int j=0;j<this->NUM_COLUMNS;j++)
			value[j]= this->Stats_PTR->Data[j].Remap(Slice(value[j],j));
	}
	
	//Anti Remaps one data vector according to probability density function
	void Anti_Remap_PDF (double value[])
	{
		if (this->Stats_PTR==NULL)
			return;
			
		for (int j=0;j<this->NUM_COLUMNS;j++)
			value[j]= this->Anti_Slice(this->Stats_PTR->Data[j].Anti_Remap(value[j]),j);
	}
	
	//preprocess external data to have same transformation than data
	void Preprocess	(double value[])
	{	
		//if exist statistics we are equalizing data
		if (this->Stats_PTR)
			this->Remap_PDF(value);
			
		//then we check if there is some mapping	
		if (this->Remap_PTR)	
			this->Remap_Lin(value);
	}
	
	//preprocess all internal data to have same transformation than data
	int Preprocess	(void)
	{	
		if (this->Data_PTR==NULL)
			return -1;

		for (int i=0;i<this->NUM_ROWS;i++)
		//We preprocess all data
			this->Preprocess(this->Data_PTR + i*(this->NUM_COLUMNS));
		
		return 0;
	}
	
	int Assign_Prepro (Data *aSet)
	{
		if (this->NUM_COLUMNS != aSet->NUM_COLUMNS)
			return -1;
			
		if (this->Reassigned_Prepro==false)
		{
			if (this->MaxMin_PTR!=NULL)
				//we free the memory
				free (this->MaxMin_PTR);	
		
			if (this->Remap_PTR!=NULL)
				//we free the memory
				free (this->Remap_PTR);	
				
			if (this->Stats_PTR!=NULL)
				//we free the memory
				free (this->Stats_PTR);	
		}
		
		//assign pointers
		this->MaxMin_PTR = aSet->MaxMin_PTR;
		this->Remap_PTR = aSet->Remap_PTR;
		this->Stats_PTR = aSet->Stats_PTR;
		this->Reassigned_Prepro = true;
		
		return 0;
	}
	
	//postprocesses given data to apply inverse of transformations
	void Postprocess (double value[])
	{	
		//then we check if there is some mapping	
		if (this->Remap_PTR)	
			this->Anti_Remap_Lin(value);
			
		//if exist statistics we are equalizing data
		if (this->Stats_PTR)
			this->Anti_Remap_PDF(value);
	}
	
	//Remaps inputs to given interval linnearly
	int Normalize (double min=0.0, double max=0.0)
	{
		if (this->Data_PTR==NULL)
			return -1;
		
		if (this->MaxMin_PTR==NULL) //first time normalizing
		{
			//we get maximun & minimun
			this->Get_MaxMin();
			
			//we create the mapping
			this->Create_Mapping(min,max);
		}
		
		for (int i=0;i<this->NUM_ROWS;i++)
		//We remap the data linnearly
			this->Remap_Lin(this->Data_PTR + i*(this->NUM_COLUMNS));
	}
	
	//analized data and create PDF's for all columns
	void Analize (void)
	{
	
	  if (this->Data_PTR==NULL)
			return;
			
	  if (this->Stats_PTR!=NULL)
			//we free the memory
			free (this->Stats_PTR);	
			
	  //we create new stats.
	  this->Stats_PTR =  new Statistics();
	  this->Stats_PTR->Count = this->NUM_COLUMNS;
		
	  //we get the maximun & minimun
	  this->Get_MaxMin();
	  
	  //We create the histograms
	  for (int j=0;j<this->NUM_COLUMNS;j++)
	  {
		 for (int i=0;i<this->NUM_ROWS;i++)
			//we increment data slice count
			this->Stats_PTR->Data[j].Inc(
				   this->Slice(*(this->Data_PTR + i*(this->NUM_COLUMNS) + j),j));
			
		 this->Stats_PTR->Data[j].Create_PDF();
	  }
	}
	
	void Equalize (double min=0.0, double max=0.0)
	{	
		if (this->Data_PTR==NULL)
			return;
			
		if (this->Stats_PTR==NULL) //first time equalizing
		{
			//we analize and create data histograms
			this->Analize();
			
			//if final scale not from 0 to 1
			if (min!=0 || max!=1)
				this->Create_Mapping(min,max);
		}
	  
		//We remap data to the PDF's
		for (int i=0;i<this->NUM_ROWS;i++)
		{
			this->Remap_PDF(this->Data_PTR + i*(this->NUM_COLUMNS));
			//if the final range is not 0..1
			if (min!=0 || max!=1)
				//we remap data to given limits
				this->Remap_Lin(this->Data_PTR + i*(this->NUM_COLUMNS));
		}
	}

	//thesholds a vector
	void Threshold (double value[], double level)
	{
		for (int j=0;j<this->NUM_COLUMNS;j++)
			if (value[j]<=level)
				value[j]=(double)0.0;
			else
				value[j]=(double)1.0;
	}
	
	//Converts the data to 1 or 0
	void Threshold (double level)
	{
		if (this->Data_PTR!=NULL)
			 for (int i=0;i<this->NUM_ROWS;i++)
			 //We threshold the data
				this->Threshold (this->Data_PTR + i*(this->NUM_COLUMNS), level);
	}
	
	//Prints the data
	int Write (char* FileName)
	{
		FILE *OutputFile;
		char cad[20];
		char buffer[5];
		
		//we create string for given integer places
		if (this->Places>=10)
		{
			buffer[0]='0'+this->Places/10;
			buffer[1]='0'+div(this->Places,10).rem;
			buffer[2]='\0';
		}
		else
		{
			buffer[0]='0'+this->Places;
			buffer[1]='\0';
		}
		
		strcpy (cad, "%");
		strcat (cad, buffer);
		
		//we create string for given Precision
		if (this->Precision>=10)
		{
			buffer[0]='0'+this->Precision/10;
			buffer[1]='0'+div(this->Precision,10).rem;
			buffer[2]='\0';
		}
		else 
		{
			buffer[0]='0'+this->Precision;
			buffer[1]='\0';
		}
		strcat (cad, ".");
		strcat (cad, buffer);
		
		strcat (cad, "f ");
		
		if (this->Data_PTR!=NULL)
		
			//create file for writing rows and columns
			if ( (OutputFile = fopen( FileName, "w" )) != NULL )
			{
				for (int i=0;i<this->NUM_ROWS;i++)
				{
					//We print the data
					for (int j=0;j<this->NUM_COLUMNS;j++)
						fprintf (OutputFile, cad /*"%5.4f "*/,
								 *(this->Data_PTR + i*(this->NUM_COLUMNS) + j));
			
					if (i<this->NUM_ROWS-1)
					   fprintf (OutputFile, "\n");
				}
				fclose(OutputFile);
				return 0;
			}
			else
			   return -1;
		else 
		   return -2; //data empty
	}
	
	void Clear_Memory(void)
	{
		if (this->Data_PTR!=NULL)
			//we free the memory
			free (this->Data_PTR);
			
		this->Data_PTR=NULL;
	}
							
	void Get_Memory(void)
	{
		this->Clear_Memory();
			
		//we get the memory
		this->Data_PTR=(double*)malloc(
		      this->NUM_ROWS*(this->NUM_COLUMNS)*sizeof(double));
	}
	
	//sets an specific data place
	void Set_Data (int row, int column, double value)
	{
	   *(this->Data_PTR + 
	        row*(this->NUM_COLUMNS) + column) = value;
	}
	
	//returns an specified data
	double Give_Data (int row, int column)
	{
	   return *(this->Data_PTR + 
			  row*(this->NUM_COLUMNS) + column);
	}
	
	//reads the data file
	int Read (char* FileName)
	{
		Cadena Cad;
		char line[255];
		int i,j;
		
		FILE *InputFile;
		
		i=0;
		//open file for counting rows and columns
		if ( (InputFile = fopen( FileName, "r" )) != NULL )
		{
		    if ( fgets (line, 255, InputFile ) != NULL)
			{
				 int k=0;
				 int col=0;
				 int tam = Tamano (line);
				 while (k<tam) 
				 {
					 Cadena Cad;
					 Cad = NextWord(line,k);
					 if (Cad.Chars[0] != '\0' && Cad.Chars[0] != '\r')
						col++; //siguiente columna
				 } //while k
				 this->NUM_COLUMNS = col;

				 i++;
			}

			while ( fgets( line, 255, InputFile ) != NULL)
				i++;
			//we close files
			fclose( InputFile );
		}
		else
			return -1;
		
		this->NUM_ROWS = i;
		
		//we get the memory
		this->Get_Memory();
		
		//open file for reading
		if ( (InputFile = fopen( FileName, "r" )) != NULL )
		{
			for (i=0;i<this->NUM_ROWS;i++)
			{
				fgets( line, 255, InputFile );
				
				int indice=0; //element of the string been readed
				
				//We read the inputs
				for (j=0;j<this->NUM_COLUMNS;j++)
				{
					Cad = NextWord(line,indice);
					this->Set_Data (i,j,(double) atof(Cad.Chars));
				}
			} //end for
			
			//we close file
			fclose( InputFile );
			
			//we print data
			if (this->Trace==true) 
			{
				this->Print (); 
				printf ("tamano = %d\n", this->NUM_COLUMNS);
				fprintf (stdout, "%d Records.\n", this->NUM_ROWS);
			}
			return 0;
		}
		else
			return -1;
	}
};


#endif
