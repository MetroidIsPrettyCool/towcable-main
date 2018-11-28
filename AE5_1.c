#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int pastAmount = 100; // The maximum amount of time you can go back with PAST

/*
  Notes:
________________________________________________________________________________________________________________________________________________________________________________________________________  
  Token values:
     Nothing       : 0
     Numbers       : -1 * (value + 1). I.E. 0 = -1, 1 = -2, 12 = -13, 117 = -118, etc.
     Symbols       : @ = 200 + = 201 - = 202 & = 100
     Numerical Ops : RAND = 300 ADDN = 301 SUBN = 302 MULN = 303 DIV = 304 MODN = 305 PAST = 306
     IFs           : EQ = 400 NQ = 401 GT = 402 LT = 403 GE = 404 LE = 405
  
     Special       : 10 is the beginning of the array, and is ignored. 0 is a blank value.
  
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
  if (tokenAmount == 1 && *(tokens + 1) == 1738) {
    printf("This is a dead meme\n");
    return 609;
  }
  if (tokenAmount == 1 && *(tokens + 1) >= 400 && *(tokens + 1) <= 405)  {
    return (*(tokens + 1) - 389) * -1;
  }
  for (;;)  {
    for (i = tokenAmount; *(tokens + i) <= 0; i--);
    currMultiOp = *(tokens + i);
    currLoc = i;
    if (currMultiOp != 10)  {
      for (i2 = 0; i2 != ((currMultiOp == 306) ? dimensions + 1 : ((currMultiOp > 300) ? 2 : dimensions)); i2++)  {
	for (i = currLoc; *(tokens + i) >= 0; i++);
	*(valS + i2) = (*(tokens + i) * -1) - 1;
	*(tokens + i) = 0;
      }
      if (currMultiOp == 200)  {
	for (i2 = 0; i2 != dimensions; i2++)  {
	  while (*(valS + i2) >= *(cellSizes + i2))  {
	    *(valS + i2) -= *(cellSizes + i2);
	  }
	}
	i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  i4 = 1;
	  for (i5 = 0; i5 <= i2 - 1; i5++)  {
	    i4 *= *(cellSizes + i5);
	  }
	  i3 += *(valS + i2) * i4;
	}
	*(tokens + currLoc) = (*(cell + i3) + 1) * -1;
	
      }
      if (currMultiOp == 201)  {
	
        for (i2 = 0; i2 != dimensions; i2++)  {
	  while (*(valS + i2) + *(cellPointerCoors + i2) >= *(cellSizes + i2))  {
	    *(valS + i2) -= *(cellSizes + i2);
	  }
	}
	i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  i4 = 1;
	  for (i5 = 0; i5 <= i2 - 1; i5++)  {
	    i4 *= *(cellSizes + i5);
	  }
	  i3 += (*(valS + i2) + *(cellPointerCoors + i2)) * i4;
	}
	*(tokens + currLoc) = (*(cell + i3) + 1) * -1;
      }
      if (currMultiOp == 202)  {
        for (i2 = 0; i2 != dimensions; i2++)  {
	  while (*(cellPointerCoors + i2) - *(valS + i2) <= 0)  {
	    *(valS + i2) += *(cellSizes + i2);
	  }
	}
	i3 = 0;
	for (i2 = 0; i2 != dimensions; i2++)  {
	  i4 = 1;
	  for (i5 = 0; i5 <= i2 - 1; i5++)  {
	    i4 *= *(cellSizes + i5);
	  }
	  i3 += (*(cellPointerCoors + i2) - *(valS + i2)) * i4;
	}
	*(tokens + currLoc) = (*(cell + i3) + 1) * -1;
      }
      if (currMultiOp == 300)  {
 	*(tokens + currLoc) = (((rand() % (*(valS + 1))) + *valS) + 1) * -1;
      }
      if (currMultiOp == 301)  {
 	*(tokens + currLoc) = ((*valS + *(valS + 1)) + 1)  * -1;
      }
      if (currMultiOp == 302)  {
 	*(tokens + currLoc) = ((*valS - *(valS + 1)) + 1) * -1;
 	if (*(tokens + currLoc) < 0)  {
 	  *(tokens + currLoc) = -1;
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
	i3 += *(valS + dimensions) * i4 * pastAmount;
	*(tokens + currLoc) = (*(pastCell + i3) + 1) * -1;
      }
    }
    else {
      return (*(tokens + 1) * -1) -1;
    }
  }
}

int main (void)  {
  srand(time(0)); // Seed the Random Number Generator with the current time

  int D = 1; // Dimensions (infinite)

  int i; // General Purpose Pointer
  int i2; // General Purpose Pointer
  int i3; // General Purpose Pointer
  
  // Variables
  int tokenAmount = 1; //Amount of tokens in the token array
  int *tokens = malloc(sizeof(int) * (tokenAmount + 1)); // Pointer to the list of tokens
  *tokens = 10;
  *(tokens + 1) = 405;

  // int *tokens, int tokenAmount, int *cell, int *cellSizes, int dimensions, int *cellPointerCoors, int *pastCell
  int *cellSizes = malloc(sizeof(int) * D); // The number of numbers used to describe the length / area / volume etc.
  *cellSizes = 1;

  for (i = 0; i != D; i++)  { // Figure out how big the cell array needs to be
    i2 *= *(cellSizes + i);
  }

  int *cell = malloc(sizeof(int) * i2); // Pointer to the cells

  *cell = 1;

  int *cellPointerCoors = malloc(sizeof(int) * D); // Stores the cell pointer coordinates

  *cellPointerCoors = 1;

  int *pastCell = calloc(i2 * pastAmount, sizeof(int));

  *(pastCell + 0) = 2;
  *(pastCell + 1) = 1;

  printf("Result: %i\n", AEND(tokens, tokenAmount, cell, cellSizes, D, cellPointerCoors, pastCell));
}
