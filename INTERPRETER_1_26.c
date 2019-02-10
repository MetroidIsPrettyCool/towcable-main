#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

const int pastAmount = 100; // The maximum amount of time you can go back with PAST

/*
  Notes:
________________________________________________________________________________________________________________________________________________________________________________________________________  
  Token values:

     Nothing       : 0
     Numbers       : -1 * (value + 1). I.E. 0 = -1, 1 = -2, 12 = -13, 117 = -118, etc.
     Symbols       : @ = 200 + = 201 - = 202 & = 100 . = 101 , = 102
     Numerical Ops : RAND = 300 ADDN = 301 SUBN = 302 MULN = 303 DIV = 304 MODN = 305 PAST = 306 LOCN = 307 ATLO = 308 RVAL = 309
     IFs           : EQ = 400 NQ = 401 GT = 402 GE = 403 LT = 404 LE = 405
  
     Special       : 10 is the beginning of the array, and is ignored. 0 is a blank value. 7 is whitespace.
  
  Miscellanea:

     The old base 64 single-digit system has been overturned in favor of a base ten system. All whole numbers are valid (floating point and negative numbers may come in a revision of the languge)
     The reason no 1xx tokens are used is they originally referred to base 64 numbers, and I'm lazy and haven't changed the token scheme. (And probably won't) (& was added after this was typed)
     PAST support is coming, probably in AE4 (already happened, old me)
     PAST in 2D will require a bit of special code, as it will use 3 values (time, x coordinate, y coordinate) (Haha, Imagine being locked to only 1-d or 2-d. This post was made by n-d gang)
     TSNI will use the tokens values desribed above, because I'm lazy :P (I'm not sure why this was listed under "Things to come in AE4")

  AE5 new features:
     
     Infinite dimensions!
     Support for EQ, NQ, GT, LT, GE and LE
     Removal of comments (sorry future me) (re-added AEND comments in release 1.08)
  
  License:
      
     DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
     Version 2, December 2004
     Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
     Everyone is permitted to copy and distribute verbatim or modified
     copies of this license document, and changing it is allowed as long
     as the name is changed.
     DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
     0. You just DO WHAT THE FUCK YOU WANT TO.        
________________________________________________________________________________________________________________________________________________________________________________________________________                   
*/

