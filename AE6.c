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
     Numerical Ops : RAND = 300 ADDN = 301 SUBN = 302 MULN = 303 DIV = 304 MODN = 305 PAST = 306 LOCN = 307
     IFs           : EQ = 400 NQ = 401 GT = 402 GE = 403 LT = 404 LE = 405
  
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
  for (;;)  {
    for (i = tokenAmount; *(tokens + i) <= 300 && *(tokens + i) != 10; i--) {
      printf("t\n");
      fflush(NULL);
    }
    currMultiOp = *(tokens + i);
    printf("qwqw%i  %i\n", currMultiOp, *(tokens + i));
    fflush(NULL);
    currLoc = i;
    if (currMultiOp != 10)  {
      for (i2 = 0; i2 != ((currMultiOp == 306) ? dimensions + 1 : ((currMultiOp == 307) ? (2 * dimensions) : 2)); i2++)  {
	for (i = currLoc; *(tokens + i) >= 300 || *(tokens + i) == 0; i++);
	printf("%i 12rewqasd;'\n", *(tokens + i));
        if (*(tokens + i) < 0)  {
	  *(valS + i2) = (*(tokens + i) * -1) - 1;
	}
	else {
	  *(valS + i2) = *(tokens + i);
	}
	*(tokens + i) = 0;
      }
      printf("qwqw%i\n", currMultiOp);
      fflush(NULL);
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
      if (currMultiOp == 307)  { // LOCN
	i3 = 0;
	printf("g\n");
	fflush(NULL);
	printf("%i %i gg234234\n", *valS, *(valS + 1));
	for (i2 = 0; i2 != dimensions; i2++)  {
	  if (*(valS + (2 * i2)) == 200) {
	    printf("wqwhiojk\n");
	    while (*(valS + (2 * i2) + 1) > *(cellSizes + i2))  {
	      printf("r\n");
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  {
	      i4 *= *(cellSizes + i5);
	    }
	    printf("%i rtqw\n", i4);
	    i3 += *(valS + (2 * i2 + 1)) * i4;
	    printf("%i 8901243\n", i3);
	  }
	  if (*(valS + (2 * i2)) == 201) {
	    printf("wqwhiojk\n");
	    while (*(cellPointerCoors + i2) + *(valS + (2 * i2) + 1) > *(cellSizes + i2))  {
	      printf("r\n");
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  {
	      i4 *= *(cellSizes + i5);
	    }
	    printf("%i rtqw\n", i4);
	    i3 += (*(cellPointerCoors + i2) + *(valS + (2 * i2 + 1))) * i4;
	    printf("%i 8901243\n", i3);
	  }
	  if (*(valS + (2 * i2)) == 202) {
	    printf("wqwhiojk\n");
	    while (*(cellPointerCoors + i2) - *(valS + (2 * i2) + 1) > *(cellSizes + i2))  {
	      printf("r\n");
	      *(valS + (2 * i2) + 1) -= *(cellSizes + i2);
	    }
	    i4 = 1;
	    for (i5 = 0; i5 < i2; i5++)  {
	      i4 *= *(cellSizes + i5);
	    }
	    printf("%i rtqw\n", i4);
	    i3 += (*(cellPointerCoors + i2) - *(valS + (2 * i2 + 1))) * i4;
	    printf("%i 8901243\n", i3);
	  }
	}
	printf("yart %i\n", *(cell + i3));
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

int main (void)  {
  srand(time(0)); // Seed the Random Number Generator with the current time

  int D = 2; // Dimensions (infinite)

  int i; // General Purpose Pointer
  int i2; // General Purpose Pointer
  int i3; // General Purpose Pointer
  
  // Variables
  int tokenAmount = 5; //Amount of tokens in the token array
  int *tokens = malloc(sizeof(int) * (tokenAmount + 1)); // Pointer to the list of tokens
  *tokens = 10;
  *(tokens + 1) = 307;
  *(tokens + 2) = 202;
  *(tokens + 3) = -2;
  *(tokens + 4) = 201;
  *(tokens + 5) = -2;

  // int *tokens, int tokenAmount, int *cell, int *cellSizes, int dimensions, int *cellPointerCoors, int *pastCell
  int *cellSizes = malloc(sizeof(int) * D); // The number of numbers used to describe the length / area / volume etc.
  *cellSizes = 2;
  *(cellSizes + 1) = 2;
  
  for (i = 0; i != D; i++)  { // Figure out how big the cell array needs to be
    i2 *= *(cellSizes + i);
  }

  int *cell = malloc(sizeof(int) * i2); // Pointer to the cells

  *cell = 1;
  *(cell + 1) = 2;
  *(cell + 2) = 3;
  *(cell + 3) = 4;

  int *cellPointerCoors = malloc(sizeof(int) * D); // Stores the cell pointer coordinates

  *cellPointerCoors = 1;
  *(cellPointerCoors  + 1) = 0;
  
  int *pastCell = calloc(i2 * pastAmount, sizeof(int));
  printf("g\n");
  fflush(NULL);

  printf("Result: %i\n", AEND(tokens, tokenAmount, cell, cellSizes, D, cellPointerCoors, pastCell));
}
