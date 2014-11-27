/** \file SaveAndRestoreParameteres.h
 *
 *
 *
*/

#include "fstream.h"
#include "string.h"

// struct definitions
#include "colors.h"

// point definitions
#include "point.h"

/** \class SaveAndRestoreParameteres
 * \brief 
 *
 * Description
*/ 
class SaveAndRestoreParameteres{
  
  private:
	/*< Preprocessing controls */
	PREPROCESSPARAM preProcessing;
	
	/**< Colors */
	HSITH hsi[6];

	/**< Distance among blobs */
	int maxDist;

	/**< Blob's min area */
	int minBlobArea;

	/**< Internet stuff */
	char ipRemote[24];
	char ipLocal[24];
	int port;

	/**< Frame rate */
	int frameRate;

	/**< Team color */
	int teamColor;

	/**< Phisical field limits */
	FFIELD fieldLim;

	/**< Logical field limits */
	FIELD fieldLogicalLim;

	ofstream outFile;
	char fileName[64];


  public:
	/** Constructor 
	*/
	SaveAndRestoreParameteres(){
	}

	/** Open a file for writing
	*/
	int Open4Write( CString fileN ){
		outFile.open( "fileN" );

		if( outFile.fail() ){
			return 0;
		}

		return 1;
	}

	/** Close a file for writing 
	*/
	void Close4Write(){
		outFile.close();
	}

	int write( char *fileN ){
		outFile.open( fileN );

		if( outFile.fail() ){
			return 0;
		}

		
		
	}

	
    void setPreProcessing( int br, int con, int sat ){
        preProcessing.bright = br;
        preProcessing.contrast = con;
        preProcessing.saturation = sat;
    }

    PREPROCESSPARAM getPreProcessing( void ){
		return preProcessing;
	}

	void savePreProcessing( void ){
	}
		

};
