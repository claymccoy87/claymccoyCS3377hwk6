//Clay McCoy
//CS 3377.501
//chm160030@utdallas.edu

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <sstream>
#include "cdk.h"

//Below is from the writeup and the example code he gave us

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25   //THIS CAN CREATE ERRORS BECAUSE THE WIDTH MAY BE TOO LONG FOR YOUR PUTTY WINDOW IF IT IS NOT FULL SCREEN
                       //It is this long in order to accomodate the longest allowed string length of 25
#define MATRIX_NAME_STRING "Binary File Contents"

//from the writeup
class BinaryFileHeader
{
 public:
  uint32_t magicNumber;    // 0xFEEDFACE
  uint32_t versionNumber;
  uint64_t numRecords;
};

const int maxRecordStringLength = 25;

class BinaryFileRecord
{
 public:
  uint8_t strlength;
  char stringBuffer[maxRecordStringLength];
};


using namespace std;

//Now begins our actual program
int main()
{
  //The first section of this code is from the example code he gave us

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  //I changed the names of the columns and rows to match the writeup
  const char 		*rowTitles[] = {"0", "a", "b", "c", "d", "e", "f"};
  const char 		*columnTitles[] = {"0", "a", "b", "c", "d", "e", "f"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);


  //This declares our ifstream and reads in the Header of the binary file to the class we defined above
  BinaryFileHeader *myHeader = new BinaryFileHeader();
  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);
  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));

  //For the magic number, we convert the uint32_t into a string and append it to the desired test, it gets printed later
  stringstream ss;
  ss << "0x" << hex << uppercase << myHeader->magicNumber;
  string binToStr1(ss.str());
  string printStr1 = "Magic: " + binToStr1;
  ss.str("");

  //repeat process as above except for the verionNumber
  ss << dec << nouppercase << myHeader->versionNumber;
  string binToStr2(ss.str());
  string printStr2 = "Version: " + binToStr2;
  ss.str("");

  //repeat process as above except for the number of records
  ss << myHeader->numRecords;
  string binToStr3(ss.str());
  string printStr3 = "NumRecords: " + binToStr3;
  ss.str("");

  //This puts our strings we made above into the columns of row 1 as required of us
  setCDKMatrixCell(myMatrix, 1, 1, printStr1.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, printStr2.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, printStr3.c_str());

  //declares an object to read the records from the bin file
  BinaryFileRecord *myRecord = new BinaryFileRecord();

  //the for loop runs once for each record as indicated by numRecords from the header
  for(unsigned int i = 2; i <= myHeader->numRecords+1; i++)
  {
    //read in a single record
    binInfile.read((char *) myRecord, sizeof(BinaryFileRecord));

    //cast the length to an int
    int castLenToInt = myRecord->strlength;
    
    //creates the desired string to put into the first column of each row
    ss << castLenToInt;
    string temp(ss.str());
    string printRecord = "strlen: " + temp;
    ss.str("");

    //puts our created string into the first column of each row that will have a record
    setCDKMatrixCell(myMatrix, i, 1, printRecord.c_str());
    //puts the actual string into the second column of each row that will have a record
    setCDKMatrixCell(myMatrix, i, 2, myRecord->stringBuffer);
  }

  //Once we have filled out the table, close our ifstream
  binInfile.close();

  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