int AEND (int *tokens, int tokenAmount, int *cell, int *cellSizes, int dimensions, int *cellPointerCoors, int *pastCell, int *registers)  {
  int i;
  int i2;
  int i3;
  int i4;
  int i5;
  int currMultiOp;
  int currLoc;
  int *valS = malloc(sizeof(int) * (2 * dimensions + 1)); // Initiallize an array big enough to fit all values passed to each MultiOp
  if (tokenAmount == 1 && *(tokens + 1) == 100) { // & should ALWAYS return -1
    return -1;
  }
  if (tokenAmount == 1 && *(tokens + 1) == 102) { // , also has a special return value (should only trigger with CMPXs (for now))
    return -2;
  }
  if (tokenAmount == 1 && *(tokens + 1) == 1738) { // r is an easter egg, btw
    printw("This is a dead meme\n");
    return 609;
  }
  for (;;)  { // Main loop (is escaped by a return, I believe)
    for (i = tokenAmount; *(tokens + i) < 300 && *(tokens + i) != 10; i--); // Search for a multiOp
    currMultiOp = *(tokens + i); // currMultiOp is set to the multiOp's token value
    currLoc = i; // Current location
    if (currMultiOp != 10)  { // While we haven't reached the end of the token array
      for (i2 = 0; i2 != ((currMultiOp == 308) ? dimensions : ((currMultiOp == 306) ? (2 * dimensions) + 1 : ((currMultiOp == 307) ? (2 * dimensions) : ((currMultiOp == 309) ? 1 : 2)))); i2++)  { // this loop fetches all the arguments for the currMultiOp and zeros their locations in the array
	for (i = currLoc; *(tokens + i) >= 300 || *(tokens + i) == 0; i++); // finds argument
        if (*(tokens + i) < 0)  { // Detokenify it if it's a number
	  *(valS + i2) = (*(tokens + i) * -1) - 1;
	}
	else {
	  *(valS + i2) = *(tokens + i); // otherwise, just store it
	}
	*(tokens + i) = 0; // Zero the location
      }
      if (currMultiOp == 300)  { // RAND (inclusive, I believe)
 	*(tokens + currLoc) = (((rand() % (*(valS + 1))) + *valS) + 1) * -1; // This line is used to insert the result into the token array
      }
      if (currMultiOp == 301)  { // ADDN
 	*(tokens + currLoc) = ((*valS + *(valS + 1)) + 1)  * -1; // Ditto
      }
      if (currMultiOp == 302)  { // SUBN
 	*(tokens + currLoc) = ((*valS - *(valS + 1)) + 1) * -1; // You get the point
 	if (*(tokens + currLoc) > 0)  { // Check for overflow
 	  *(tokens + currLoc) = 0;
 	}
      }
      if (currMultiOp == 303)  { // MULN
 	*(tokens + currLoc) = ((*valS * *(valS + 1)) + 1) * -1;
      }
      if (currMultiOp == 304)  { // DIVN
	if (*(valS + 1) == 0)  { // If we divide by zero, return 0
	  *(tokens + currLoc) = -1;
	}
	else  {
	  *(tokens + currLoc) = ((*valS / *(valS + 1)) + 1) * -1;
	}
      }
      if (currMultiOp == 305)  { // MODN
 	*(tokens + currLoc) = ((*valS % *(valS + 1)) + 1) * -1;
      }
      if (currMultiOp == 306)  { // PAST
        i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  if (*(valS + (2 * i2)) == 200) { // is the location modifier @?
	    while (*(valS + (2 * i2) + 1) > *(cellSizes + i2))  { // overflow checker
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // This is used to get the right offset for values hwen dealing with multiple dimensions, so that 2,2 points to 2,2, not 4,0
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += *(valS + (2 * i2 + 1)) * i4; // Update i3 with the most recent location of where we are pointing at
	  }
	  if (*(valS + (2 * i2)) == 201) { // +?
	    while (*(cellPointerCoors + i2) + *(valS + (2 * i2) + 1) >= *(cellSizes + i2))  { // overflow checker
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // This is used to get the right offset for values hwen dealing with multiple dimensions, so that 2,2 points to 2,2, not 4,0
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) + *(valS + (2 * i2 + 1))) * i4; // Update i3 with the most recent location of where we are pointing at
	  }
	  if (*(valS + (2 * i2)) == 202) { // -?
	    while (*(cellPointerCoors + i2) - *(valS + (2 * i2) + 1) < 0)  { // overflow checker
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // This is used to get the right offset for values hwen dealing with multiple dimensions, so that 2,2 points to 2,2, not 4,0
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) - *(valS + (2 * i2 + 1))) * i4; // Update i3 with the most recent location of where we are pointing at
	  }
	}
	i3 += *(valS + (2 * dimensions)) * i4; // Finally, factor in how far back in the past we're talkin'
	*(tokens + currLoc) = (*(pastCell + i3) + 1) * -1; // Return the result
      }
      if (currMultiOp == 307)  { // LOCN
	i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  if (*(valS + (2 * i2)) == 200) { // @
	    while (*(valS + (2 * i2) + 1) > *(cellSizes + i2))  { // Check for overflow
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // Calculate offset
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += *(valS + (2 * i2 + 1)) * i4;
	  }
	  if (*(valS + (2 * i2)) == 201) { // +
	    while (*(cellPointerCoors + i2) + *(valS + (2 * i2) + 1) >= *(cellSizes + i2))  { // Check for overflow
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // Calculate offset
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) + *(valS + (2 * i2 + 1))) * i4;
	  }
	  if (*(valS + (2 * i2)) == 202) { // -
	    while (*(cellPointerCoors + i2) - *(valS + (2 * i2) + 1) < 0)  { // Check for overflow
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // Calculate offset
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) - *(valS + (2 * i2 + 1))) * i4;
	  }
	}
	i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  if (*(valS + (2 * i2)) == 200) { // is the location modifier @?
	    while (*(valS + (2 * i2) + 1) > *(cellSizes + i2))  { // overflow checker
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // This is used to get the right offset for values hwen dealing with multiple dimensions, so that 2,2 points to 2,2, not 4,0
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += *(valS + (2 * i2 + 1)) * i4; // Update i3 with the most recent location of where we are pointing at
	  }
	  if (*(valS + (2 * i2)) == 201) { // +?
	    while (*(cellPointerCoors + i2) + *(valS + (2 * i2) + 1) >= *(cellSizes + i2))  { // overflow checker
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // This is used to get the right offset for values hwen dealing with multiple dimensions, so that 2,2 points to 2,2, not 4,0
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) + *(valS + (2 * i2 + 1))) * i4; // Update i3 with the most recent location of where we are pointing at
	  }
	  if (*(valS + (2 * i2)) == 202) { // -?
	    while (*(cellPointerCoors + i2) - *(valS + (2 * i2) + 1) < 0)  { // overflow checker
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  { // This is used to get the right offset for values hwen dealing with multiple dimensions, so that 2,2 points to 2,2, not 4,0
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) - *(valS + (2 * i2 + 1))) * i4; // Update i3 with the most recent location of where we are pointing at
	  }
	}
	*(tokens + currLoc) = (*(cell + i3) + 1) * -1; // Return result
      }
      if (currMultiOp == 308)  { // ATLO (at location)
	i5 = 0;
	for (i4 = 0; i4 != dimensions; i4++)  { // this loop does all the calculations
	  i2 = 1;
	  while (*(valS + i4) >= *(cellSizes + i4))  { // Deals w/ overflows
	    *(valS + i4) -= *(cellSizes + i4);
	  }
	  for (i3 = 0; i3 != i4; i3++)  { // figures out offset
	    i2 *= *(cellSizes + i3);
	  }
	  i5 += *(valS + i4) * i2; // adds a coordinate times its offset
	}
	*(tokens + currLoc) = (-1 * i5) - 1;
      }
      if (currMultiOp == 309)  { // RVAL
	*(tokens + currLoc) = (-1 * *(registers + *valS)) - 1;
      }
      if (currMultiOp == 400)  { // EQ (equals)
	if (*valS == *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 401)  { // NQ (not equals)
	if (*valS != *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 402)  { // GT (greater than)
	if (*valS > *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 403)  { // GE (greater than or equal to)
	if (*valS >= *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 404)  { // LT (less than)
	if (*valS < *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 405)  { // LE (less than or equal to)
	if (*valS <= *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
    }
    else { // Return the final result
      return (*(tokens + 1) * -1) -1;
    }
  }
}

int main (int argc, char *argv[])  {

  printf("DONE.\n");

  initscr(); // Enter curses

  scrollok(stdscr, true);
  idlok(stdscr, false);
  
  char filename [100];

  FILE *fp;

  strcpy(filename, argv[1]);
  fp = fopen(filename, "r");

  if (fp == NULL)  {
    printw("ILLEGAL FILENAME OR FILEPATH\n");
    refresh();
    return -1;
  }
  
  srand(time(0)); // Seed the Random Number Generator with the current time

  int i; // General Purpose Counter
  int i2; // General Purpose Counter
  int i3; // General Purpose Counter
  int i4; // General Purpose Counter
  int i5; // General Purpose Counter

  int step = 0;

  int input = 10; // General purpose input variable

  int D = 1; // Dimensions (infinite)

  int it;
  
  int *cellSizes = calloc(D, sizeof(int)); // The number of numbers used to describe the length / area / volume etc.
  
  it = 1;
  
  for (i = 0; i != D; i++)  { // Figure out how big the cell array needs to be
    it *= *(cellSizes + i);
  }

  int *cell = calloc(i2, sizeof(int)); // Pointer to the cells
  int *ruleCell = calloc(i2, sizeof(int)); // Array used for when rules or complexes are running
  int *cellPointerCoors = calloc(D, sizeof(int)); // Stores the cell pointer coordinates
  int *pastCell = calloc(i2 * pastAmount, sizeof(int));

  int tokenAmount; // Stores the number of tokens in the list
  int tracker;
  int executed;
  int *tokens = malloc(sizeof(int));;

  int argAmount;
  int *arguments = malloc(sizeof(int));
  int argStop;

  int currCommand;
  char inptStr [1000000];
  int inStrPtr;

  int init = 1; // Are we in initialization mode?

  int c;

  int inChr;
  int len;

  int runRule = 0;
  char **ruleStore = malloc(sizeof(char*));
  int *ruleLocs;
  int *allocedRules = calloc(0, sizeof(int));
  int ruleLocAmount;

  int largestRuleID = 0;
  int prevLargestRuleID;

  char **cmpxStore = malloc(sizeof(char*));
  int *allocedCmpxs = calloc(0, sizeof(int));

  int largestCmpxID = 0;
  int prevLargestCmpxID;

  long readAmount = 0;
  long lastComm = 0;
  long *loopLocations = malloc(sizeof(long));
  int *loopAmounts = malloc(sizeof(int));
  int largestLoopID = 0;
  int prevLargestLoopID;
  int *allocedLoops = calloc(0, sizeof(int));
  int calledLoop = 0;

  int inCond = 0;

  int *registers = malloc(sizeof(int));
  int *allocedRegisters = malloc(sizeof(int));
  int largestRegisterID = 0;
  int prevLargestRegisterID;
  int encRVAL = 0; // encountered RVAL?
  
  inChr = fgetc(fp);
  
  for (;;)  {
    argStop = 0; // Reset various variables
    free(arguments);
    arguments = malloc(sizeof(int) * 2);
    inStrPtr = 4;

    if (runRule == 0)  { // If we aren't running a rule or cmpx
    
      strcpy(inptStr, ""); // Clear the input string

      for (inStrPtr; inChr  == ' ' || inChr  == '\t' || inChr  == '\n' || inChr  == '(' || inChr  == ')' || inChr == '!'; inChr = fgetc(fp))  { // Read all whitespace before command starts
	readAmount++;
	if (inChr == EOF)  {
	  endwin();
	  printf("ERR 02: UNEXPECTED EOF\n");
	  return -1;
	}
	if (inChr == '!')  {
	  inChr = fgetc(fp);
	  readAmount++;
	  if (inChr == EOF)  {
	    endwin();
	    printf("ERR 02: UNEXPECTED EOF\n");
	    return -1;
	  }
	  for (; inChr != '!'; inChr = fgetc(fp))  {
	    readAmount++;
	    if (inChr == EOF)  {
	      endwin();
	      printf("ERR 02: UNEXPECTED EOF\n");
	      return -1;
	    }
	  }
	}
      }

      lastComm = readAmount - 1;

      while (inChr != '.')  { // Read command into input string
	len = strlen(inptStr);
	inptStr[len] = inChr;
	inptStr[len + 1] = '\0';

	inChr = fgetc(fp);
	readAmount++;
	if (inChr == EOF)  {
	  endwin();
	  printf("ERR 02: UNEXPECTED EOF\n");
	  return -1;
	}
      }

      len = strlen(inptStr); // Read the period into the string
      inptStr[len] = inChr;
      inptStr[len + 1] = '\0';

      inChr = fgetc(fp);
      readAmount++;
      if (inChr == EOF)  {
	endwin();
	printf("ERR 02: UNEXPECTED EOF\n");
	return -1;
      }

    }
    else  {
      i3 = 1;
      for (i = 0; i != D; i++)  {
	i2 = *(cellSizes + i);
	*(cellPointerCoors + i) = floor((*(ruleLocs + i4) / i3) % i2);
	i3 *= *(cellSizes + i);
      }
    }
    if (init == 1)  { // Initialiaztion commands
      if (*(inptStr) == 'D' && *(inptStr + 1) == 'I' && *(inptStr + 2) == 'M' && *(inptStr + 3) == 'N')  {
	currCommand = 500;
      }
      else if (*(inptStr) == 'S' && *(inptStr + 1) == 'I' && *(inptStr + 2) == 'Z' && *(inptStr + 3) == 'E')  {
	currCommand = 501;
      }
      else if (*(inptStr) == 'S' && *(inptStr + 1) == 'E' && *(inptStr + 2) == 'T' && *(inptStr + 3) == 'P')  {
	currCommand = 502;
      }
      else if (*(inptStr) == 'R' && *(inptStr + 1) == 'U' && *(inptStr + 2) == 'L' && *(inptStr + 3) == 'E')  {
	currCommand = 503;
      }
      else if (*(inptStr) == 'C' && *(inptStr + 1) == 'M' && *(inptStr + 2) == 'P' && *(inptStr + 3) == 'X')  {
	currCommand = 504;
      }
      else if (*(inptStr) == 'B' && *(inptStr + 1) == 'E' && *(inptStr + 2) == 'G' && *(inptStr + 3) == 'N')  {
	currCommand = 505;
      }
      else if (*(inptStr) == 'R' && *(inptStr + 1) == 'E' && *(inptStr + 2) == 'G' && *(inptStr + 3) == 'I')  {
        currCommand = 506;
      }
      else  {
	endwin();
	printf("ERR 01: ILLEGAL OR UNEXPECTED COMMAND \"%c%c%c%c\"\n", *(inptStr), *(inptStr + 1), *(inptStr + 2), *(inptStr + 3));
	return -1;
      }
    }
    else  { // Runtime commands
      if (*(inptStr) == 'O' && *(inptStr + 1) == 'U' && *(inptStr + 2) == 'P' && *(inptStr + 3) == 'T')  {
	currCommand = 600;
      }
      else if (*(inptStr) == 'C' && *(inptStr + 1) == 'R' && *(inptStr + 2) == 'P' && *(inptStr + 3) == 'T')  {
	currCommand = 601;
      }
      else if (*(inptStr) == 'I' && *(inptStr + 1) == 'N' && *(inptStr + 2) == 'P' && *(inptStr + 3) == 'T')  {
	currCommand = 602;
      }
      else if (*(inptStr) == 'C' && *(inptStr + 1) == 'A' && *(inptStr + 2) == 'L' && *(inptStr + 3) == 'L')  {
	currCommand = 603;
      }
      else if (*(inptStr) == 'R' && *(inptStr + 1) == 'U' && *(inptStr + 2) == 'L' && *(inptStr + 3) == 'E')  {
	currCommand = 503;
      }
      else if (*(inptStr) == 'C' && *(inptStr + 1) == 'A' && *(inptStr + 2) == 'L' && *(inptStr + 3) == 'X')  {
	currCommand = 604;
      }
      else if (*(inptStr) == 'C' && *(inptStr + 1) == 'M' && *(inptStr + 2) == 'P' && *(inptStr + 3) == 'X')  {
	currCommand = 504;
      }
      else if (*(inptStr) == 'L' && *(inptStr + 1) == 'O' && *(inptStr + 2) == 'O' && *(inptStr + 3) == 'P')  {
	currCommand = 605;
      }
      else if (*(inptStr) == 'L' && *(inptStr + 1) == 'O' && *(inptStr + 2) == 'T' && *(inptStr + 3) == 'O')  {
	currCommand = 606;
      }
      else if (*(inptStr) == 'C' && *(inptStr + 1) == 'O' && *(inptStr + 2) == 'N' && *(inptStr + 3) == 'D')  {
	currCommand = 607;
      }
      else if (*(inptStr) == 'E' && *(inptStr + 1) == 'N' && *(inptStr + 2) == 'D' && *(inptStr + 3) == 'C')  {
	currCommand = 608;
      }
      else if (*(inptStr) == 'S' && *(inptStr + 1) == 'T' && *(inptStr + 2) == 'R' && *(inptStr + 3) == 'O')  { // String output
	if (inCond == 0)  {
	  for (; *(inptStr + inStrPtr)  == ' ' || *(inptStr + inStrPtr)  == '\t' || *(inptStr + inStrPtr)  == '\n' || *(inptStr + inStrPtr)  == '(' || *(inptStr + inStrPtr)  == ')' || *(inptStr + inStrPtr) == '!'; inStrPtr++)  { // Read all leading whitespace
	    if (*(inptStr + inStrPtr) == '!')  {
	      inStrPtr++;
	      for (; *(inptStr + inStrPtr) != '!'; inStrPtr++);
	    }
	  }
	  if (*(inptStr + inStrPtr) != '\"')  { // Throw error if the next character is not a "
	    endwin();
	    printf("ERR 20: MISSING DOUBLE QUOTE\n");
	    return -1;
	  }
	  inStrPtr++;
	  while (*(inptStr + inStrPtr) != '\"')  { // Deals with escape sequences
	    if (*(inptStr + inStrPtr) == '\n')  {
	      endwin();
	      printf("ERR 14: NEWLINE IN STRING\n");
	      return -1;
	    }
	    if (*(inptStr + inStrPtr) == '\\')  {
	      inStrPtr++;
	      if (*(inptStr + inStrPtr) == 'n')  {
		printw("\n");
	      }
	      else if (*(inptStr + inStrPtr) == 't')  {
		printw("\t");
	      }
	      else if (*(inptStr + inStrPtr) == '\"')  {
		printw("\"");
	      }
	      else if (*(inptStr + inStrPtr) == '\\')  {
		printw("\\");
	      }
	      else if (*(inptStr + inStrPtr) == 'b')  {
		printw("\b");
	      }
	      else  {
		endwin();
		printf("ERR 22: ILLEGAL ESCAPE SEQUENCE\n");;
		return -1;
	      }
	    }
	    else  { // Print the current character read
	      printw("%c", *(inptStr + inStrPtr));
	    }
	    inStrPtr++;
	  }
	  inStrPtr++;
	  for (; *(inptStr + inStrPtr)  == ' ' || *(inptStr + inStrPtr)  == '\t' || *(inptStr + inStrPtr)  == '\n' || *(inptStr + inStrPtr)  == '(' || *(inptStr + inStrPtr)  == ')' || *(inptStr + inStrPtr) == '!'; inStrPtr++)  { // Deal with ending spaces
	    if (*(inptStr + inStrPtr) == '!')  {
	      inStrPtr++;
	      for (; *(inptStr + inStrPtr) != '!'; inStrPtr++);
	    }
	  }
	}
	continue;
	refresh();
      }
      else if (*(inptStr) == 'E' && *(inptStr + 1) == 'X' && *(inptStr + 2) == 'I' && *(inptStr + 3) == 'T')  {
	currCommand = 610;
      }
      else if (*(inptStr) == 'T' && *(inptStr + 1) == 'E' && *(inptStr + 2) == 'A' && *(inptStr + 3) == 'P')  {
	endwin();
	printf("ERR 42: I'M A TEAPOT\n");
	return -1;
      }
      else if (*(inptStr) == 'S' && *(inptStr + 1) == 'E' && *(inptStr + 2) == 'T' && *(inptStr + 3) == 'R')  {
	currCommand = 611;
      }
      else if (*(inptStr) == 'C' && *(inptStr + 1) == 'L' && *(inptStr + 2) == 'R' && *(inptStr + 3) == 'S')  {
	currCommand = 612;
      }
      else  {
	endwin();
	printf("ERR 01: ILLEGAL OR UNEXPECTED COMMAND \"%c%c%c%c\"\n", *(inptStr), *(inptStr + 1), *(inptStr + 2), *(inptStr + 3));
	return -1;
      }
    }
    for (argAmount = 0; argStop != 1; argAmount++)  { // Reads the arguments into the argument array
      input = 10;
      tracker = 0;
      executed = 0;
      free(tokens);
      tokens = malloc(sizeof(int) * 2); // Pointer to the list of tokens
      for (tokenAmount = 0; tracker != 0 || executed == 0; tokenAmount++)  { // Loop to fill tokens
	executed = 1;
	if (input != 7)  {
	  *(tokens + tokenAmount) = input; // Put the input into tokens
	  tokens = realloc(tokens, (sizeof(int) * (tokenAmount + 2))); // Increase the size of tokens to fit the amount of tokens
	}
	if ((input < 0 || input == 100 || input == 102 || input == 1738 || (input >= 200 && input < 203)) && input != 7)  {
	  tracker--;
	}
	else if (input != 7)  {
	  tracker += ((input == 308) ? D : ((input == 306) ? (2 * D) + 1 : ((input == 307) ? (2 * D) : ((input == 309) ? 1 : 2)))) - 1; // Figures out how many arguments each command takes
	}
	if (tracker != 0)  {
	  if (isdigit(*(inptStr + inStrPtr)))  { // Gets numbers
	    input = 0;
	    while (isdigit(*(inptStr + inStrPtr)))  {
	      input *= 10;
	      input += *(inptStr + inStrPtr) - '0';
	      inStrPtr++;
	    }
	    if (*(inptStr + inStrPtr) != 'i')  {
	      endwin();
	      printf("ERR 19: NUMBER TERMINATED BY ILLEGAL CHARACTER '%c'\n", *(inptStr + inStrPtr));
	      return -1;
	    }
	    if (encRVAL && *(allocedRegisters + input) == 0)  { // checks for error 27
	      endwin();
	      printf("ERR 27: ATTEMPTING TO READ VALUE OF NON-EXISTANT REGISTER\n");
	      return -1;
	    }
	    encRVAL = 0;
	    input = (-1 * input) - 1;
	    inStrPtr++;
	  }
	  else if (isalpha(*(inptStr + inStrPtr)))  { //Gets multiOps
	    if (*(inptStr + inStrPtr) == 'E' && *(inptStr + inStrPtr + 1) == 'Q')  {
	      input = 400;
	      inStrPtr += 2;
	    }
	    else if (*(inptStr + inStrPtr) == 'N' && *(inptStr + inStrPtr + 1) == 'Q')  {
	      input = 401;
	      inStrPtr += 2;
	    }
	    else if (*(inptStr + inStrPtr) == 'G' && *(inptStr + inStrPtr + 1) == 'T')  {
	      input = 402;
	      inStrPtr += 2;
	    }
	    else if (*(inptStr + inStrPtr) == 'G' && *(inptStr + inStrPtr + 1) == 'E')  {
	      input = 403;
	      inStrPtr += 2;
	    }
	    else if (*(inptStr + inStrPtr) == 'L' && *(inptStr + inStrPtr + 1) == 'T')  {
	      input = 404;
	      inStrPtr += 2;
	    }
	    else if (*(inptStr + inStrPtr) == 'L' && *(inptStr + inStrPtr + 1) == 'E')  {
	      input = 405;
	      inStrPtr += 2;
	    }
	    else if (*(inptStr + inStrPtr) == 'R' && *(inptStr + inStrPtr + 1) == 'A' && *(inptStr + inStrPtr + 2) == 'N' && *(inptStr + inStrPtr + 3) == 'D')  {
	      input = 300;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'A' && *(inptStr + inStrPtr + 1) == 'D' && *(inptStr + inStrPtr + 2) == 'D' && *(inptStr + inStrPtr + 3) == 'N')  {
	      input = 301;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'S' && *(inptStr + inStrPtr + 1) == 'U' && *(inptStr + inStrPtr + 2) == 'B' && *(inptStr + inStrPtr + 3) == 'N')  {
	      input = 302;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'M' && *(inptStr + inStrPtr + 1) == 'U' && *(inptStr + inStrPtr + 2) == 'L' && *(inptStr + inStrPtr + 3) == 'N')  {
	      input = 303;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'D' && *(inptStr + inStrPtr + 1) == 'I' && *(inptStr + inStrPtr + 2) == 'V' && *(inptStr + inStrPtr + 3) == 'N')  {
	      input = 304;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'M' && *(inptStr + inStrPtr + 1) == 'O' && *(inptStr + inStrPtr + 2) == 'D' && *(inptStr + inStrPtr + 3) == 'N')  {
	      input = 305;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'P' && *(inptStr + inStrPtr + 1) == 'A' && *(inptStr + inStrPtr + 2) == 'S' && *(inptStr + inStrPtr + 3) == 'T')  {
	      input = 306;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'L' && *(inptStr + inStrPtr + 1) == 'O' && *(inptStr + inStrPtr + 2) == 'C' && *(inptStr + inStrPtr + 3) == 'N')  {
	      input = 307;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'A' && *(inptStr + inStrPtr + 1) == 'T' && *(inptStr + inStrPtr + 2) == 'L' && *(inptStr + inStrPtr + 3) == 'O')  {
	      input = 308;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'R' && *(inptStr + inStrPtr + 1) == 'V' && *(inptStr + inStrPtr + 2) == 'A' && *(inptStr + inStrPtr + 3) == 'L')  {
	      input = 309;
	      inStrPtr += 4;
	      encRVAL = 1;
	    }
	    else if (*(inptStr + inStrPtr) == 'r')  {
	      input = 1738;
	      inStrPtr++;
	    }
	    else {
	      endwin();
	      printf("ERR 05: ILLEGAL OR UNEXPECTED CHARACTER '%c' AT %i\n", *(inptStr + inStrPtr), inStrPtr);
	      return -1;
	    }
	  }
	  else {
	    if (*(inptStr + inStrPtr) == ' ' || *(inptStr + inStrPtr) == '\t' || *(inptStr + inStrPtr) == '\n' || *(inptStr + inStrPtr) == '(' || *(inptStr + inStrPtr) == ')')  {
	      input = 7;
	      tokenAmount--;
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '&')  {
	      if ((argAmount != 1 && currCommand != 600 && currCommand != 601) || ((currCommand < 599 && currCommand > 605) || currCommand == 602))  {
		endwin();
		printf("ERR 23: & IN ILLEGAL LOCATION\n");
		return -1;
	      }
	      input = 100;
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '.')  {
	      input = 101;
	      if (tracker != 2 && tokenAmount != 0)  {
		endwin();
		printf("ERR 04: ONE OR MORE ARGUMENTS TERMINATED EARLY\n");
		return -1;
	      }
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == ',')  {
	      input = 102;
	      if (currCommand != 504)  {
		endwin();
		printf("ERR 24: COMMA OUTSIDE CMPX\n");
		return -1;
	      }
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '@')  {
	      input = 200;
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '+')  {
	      if (currCommand != 503 && currCommand != 504)  {
		endwin();
		printf("ERR 13: USE OF + OR - OUTSIDE OF RULE OR CMPX\n");
		return -1;
	      }
	      input = 201;
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '-')  {
	      if (currCommand != 503 && currCommand != 504)  {
		endwin();
		printf("ERR 13: USE OF + OR - OUTSIDE OF RULE OR CMPX\n");
		return -1;
	      }
	      input = 202;
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '!')  {
	      inStrPtr++;
	      for (; *(inptStr + inStrPtr) != '!'; inStrPtr++);
	      inStrPtr++;
	      tokenAmount--;
	      input = 7;
	    }
	    else {
	      endwin();
	      printf("ERR 05: ILLEGAL OR UNEXPECTED CHARACTER '%c' AT %i\n", *(inptStr + inStrPtr), inStrPtr);
	      return -1;
	    }
	  }
	  if (tokenAmount == 0 && input == 101)  {
	    argStop = 1;
	    break;
	  }
	}
      }
      if (argStop == 0)  {
	tokenAmount--;
	*(arguments + argAmount) = AEND(tokens, tokenAmount, cell, cellSizes, D, cellPointerCoors, pastCell, registers);
	arguments = realloc(arguments, sizeof(int) * (argAmount + 2));
      }
    }
    // Execute commands:
    argAmount -= 2;
    if (init == 1)  {
      if (currCommand == 500 && step == 0)  { // DIMN
	if (*arguments <= 0)  {
	  endwin();
	  printf("ERR 06: DIMENSIONS 0\n");
	  return -1;
	}
	if (argAmount != 0)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	step = 1;
	D = *arguments;
	free(cellSizes);
	cellSizes = calloc(D, sizeof(int));
      }
      else if (currCommand == 500)  { // DIMN ERROR
	endwin();
	printf("ERR 08: MULTIPLE CALLS TO DIMN\n");
	return -1;
      }
      else if (currCommand == 501 && step == 1)  { // SIZE
	if (argAmount != D - 1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	for (i = 0; i != D; i++)  {
	  input = *(i + arguments);
	  if (input <= 0)  {
	    endwin();
	    printf("ERR 07: LENGTH 0\n");
	    return -1;
	  }
	  *(cellSizes + i) = input;
	}
	it = 1;
  
	for (i = 0; i != D; i++)  { // Figure out how big the cell array needs to be
	  it *= *(cellSizes + i);
	}
	free(cell);
	cell = calloc(it, sizeof(int)); // Pointer to the cells
	free(ruleCell);
	ruleCell = calloc(it, sizeof(int)); // Pointer to the cells for rules or complexes
	free(cellPointerCoors);
	cellPointerCoors = calloc(D, sizeof(int)); // Stores the cell pointer coordinates
	free(pastCell);
	pastCell = calloc(it * pastAmount, sizeof(int));
	step = 2;
      }
      else if (currCommand == 501 && step > 1)  { // SIZE ERROR
	endwin();
	printf("ERR 09: MULTIPLE CALLS TO SIZE\n");
	return -1;
      }
      else if (currCommand == 502 && step == 2)  { // SETP
	if (argAmount != it - 1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	for (i = 0; i != it; i++)  {
	  *(cell + i) = *(arguments + i);
	}
	step = 3;
      }
      else if (currCommand == 502 && step > 2)  { // SIZE ERROR
	endwin();
	printf("ERR 10: MULTIPLE CALLS TO SETP\n");
	return -1;
      }
      else if (currCommand == 503 && (step == 2 || step == 3))  { // RULE
	if (argAmount <= 2)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments > largestRuleID)  { // If the ID of the current rule is greater than the previous largest ID
	  prevLargestRuleID = largestRuleID;
	  largestRuleID = *arguments;
	  allocedRules = realloc(allocedRules, sizeof(int) * largestRuleID);
	  for (i3 = prevLargestRuleID; i3 != largestRuleID - 1; i3++)  {
	    *(allocedRules + i3) = 0;
	  }
	  ruleStore = realloc(ruleStore, sizeof(char*) * (largestRuleID + 1));
	}
	if (*(allocedRules + *arguments) == 1)  {
	  endwin();
	  printf("ERR 12: RULE ID ALREADY IN USE\n");
	  return -1;
	}
	*(allocedRules + *arguments) = 1;
	*(ruleStore + *arguments) = malloc(strlen(inptStr)); // Store the string
	*(ruleStore + *arguments) = strcpy(*(ruleStore + *arguments), inptStr);
      }
      else if (currCommand == 504 && (step == 2 || step == 3))  { // CMPX
	if (argAmount <= 2 || *arguments == -2)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments > largestCmpxID)  { // If the ID of the current rule is greater than the previous largest ID
	  prevLargestCmpxID = largestCmpxID;
	  largestCmpxID = *arguments;
	  allocedCmpxs = realloc(allocedCmpxs, sizeof(int) * (largestCmpxID + 1));
	  for (i3 = prevLargestCmpxID; i3 != largestCmpxID - 1; i3++)  {
	    *(allocedCmpxs + i3) = 0;
	  }
	  cmpxStore = realloc(cmpxStore, sizeof(char*) * (largestCmpxID + 1));
	}
	if (*(allocedCmpxs + *arguments) == 1)  {
	  endwin();
	  printf("ERR 21: CMPX ID ALREADY IN USE\n");
	  return -1;
	}
	*(allocedCmpxs + *arguments) = 1;
	*(cmpxStore + *arguments) = malloc(strlen(inptStr)); // Store the string
	*(cmpxStore + *arguments) = strcpy(*(cmpxStore + *arguments), inptStr);
      }
      else if (currCommand == 506 && (step == 2 || step == 3))  { // REGI
	if (argAmount != 0)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments > largestRegisterID)  { // if the register value is largest then the previuos largest, resize the register array
	  prevLargestRegisterID = largestRegisterID;
	  largestRegisterID = *arguments;
	  registers = realloc(registers, sizeof(int) * *arguments);
	  allocedRegisters = realloc(allocedRegisters, sizeof(int) * *arguments);
	  for (i = prevLargestRegisterID; i != largestRegisterID - 1; i++)  {
	    *(allocedRegisters + i) = 0;
	  }
	}
	else if (*(allocedRegisters + *arguments) == 1)  {
	  endwin();
	  printf("ERR 25: REGISTER ALREADY EXISTS\n");
	  return -1;
	}
	*(allocedRegisters + *arguments) = 1;
	*(registers + *arguments) = 0; // Initiallize the register
      }
      else if (currCommand == 505 && (step == 2 || step == 3))  {
	init = 0;
      }
      else if (currCommand == 504 || currCommand == 505)  {
	endwin();
	printf("ERR 00: PROGRAM LACKS NECCESARY SETUP COMMANDS\n");
	return -1;
      }
    }
    else if (inCond == 0)  {
      if (currCommand == 600)  { // OUPT
	if (argAmount == -1 || (argAmount != 0 && *arguments == -1))  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (argAmount == 0 && *arguments == -1)  { // INFINITE
	  i = 0;
	  for (i3 = 1; i != D; i++)  {
	    i3 *= *(cellSizes + i);
	  }
	  for (i2 = 0; i2 != i3; i2++)  {
	    printw("%d ", *(cell + i2));
	    if (i2 != 0)  {
	      if ((i2 + 1) % *cellSizes == 0)  {
		printw("\n");
	      }
	    }
	  }
	}
	else  {
	  for (i2 = 0; i2 != argAmount + 1; i2++)  {
	    if (*(i2 + arguments) >= it)  {
	      endwin();
	      printf("ERR 15: I/O ACCESSING ILLEGAL LOCATION\n");
	      return -1;
	    }
	    printw("%d ", *(cell + *(i2 + arguments)));
	  }
	  printw("\n");
	}
	refresh();
      }
      else if (currCommand == 601)  { // CRPT
	if (argAmount == -1 || (argAmount != 0 && *arguments == -1))  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (argAmount == 0 && *arguments == -1)  { // INFINITE
	  i = 0;
	  for (i3 = 1; i != D; i++)  {
	    i3 *= *(cellSizes + i);
	  }
	  for (i2 = 0; i2 != i3; i2++)  {
	    printw("%c ", *(cell + i2));
	    if (i2 != 0)  {
	      if ((i2 + 1) % *cellSizes == 0)  {
		printw("\n");
	      }
	    }
	  }
	}
	else  {
	  for (i2 = 0; i2 != argAmount + 1; i2++)  {
	    if (*(i2 + arguments) >= it)  {
	      endwin();
	      printf("ERR 15: I/O ACCESSING ILLEGAL LOCATION\n");
	      return -1;
	    }
	    printw("%c ", *(cell + *(i2 + arguments)));
	  }
	  printw("\n");
	}
	refresh();
      }
      else if (currCommand == 602)  { // INPT
	if (argAmount != 0)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments >= it)  {
	  endwin();
	  printf("ERR 15: I/O ACCESSING ILLEGAL LOCATION\n");
	  return -1;
	}
	scanw("%d%s", (cell + *arguments), NULL);
	if (*(cell + *arguments) < 0)  {
	  *(cell + *arguments) = 0;
	}
      }
      else if (currCommand == 603)  { // CALL
	if (argAmount == -1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*(allocedRules + *arguments) != 1 || *arguments > largestRuleID)  {
	  printf("ERR 17: CALLING NON-EXISTENT RULE OR CMPX\n");
	  return -1;
	}
	runRule = 1;
	strcpy(inptStr, *(ruleStore + *arguments)); // Copy the rule into input
	if (*(arguments + 1) != -1)  {
	  ruleLocAmount = argAmount - 1;
	  ruleLocs = malloc(sizeof(int) * ruleLocAmount);
	  for (i = 0; i != ruleLocAmount + 1; i++)  { // Copy all locations to be run upon into ruleLocs
	    if (*(arguments + i + 1) >= it)  {
	      endwin();
	      printf("ERR 18: RULE OR CMPX ACCESSING ILLEGAL LOCATION %i\n", *(arguments + i + 1));
	      return -1;
	    }
	    *(ruleLocs + i) = *(arguments + i + 1);
	  }
	}
	else  {
	  ruleLocAmount = it;
	  ruleLocs = malloc(sizeof(int) * ruleLocAmount);
	  for (i = 0; i != ruleLocAmount + 1; i++)  { // Copy all locations to be run upon into ruleLocs
	    *(ruleLocs + i) = i;
	  }
	}
	for (i = 0; i != it; i++)  { // Copycell into ruleCell
	  *(ruleCell + i) = *(cell + i);
	}
	i4 = 0;
      }
      else if (currCommand == 503 && runRule == 1)  { // RULE
	if (argAmount <= 2)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	i2 = 1;
	for (i = 3; i != argAmount + 1; i++)  {
	  i2 *= *(arguments + i);
	}
	if (i2 > 1)  {
	  endwin();
	  printf("ERR 11: NON-BOOLEAN RESULT FOR TEST\n");
	  return -1;
	}
	*(ruleCell + *(ruleLocs + i4)) = (i2 == 1) ? *(arguments + 1) : *(arguments + 2);
	i4++;
	if (i4 > ruleLocAmount)  { // Once we've gone through all the locations
	  for (i = 0; i != ruleLocAmount + 1; i++)  { // Update pastCell
	    for (i2 = pastAmount - 2; i2 != -1; i2--)  {
	      i3 = *(ruleLocs + i) + (it * i2);
	      *(pastCell + i3 + 1) = *(pastCell + i3 + 1);
	    }
	    *(pastCell + i3) = *(cell + i3);
	  }
	  for (i = 0; i != it; i++)  { // Copy ruleCell into cell
	    *(cell + i) = *(ruleCell + i);
	  }
	  free(ruleLocs);
	  runRule = 0;
	}
      }
      else if (currCommand == 604)  { // CALX
	if (argAmount == -1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*(allocedCmpxs + *arguments) != 1 || *arguments > largestCmpxID)  {
	  endwin();
	  printf("ERR 17: CALLING NON-EXISTENT RULE OR CMPX\n");
	  return -1;
	}
	runRule = 1;
	strcpy(inptStr, *(cmpxStore + *arguments)); // Copy the rule into input
	if (*(arguments + 1) != -1)  {
	  ruleLocAmount = argAmount - 1;
	  ruleLocs = malloc(sizeof(int) * ruleLocAmount);
	  for (i = 0; i != ruleLocAmount + 1; i++)  { // Copy all locations to be run upon into ruleLocs
	    if (*(arguments + i + 1) >= it)  {
	      endwin();
	      printf("ERR 18: RULE OR CMPX ACCESSING ILLEGAL LOCATION\n");
	      return -1;
	    }
	    *(ruleLocs + i) = *(arguments + i + 1);
	  }
	}
	else  {
	  ruleLocAmount = it;
	  ruleLocs = malloc(sizeof(int) * ruleLocAmount);
	  for (i = 0; i != ruleLocAmount + 1; i++)  { // Copy all locations to be run upon into ruleLocs
	    *(ruleLocs + i) = i;
	  }
	}
	for (i = 0; i != it; i++)  { // Copy cell into ruleCell
	  *(ruleCell + i) = *(cell + i);
	}
	i4 = 0;
      }
      else if (currCommand == 504 && runRule == 1)  { // CMPX
	if (argAmount <= 2)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	for (i5 = 2; i5 < argAmount; i5 = i + 1)  {
	  if (*(arguments + 1) == -2 || *(arguments + 2) == -2 || *(arguments + i5) == -2 || *(arguments + i5 + 1) == -2)  {
	    endwin();
	    printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	    return -1;
	  }
	  i2 = 1;
	  for (i = i5 + 1; (*(arguments + i) != -2) && (i != argAmount + 1); i++)  {
	    i2 *= *(arguments + i);
	  }
	  if (i2 > 1)  {
	    endwin();
	    printf("ERR 11: NON-BOOLEAN RESULT FOR TEST\n");
	    return -1;
	  }
	  if (i2 == 1)  {
	    *(ruleCell + *(ruleLocs + i4)) = *(arguments + i5);
	    i = argAmount - 1;
	  }
	  else  {
	    *(ruleCell + *(ruleLocs + i4)) = *(arguments + 1);
	  }
	}
	i4++;
	if (i4 > ruleLocAmount)  { // Once we've gone through all the locations
	  for (i = 0; i != ruleLocAmount + 1; i++)  { // Update pastCell
	    for (i2 = pastAmount - 2; i2 != -1; i2--)  {
	      i3 = *(ruleLocs + i) + (it * i2);
	      *(pastCell + i3 + 1) = *(pastCell + i3 + 1);
	    }
	    *(pastCell + i3) = *(cell + i3);
	  }
	  for (i = 0; i != it; i++)  { // Copy ruleCell into cell
	    *(cell + i) = *(ruleCell + i);
	  }
	  free(ruleLocs);
	  runRule = 0;
	}
      }
      else if (currCommand == 605)  { // LOOP
	if (argAmount != 1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments >= largestLoopID)  {
	  prevLargestLoopID = largestLoopID;
	  largestLoopID = *arguments;
	  for (i3 = prevLargestLoopID - 1; i3 != largestLoopID - 1; i3++)  {
	    *(allocedLoops + i3) = 0;
	  }
	  loopLocations = realloc(loopLocations, sizeof(long) * (largestLoopID + 1));
	  loopAmounts = realloc(loopAmounts, sizeof(int) * (largestLoopID + 1));
	}
	if (calledLoop == 0)  {
	  if (*(allocedLoops + *arguments) == 1)  {
	    endwin();
	    printf("ERR 12: ID ALREADY IN USE\n");
	    return -1;
	  }
	  *(allocedLoops + *arguments) = 1;
	  *(loopLocations + *arguments) = lastComm;
	  *(loopAmounts + *arguments) = *(arguments + 1);
	}
	else  {
	  if (*(loopAmounts + *arguments) != -1)  {
	    --*(loopAmounts + *arguments);
	  }
	}
	calledLoop = 0;
      }
      else if (currCommand == 606)  { // LOTO
	if (argAmount != 0)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*(allocedLoops + *arguments) != 1 || *arguments > largestLoopID)  {
	  endwin();
	  printf("ERR 16: CALLING NON-EXISTENT LOOP\n");
	  return -1;
	}
	if (*(loopAmounts + *arguments) != 1)  {
	  fseek(fp, *(loopLocations + *arguments), SEEK_SET);
	  readAmount = *(loopLocations + *arguments);
	  calledLoop = 1;
	}
      }
      else if (currCommand == 607)  { // COND
	if (argAmount == -1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	i2 = 1;
	for (i = 0; i != argAmount + 1; i++)  {
	  i2 *= *(arguments + i);
	}
	if (i2 == 0) {
	  i = 1;
	  inCond = 1;
	}
      }
      else if (currCommand == 610)  { // EXIT
	if (argAmount != -1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	printw("Press any key to continue...");
	refresh();
	getch();
	endwin();
	return 0;
      }
      else if (currCommand == 611)  { // SETR
	if (argAmount != 1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*(allocedRegisters + *arguments) == 0)  {
	  endwin();
	  printf("ERR 26: ATTEMPTING TO SET VALUE OF NON-EXISTANT REGISTER\n");
	  return -1;
	}
	if (*(arguments + 1) < 0)  {
	  *(arguments + 1) == 0;
	}
	*(registers + *arguments) = *(arguments + 1);
      }
      else if (currCommand == 612)  { // CLRS
	if (argAmount != -1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	clear();
      }
    }
    else  {
      if (currCommand == 607)  {
	if (argAmount == -1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	i++;
      }
      if (currCommand == 608)  {
	if (argAmount != -1)  {
	  endwin();
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	i--;
	if (i == 0)  {
	  inCond = 0;
	}
      }
    }
  }
}
