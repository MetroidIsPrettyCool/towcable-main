#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
  Notes:
________________________________________________________________________________________________________________________________________________________________________________________________________  

  Token values:

     Nothing       : 0
     Numbers       : -1 * (value + 1). I.E. 0 = -1, 1 = -2, 12 = -13, 117 = -118, etc.
     Symbols       : @ = 200 + = 201 - = 202 
     Numerical Ops : RAND = 300 ADDN = 301 SUBN = 302 MULN = 303 DIV = 304 MODN = 305 PAST = 306
  
  Special       : 10 is the beginning of the array, and is ignored. 0 is a blank value.
  
  Miscellanea:

     The old base 64 single-digit sy7stem has been overturned in favor of a base ten system. All whole numbers are valid (floating point and negative numbers may come in a revision of the languge)
     The reason no 1xx tokens are used is they originally referred to base 64 numbers, and I'm lazy and haven't changed the token scheme. (And probably won't)
     PAST support is coming, probably in AE4
     PAST in 2D will require a bit of special code, as it will use 3 values (time, x coordinate, y coordinate)

  Things to come in AE4:

      Support for 2-d programs
      Integration of PAST
      Support for &
      All of the major code will be moved into two separate functions, AE1D and AE2D, in preparation for integration into TSNI
      TSNI will use the tokens values desribed above, because I'm lazy :P
      Maybe a joke operator, because this crap is boring sometimes
  
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

int main (void)  {
  srand(time(0)); // Seed the random Number Generator with the current itme
  
  // Variables
  int tokenAmount = 14; //Amount of tokens in the token array.
  int *tokens = malloc(sizeof(int) * (tokenAmount + 1)); // Pointer to the list of tokens
  *tokens = 10;
  *(tokens + 1) = 200;
  *(tokens + 2) = 201;
  *(tokens + 3) = 301;
  *(tokens + 4) = 303;
  *(tokens + 5) = -7;
  *(tokens + 6) = 200;
  *(tokens + 7) = -36;
  *(tokens + 8) = 302;
  *(tokens + 9) = 305;
  *(tokens + 10) = -7;
  *(tokens + 11) = 201;
  *(tokens + 12) = -5;
  *(tokens + 13) = 200;
  *(tokens + 14) = -4;
  int cellSize = 12; // Amount of cells in the array.
  int *cell = malloc(sizeof(int) * (cellSize + 1)); // Pointer to the cells (1d)
  *cell = 1;
  *(cell + 1) = 7;
  *(cell + 2) = 8;
  *(cell + 3) = 9;
  *(cell + 4) = 10;
  *(cell + 5) = 31;
  *(cell + 6) = 6;
  *(cell + 7) = 21;
  *(cell + 8) = 43;
  *(cell + 9) = 51;
  *(cell + 10) = 7;
  *(cell + 11) = 13;
  int currCell = 3; // Cell being operated upon.
  int i;
  int i2; // Various multi-purpose counters
  int currMultiOp; // Stores the current multiOp being executed
  int currLoc; // Stores the location of the current MultiOp
  int valA;
  int valB; // Store the values the MultiOp is acting on.
  int exitNow = 0; // If this is one, stop parsing the input

  
  for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
  while (exitNow != 1)  {
    currMultiOp = *(tokens + i); // Sets the current MultiOp to the right token value
    currLoc = i; // Sets the location of the last MultiOp
    if (currMultiOp != 10)  { // If there is a MultiOp left in the argument
      for (i = currLoc; *(tokens + i) >= 0; i++); // Find the first argument
      valA = (*(tokens + i) * -1) - 1; // Sets the first value of the MultiOp
      printf("AF%i\n", valA);
      *(tokens + i) = 0; // Sets the value pointed at to 0
      for (i2 = i - 1; *(tokens + i2) <= 202 && *(tokens + i2) >= 200; i2++) { // Loop to find all UniOps applying to the current value
	if (*(tokens + i2) == 200)  { // If the token is @
	  while (valA > cellSize)  { // If i2 overflows
	    valA -= cellSize; // Move i2 back
	  }
	  valA = *(cell + valA); // Set the value to the value at the absolute location pointed at.
	}
	if (*(tokens + i2) == 201)  { // If the token is +
	  while ((currCell + valA) > cellSize)  { // If i2 overflows
	    valA -= cellSize; // Move i2 back
	  }
	  valA = *(cell + valA + currCell); // Set the value to the value x to the right
	}
	if (*(tokens + i2) == 202)  { // If the token is -
	  while ((currCell - valA) < 0)  { // If i2 underflows
	    valA += cellSize; // Move i2 forwards
	  }
	  valA = *(cell + currCell - valA); // Set the value to the value x to the left
	}
	*(tokens + i2) = 0; // Sets the value pointed at to 0
      }
      printf("VA%i\n", valA);
            
      for (i = currLoc; *(tokens + i) >= 0; i++); // Find the second argument
      valB = (*(tokens + i) * -1) - 1; // Sets the first value of the MultiOp
      printf("BF%i\n", valB);
      *(tokens + i) = 0; // Sets the value pointed at to 0
      for (i2 = i - 1; *(tokens + i2) <= 202 && *(tokens + i2) >= 200; i2++) { // Loop to find all UniOps applying to the current value
	if (*(tokens + i2) == 200)  { // If the token is @
	  while (valB > cellSize)  { // If i2 overflows
	    valB -= cellSize; // Move i2 back
	  }
	  valB = *(cell + valB); // Set the value to the value at the absolute location pointed at.
	}
	if (*(tokens + i2) == 201)  { // If the token is +
	  while ((valB + currCell) > cellSize)  { // If i2 overflows
	    valB -= cellSize; // Move i2 back
	  }
	  valB = *(cell + valB + currCell); // Set the value to the value x to the right
	}
	if (*(tokens + i2) == 202)  { // If the token is -
	  while ((currCell - valB) < 0)  { // If i2 underflows
	    valB += cellSize; // Move i2 forwards
	  }
	  valB = *(cell + currCell - valB); // Set the value to the value x to the left
	}
	*(tokens + i2) = 0; // Sets the value pointed at to 0
      }
      printf("VB%i\n", valB);
      
      if (currMultiOp == 300)  { // If the MultiOp is RAND
	*(tokens + currLoc) = (rand() % (valB)) + valA;
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 301)  { // If the MultiOp is ADDN
	*(tokens + currLoc) = valA + valB;
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 302)  { // If the MultiOp is SUBN
	*(tokens + currLoc) = valA - valB;
	if (*(tokens + currLoc) < 0)  {
	  *(tokens + currLoc) = 0;
	}
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 303)  { // If the MultiOp is MULN
	*(tokens + currLoc) = valA * valB;
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 304)  { // If the MultiOp is DIVN
	*(tokens + currLoc) = valA / valB;
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 305)  { // If the MultiOp is MODN
	*(tokens + currLoc) = valA % valB;
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 306)  { // If the MultiOp is PAST (Will be implemented later)
      
      }
      printf("RS%i\n", *(tokens + currLoc) * -1 - 1);
    }
        
    else { // If no MultiOp remains in the argument
      for (i = tokenAmount; *(tokens + i) >= 0; i--);  // Find the last constant value
      valA = (*(tokens + i) * -1) - 1; // Set valA to that value
      while (*(tokens + i) != 10)  { // While we haven't hit the end of the token array
	if (*(tokens + i) == 200)  { // If the token is @
	  while (valA > cellSize)  { // If val overflows
	    valA -= cellSize; // Move val back
	  }
	  valA = *(cell + valA); // Set the value to the value at the absolute location pointed at.
	}
	if (*(tokens + i) == 201)  { // If the token is +
	  while (valA + currCell > cellSize)  { // If val overflows
	    valA -= cellSize; // Move val back
	  }
	  valA = *(cell + valA + currCell); // Set the value to the value x to the right
	}
	if (*(tokens + i) == 202)  { // If the token is -
	  while (currCell - valA < 0)  { // If val overflows
	    valA -= cellSize; // Move val back
	  }
	  valA = *(cell + currCell - valA); // Set the value to the value x to the left
	}
	i--;
      }
      exitNow = 1;
      printf("%i\n", valA);
    }
  }
}
