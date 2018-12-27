#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

const int pastAmount = 100; // The maximum amount of time you can go back with PAST

/*
  Notes:
________________________________________________________________________________________________________________________________________________________________________________________________________  
  Token values:
     Nothing       : 0
     Numbers       : -1 * (value + 1). I.E. 0 = -1, 1 = -2, 12 = -13, 117 = -118, etc.
     Symbols       : @ = 200 + = 201 - = 202 & = 100 . = 101 , = 102
     Numerical Ops : RAND = 300 ADDN = 301 SUBN = 302 MULN = 303 DIV = 304 MODN = 305 PAST = 306 LOCN = 307
     IFs           : EQ = 400 NQ = 401 GT = 402 GE = 403 LT = 404 LE = 405
  
     Special       : 10 is the beginning of the array, and is ignored. 0 is a blank value. 7 is whitespace.
  
  Miscellanea:
     The old base 64 single-digit system has been overturned in favor of a base ten system. All whole numbers are valid (floating point and negative numbers may come in a revision of the languge)
     The reason no 1xx tokens are used is they originally referred to base 64 numbers, and I'm lazy and haven't changed the token scheme. (And probably won't) (& was added after this was typed)
     PAST support is coming, probably in AE4
     PAST in 2D will require a bit of special code, as it will use 3 values (time, x coordinate, y coordinate)
     TSNI will use the tokens values desribed above, because I'm lazy :P (I'm not sure why this was listed under "Things to come in AE4")
  AE5 new features:
     
     Infinite dimensions!
     Support for EQ, NQ, GT, LT, GE and LE
     Removal of comments (sorry future me)
  
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

int AEND (int *tokens, int tokenAmount, int *cell, int *cellSizes, int dimensions, int *cellPointerCoors, int *pastCell)  {
  int i;
  int i2;
  int i3;
  int i4;
  int i5;
  int currMultiOp;
  int currLoc;
  int *valS = malloc(sizeof(int) * (dimensions + 1));
  if (tokenAmount == 1 && *(tokens + 1) == 100) {
    return -1;
  }
  if (tokenAmount == 1 && *(tokens + 1) == 102) {
    return -2;
  }
  if (tokenAmount == 1 && *(tokens + 1) == 1738) {
    printf("This is a dead meme\n");
    return 609;
  }
  for (;;)  {
    for (i = tokenAmount; *(tokens + i) < 300 && *(tokens + i) != 10; i--) {
    }
    currMultiOp = *(tokens + i);
    currLoc = i;
    if (currMultiOp != 10)  {
      for (i2 = 0; i2 != ((currMultiOp == 306) ? dimensions + 1 : ((currMultiOp == 307) ? (2 * dimensions) : 2)); i2++)  {
	for (i = currLoc; *(tokens + i) >= 300 || *(tokens + i) == 0; i++);
        if (*(tokens + i) < 0)  {
	  *(valS + i2) = (*(tokens + i) * -1) - 1;
	}
	else {
	  *(valS + i2) = *(tokens + i);
	}
	*(tokens + i) = 0;
      }
      if (currMultiOp == 300)  {
 	*(tokens + currLoc) = (((rand() % (*(valS + 1))) + *valS) + 1) * -1;
      }
      if (currMultiOp == 301)  {
 	*(tokens + currLoc) = ((*valS + *(valS + 1)) + 1)  * -1;
      }
      if (currMultiOp == 302)  {
 	*(tokens + currLoc) = ((*valS - *(valS + 1)) + 1) * -1;
 	if (*(tokens + currLoc) > 0)  {
 	  *(tokens + currLoc) = 0;
 	}
      }
      if (currMultiOp == 303)  {
 	*(tokens + currLoc) = ((*valS * *(valS + 1)) + 1) * -1;
      }
      if (currMultiOp == 304)  {
 	*(tokens + currLoc) = ((*valS / *(valS + 1)) + 1) * -1;
      }
      if (currMultiOp == 305)  {
 	*(tokens + currLoc) = ((*valS % *(valS + 1)) + 1) * -1;
      }
      if (currMultiOp == 306)  {
	i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  i4 = 1;
	  for (i5 = 0; i5 <= i2 - 1; i5++)  {
	    i4 *= *(cellSizes + i5);
	  }
	  i3 += *(valS + i2) * i4;
	}
	i3 += *(valS + dimensions) * i4;
	*(tokens + currLoc) = (*(pastCell + i3) + 1) * -1;
      }
      if (currMultiOp == 307)  {
	i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  if (*(valS + (2 * i2)) == 200) {
	    while (*(valS + (2 * i2) + 1) > *(cellSizes + i2))  {
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  {
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += *(valS + (2 * i2 + 1)) * i4;
	  }
	  if (*(valS + (2 * i2)) == 201) {
	    while (*(cellPointerCoors + i2) + *(valS + (2 * i2) + 1) > *(cellSizes + i2))  {
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  {
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) + *(valS + (2 * i2 + 1))) * i4;
	  }
	  if (*(valS + (2 * i2)) == 202) {
	    while (*(cellPointerCoors + i2) - *(valS + (2 * i2) + 1) > *(cellSizes + i2))  {
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  {
	      i4 *= *(cellSizes + i5);
	    }
	    i3 += (*(cellPointerCoors + i2) - *(valS + (2 * i2 + 1))) * i4;
	  }
	}
	*(tokens + currLoc) = (*(cell + i3) + 1) * -1;
      }
      if (currMultiOp == 400)  {
	if (*valS == *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 401)  {
	if (*valS != *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 402)  {
	if (*valS > *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 403)  {
	if (*valS >= *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 404)  {
	if (*valS < *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
      if (currMultiOp == 405)  {
	if (*valS <= *(valS + 1))  {
	  *(tokens + currLoc) = -2;
	}
	else {
	  *(tokens + currLoc) = -1;
	}
      }
    }
    else {
      return (*(tokens + 1) * -1) -1;
    }
  }
}

int main (int argc, char *argv[])  {

  char filename [100];

  FILE *fp;

  strcpy(filename, argv[1]);
  fp = fopen(filename, "r");
  
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
	  printf("ERR 02: UNEXPECTED EOF\n");
	  return -1;
	}
	if (inChr == '!')  {
	  inChr = fgetc(fp);
	  readAmount++;
	  if (inChr == EOF)  {
	    printf("ERR 02: UNEXPECTED EOF\n");
	    return -1;
	  }
	  for (; inChr != '!'; inChr = fgetc(fp))  {
	    readAmount++;
	    if (inChr == EOF)  {
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
	printf("ERR 02: UNEXPECTED EOF\n");
	return -1;
      }

    }
    else  {
      i2 = 1;
      for (i = 0; i != D; i++)  {
	i2 *= *(cellSizes + i);
	*(cellPointerCoors + i) = *(ruleLocs + i4) % i2;
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
      else  {
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
	    printf("ERR 20: MISSING DOUBLE QUOTE\n");
	    return -1;
	  }
	  inStrPtr++;
	  while (*(inptStr + inStrPtr) != '\"')  { // Deals with escape sequences
	    if (*(inptStr + inStrPtr) == '\n')  {
	      printf("ERR 14: NEWLINE IN STRING\n");
	      return -1;
	    }
	    if (*(inptStr + inStrPtr) == '\\')  {
	      inStrPtr++;
	      if (*(inptStr + inStrPtr) == 'n')  {
		printf("\n");
	      }
	      else if (*(inptStr + inStrPtr) == 't')  {
		printf("\t");
	      }
	      else if (*(inptStr + inStrPtr) == '\"')  {
		printf("\"");
	      }
	      else if (*(inptStr + inStrPtr) == '\\')  {
		printf("\\");
	      }
	      else  {
		printf("ERR 22: ILLEGAL ESCAPE SEQUENCE\n");
		return -1;
	      }
	    }
	    else  { // Print the current character read
	      printf("%c", *(inptStr + inStrPtr));
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
	  if (*(inptStr + inStrPtr) != '.')  { // CHeck if the line ends with a period
	    printf("ERR 21: MISSING PERIOD\n");
	    return -1;
	  }
	}
	continue;
      }
      else if (*(inptStr) == 'E' && *(inptStr + 1) == 'X' && *(inptStr + 2) == 'I' && *(inptStr + 3) == 'T')  {
	currCommand = 610;
      }
      else if (*(inptStr) == 'T' && *(inptStr + 1) == 'E' && *(inptStr + 2) == 'A' && *(inptStr + 3) == 'P')  {
	printf("ERR 42: I'M A TEAPOT\n");
	return -1;
      }
      else  {
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
	  tracker += ((input == 306) ? D + 1 : ((input == 307) ? (2 * D) : 2)) - 1;
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
	      printf("ERR 19: NUMBER TERMINATED BY ILLEGAL CHARACTER '%c'\n", *(inptStr + inStrPtr));
	      return -1;
	    }
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
	      if (currCommand != 503 && currCommand != 504)  {
		printf("ERR 13: USE OF LOCN OUTSIDE OF RULE OR CMPX\n");
		return -1;
	      }
	      input = 307;
	      inStrPtr += 4;
	    }
	    else if (*(inptStr + inStrPtr) == 'r')  {
	      input = 1738;
	      inStrPtr++;
	    }
	    else {
	      printf("ERR: ILLEGAL CHARACTER '%c' AT %i\n", *(inptStr + inStrPtr), inStrPtr);
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
		printf("ERR 23: & IN ILLEGAL LOCATION\n");
		return -1;
	      }
	      input = 100;
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '.')  {
	      input = 101;
	      if (tracker != 2 && tokenAmount != 0)  {
		printf("ERR 04: ONE OR MORE ARGUMENTS TERMINATED EARLY\n");
		return -1;
	      }
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == ',')  {
	      input = 102;
	      if (currCommand != 504)  {
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
	      input = 201;
	      inStrPtr++;
	    }
	    else if (*(inptStr + inStrPtr) == '-')  {
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
	*(arguments + argAmount) = AEND(tokens, tokenAmount, cell, cellSizes, D, cellPointerCoors, pastCell);
	arguments = realloc(arguments, sizeof(int) * (argAmount + 2));
      }
    }
    // Execute commands:
    argAmount -= 2;
    if (init == 1)  {
      if (currCommand == 500 && step == 0)  { // DIMN
	if (*arguments <= 0)  {
	  printf("ERR 06: DIMENSIONS 0\n");
	  return -1;
	}
	if (argAmount != 0)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	step = 1;
	D = *arguments;
	free(cellSizes);
	cellSizes = calloc(D, sizeof(int));
      }
      else if (currCommand == 500)  { // DIMN ERROR
	printf("ERR 08: MULTIPLE CALLS TO DIMN\n");
	return -1;
      }
      else if (currCommand == 501 && step == 1)  { // SIZE
	if (argAmount != D - 1)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	for (i = 0; i != D; i++)  {
	  input = *(i + arguments);
	  if (input <= 0)  {
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
	printf("ERR 09: MULTIPLE CALLS TO SIZE\n");
	return -1;
      }
      else if (currCommand == 502 && step == 2)  { // SETP
	if (argAmount != it - 1)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	for (i = 0; i != it; i++)  {
	  *(cell + i) = *(arguments + i);
	}
	step = 3;
      }
      else if (currCommand == 502 && step > 2)  { // SIZE ERROR
	printf("ERR 10: MULTIPLE CALLS TO SETP\n");
	return -1;
      }
      else if (currCommand == 503 && (step == 2 || step == 3))  { // RULE
	if (argAmount <= 2)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments > largestRuleID)  { // If the ID of the current rule is greater than the previous largest ID
	  prevLargestRuleID = largestRuleID;
	  largestRuleID = *arguments;
	  allocedRules = realloc(allocedRules, sizeof(int) * largestRuleID);
	  for (i3 = prevLargestRuleID - 1; i3 != largestRuleID - 1; i3++)  {
	    *(allocedRules + i3) = 0;
	  }
	  ruleStore = realloc(ruleStore, sizeof(char*) * (largestRuleID + 1));
	}
	if (*(allocedRules + *arguments) == 1)  {
	  printf("ERR 12: ID ALREADY IN USE\n");
	  return -1;
	}
	*(allocedRules + *arguments) = 1;
	*(ruleStore + *arguments) = malloc(strlen(inptStr)); // Store the string
	*(ruleStore + *arguments) = strcpy(*(ruleStore + *arguments), inptStr);
      }
      else if (currCommand == 504 && (step == 2 || step == 3))  { // CMPX
	if (argAmount <= 2)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments > largestCmpxID)  { // If the ID of the current rule is greater than the previous largest ID
	  prevLargestCmpxID = largestCmpxID;
	  largestCmpxID = *arguments;
	  allocedCmpxs = realloc(allocedCmpxs, sizeof(int) * largestCmpxID);
	  for (i3 = prevLargestCmpxID - 1; i3 != largestCmpxID - 1; i3++)  {
	    *(allocedCmpxs + i3) = 0;
	  }
	  cmpxStore = realloc(cmpxStore, sizeof(char*) * (largestCmpxID + 1));
	}
	if (*(allocedCmpxs + *arguments) == 1)  {
	  printf("ERR 12: ID ALREADY IN USE\n");
	  return -1;
	}
	*(allocedCmpxs + *arguments) = 1;
	*(cmpxStore + *arguments) = malloc(strlen(inptStr)); // Store the string
	*(cmpxStore + *arguments) = strcpy(*(cmpxStore + *arguments), inptStr);
      }
      else if (currCommand == 505 && (step == 2 || step == 3))  {
	init = 0;
      }
      else if (currCommand == 504 || currCommand == 505)  {
	printf("ERR 00: PROGRAM LACKS NECCESARY SETUP COMMANDS\n");
	return -1;
      }
    }
    else if (inCond == 0)  {
      if (currCommand == 600)  { // OUPT
	if (argAmount == -1 || (argAmount != 0 && *arguments == -1))  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (argAmount == 0 && *arguments == -1)  { // INFINITE
	  i = 0;
	  for (i3 = 1; i != D; i++)  {
	    i3 *= *(cellSizes + i);
	  }
	  for (i2 = 0; i2 != i3; i2++)  {
	    printf("%d ", *(cell + i2));
	    if (i2 != 0)  {
	      if ((i2 + 1) % *cellSizes == 0)  {
		printf("\n");
	      }
	    }
	  }
	}
	else  {
	  for (i2 = 0; i2 != argAmount + 1; i2++)  {
	    if (*(i2 + arguments) >= it)  {
	      printf("ERR 15: I/O ACCESSING ILLEGAL LOCATION\n");
	      return -1;
	    }
	    printf("%d ", *(cell + *(i2 + arguments)));
	  }
	}
      }
      else if (currCommand == 601)  { // CRPT
	if (argAmount == -1 || (argAmount != 0 && *arguments == -1))  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (argAmount == 0 && *arguments == -1)  { // INFINITE
	  i = 0;
	  for (i3 = 1; i != D; i++)  {
	    i3 *= *(cellSizes + i);
	  }
	  for (i2 = 0; i2 != i3; i2++)  {
	    printf("%c ", *(cell + i2));
	    if (i2 != 0)  {
	      if ((i2 + 1) % *cellSizes == 0)  {
		printf("\n");
	      }
	    }
	  }
	}
	else  {
	  for (i2 = 0; i2 != argAmount + 1; i2++)  {
	    if (*(i2 + arguments) >= it)  {
	      printf("ERR 15: I/O ACCESSING ILLEGAL LOCATION\n");
	      return -1;
	    }
	    printf("%c ", *(cell + *(i2 + arguments)));
	  }
	}
      }
      else if (currCommand == 602)  { // INPT
	if (argAmount != 0)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*arguments >= it)  {
	  printf("ERR 15: I/O ACCESSING ILLEGAL LOCATION\n");
	  return -1;
	}
	scanf("%d", (cell + *arguments));
	while ((c = getchar()) != '\n' && c != EOF);
      }
      else if (currCommand == 603)  { // CALL
	if (argAmount == -1)  {
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
	      printf("ERR 18: RULE OR CMPX ACCESSING ILLEGAL LOCATION\n");
	      return -1;
	    }
	    *(ruleLocs + i) = *(arguments + i + 1);
	  }
	}
	else  {
	  ruleLocAmount = it;
	  ruleLocs = malloc(sizeof(int) * ruleLocAmount);
	  for (i = 0; i != ruleLocAmount; i++)  { // Copy all locations to be run upon into ruleLocs
	    *(ruleLocs + i) = i;
	  }
	}
	for (i = 0; i != it; i++)  { // Copy cell into ruleCell
	  *(ruleCell + i) = *(cell + i);
	}
	i4 = 0;
      }
      else if (currCommand == 503 && runRule == 1)  { // RULE
	if (argAmount <= 2)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	i2 = 1;
	for (i = 3; i != argAmount + 1; i++)  {
	  i2 *= *(arguments + i);
	}
	if (i2 > 1)  {
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
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*(allocedCmpxs + *arguments) != 1 || *arguments > largestCmpxID)  {
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
	      printf("ERR 18: RULE OR CMPX ACCESSING ILLEGAL LOCATION\n");
	      return -1;
	    }
	    *(ruleLocs + i) = *(arguments + i + 1);
	  }
	}
	else  {
	  ruleLocAmount = it;
	  ruleLocs = malloc(sizeof(int) * ruleLocAmount);
	  for (i = 0; i != ruleLocAmount; i++)  { // Copy all locations to be run upon into ruleLocs
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
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	for (i5 = 2; i5 != argAmount; i5 = i + 1)  {
	  i2 = 1;
	  for (i = i5 + 1; (*(arguments + i) != -2) && (i < argAmount); i++)  {
	    i2 *= *(arguments + i);
	  }
	  if (i2 > 1)  {
	    printf("ERR 11: NON-BOOLEAN RESULT FOR TEST\n");
	    return -1;
	  }
	  *(ruleCell + *(ruleLocs + i4)) = *(arguments + 1);
	  if (i2 == 1)  {
	    *(ruleCell + *(ruleLocs + i4)) = *(arguments + i5);
	    i = argAmount - 1;
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
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	if (*(allocedLoops + *arguments) != 1 || *arguments > largestLoopID)  {
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
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	return 0;
      }
    }
    else  {
      if (currCommand == 607)  {
	if (argAmount == -1)  {
	  printf("ERR 03: INVALID PARAMETER AMOUNT\n");
	  return -1;
	}
	i++;
      }
      if (currCommand == 608)  {
	if (argAmount != -1)  {
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


