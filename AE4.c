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
  
     Special       : 10 is the beginning of the array, and is ignored. 0 is a blank value.
  
  Miscellanea:

     The old base 64 single-digit system has been overturned in favor of a base ten system. All whole numbers are valid (floating point and negative numbers may come in a revision of the languge)
     The reason no 1xx tokens are used is they originally referred to base 64 numbers, and I'm lazy and haven't changed the token scheme. (And probably won't) (& was added after this was typed)
     PAST support is coming, probably in AE4
     PAST in 2D will require a bit of special code, as it will use 3 values (time, x coordinate, y coordinate)
     TSNI will use the tokens values desribed above, because I'm lazy :P (I'm not sure why this was listed under "Things to come in AE4")

  Things to come in AE4:

      Integration of PAST
  
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

int AE2D (int *tokens, int tokenAmount, int *cell, int cellXSize, int cellYSize, int currCellX, int currCellY, int *pastCell)  {
  // AE2D considers UniOps to be MultiOps (as they require two arguments in 2D mode), and thus they are referred to as such 
  
  // Setup variables
  int i; // Multi-purpose counter
  int i2; // Multi-purpose counter
  int currMultiOp; // Stores the current multiOp being executed
  int currLoc; // Stores the location of the current MultiOp
  int valA; // Stores the values the MultiOp is acting on.
  int valB; // Stores the values the MultiOp is acting on.
  int valC; // Used only by PAST, stores the third argument

  if (tokenAmount == 1 && *(tokens + 1) == 100) { // If all we get is &
    return -1;
  }

  if (tokenAmount == 1 && *(tokens + 1) == 1738) { // Fetty Wap Operator
    printf("This is a dead meme\n");
    return 609;
  }
  
  for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 200) && *(tokens + i) != 10; i--); // Finds the last MultiOp 
  for (;;)  { 
    currMultiOp = *(tokens + i); // Sets the current MultiOp to the right token value 
    currLoc = i; // Sets the location of the last MultiOp 
    if (currMultiOp != 10)  { // If there is a MultiOp left in the argument 
      for (i = currLoc; *(tokens + i) >= 0; i++); // Find the first argument 
      valA = (*(tokens + i) * -1) - 1; // Sets the first value of the MultiOp 
      *(tokens + i) = 0; // Sets the value pointed at to 0 
      
      for (i = currLoc; *(tokens + i) >= 0; i++); // Find the second argument
      valB = (*(tokens + i) * -1) - 1; // Sets the first value of the MultiOp
      *(tokens + i) = 0; // Sets the value pointed at to 0

      if (currMultiOp == 200)  { // If the MultiOp is @
	while (valA >= cellXSize)  { // If valA would overflow
	  valA -= cellXSize; // Decrease it
	}
	while (valB >= cellYSize)  { // If valB would overflow
	  valB -= cellYSize; // Decrease it
	}
	*(tokens + currLoc) = *(cell + (cellXSize * valB) + valA);
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
 	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 201)  { // If the MultiOp is +
	while (valA + currCellX >= cellXSize)  { // If valA would overflow
	  valA -= cellXSize; // Decrease it
	}
	while (valB + currCellY >= cellYSize)  { // If valB would overflow
	  valB -= cellYSize; // Decrease it
	}
	*(tokens + currLoc) = *(cell + ((cellXSize * valB) + currCellY) + (valA + currCellX));
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
 	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 202)  { // If the MultiOp is -
	while (valA - currCellX >= cellXSize)  { // If valA would overflow
	  valA += cellXSize; // Increase it
	}
	while (valB - currCellY >= cellYSize)  { // If valB would overflow
	  valB += cellYSize; // Increase it
	}
	*(tokens + currLoc) = *(cell + ((cellXSize * valB) - currCellY) + (valA - currCellY));
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
 	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
      
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
	for (i = currLoc; *(tokens + i) >= 0; i++); // Find the third argument
	valC = (*(tokens + i) * -1) - 1; // Sets the third value of the MultiOp
	*(tokens + i) = 0; // Sets the value pointed at to 0
	
	*(tokens + currLoc) = *(pastCell + valA + (valB * cellXSize) + (valC * cellXSize * cellYSize));
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
 	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
    }
    
    else { // If no MultiOp remains in the argument
      for (i = tokenAmount; *(tokens + i) >= 0; i--);  // Find the last constant value
      valA = (*(tokens + i) * -1) - 1; // Set valA to that value
      return valA;
    }
  }
}

int AE1D (int *tokens, int tokenAmount, int *cell, int cellSize, int currCell, int *pastCell)  {
  // Setup variables
  int i; // Multi-purpose counter
  int i2; // Multi-purpose counter
  int currMultiOp; // Stores the current multiOp being executed
  int currLoc; // Stores the location of the current MultiOp
  int valA; // Stores the values the MultiOp is acting on.
  int valB; // Stores the values the MultiOp is acting on.

  if (tokenAmount == 1 && *(tokens + 1) == 100) { // If all we get is &
    return -1;
  }

  if (tokenAmount == 1 && *(tokens + 1) == 1738) { // Fetty Wap Operator
    printf("This is a dead meme\n");
    return 2488;
  }
  
  for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp 
  for (;;)  { 
    currMultiOp = *(tokens + i); // Sets the current MultiOp to the right token value 
    currLoc = i; // Sets the location of the last MultiOp 
    if (currMultiOp != 10)  { // If there is a MultiOp left in the argument 
      for (i = currLoc; *(tokens + i) >= 0; i++); // Find the first argument 
      valA = (*(tokens + i) * -1) - 1; // Sets the first value of the MultiOp 
      *(tokens + i) = 0; // Sets the value pointed at to 0 
      for (i2 = i - 1; *(tokens + i2) <= 202 && *(tokens + i2) >= 200; i2++) { // Loop to find all UniOps applying to the current value 
 	if (*(tokens + i2) == 200)  { // If the token is @ 
 	  while (valA > cellSize)  { // If valA overflows 
 	    valA -= cellSize; // Decrease valA 
 	  } 
 	  valA = *(cell + valA); // Set the value to the value at the absolute location pointed at. 
 	} 
 	if (*(tokens + i2) == 201)  { // If the token is + 
 	  while ((currCell + valA) > cellSize)  { // If valA overflows 
 	    valA -= cellSize; // Decrease valA 
 	  }
 	  valA = *(cell + valA + currCell); // Set the value to the value x to the right
 	}
 	if (*(tokens + i2) == 202)  { // If the token is -
 	  while ((currCell - valA) < 0)  { // If valA underflows
 	    valA += cellSize; // Increase valA
 	  }
 	  valA = *(cell + currCell - valA); // Set the value to the value x to the left
 	}
 	*(tokens + i2) = 0; // Sets the value pointed at to 0
      }
      
      for (i = currLoc; *(tokens + i) >= 0; i++); // Find the second argument
      valB = (*(tokens + i) * -1) - 1; // Sets the first value of the MultiOp
      *(tokens + i) = 0; // Sets the value pointed at to 0
      for (i2 = i - 1; *(tokens + i2) <= 202 && *(tokens + i2) >= 200; i2++) { // Loop to find all UniOps applying to the current value
 	if (*(tokens + i2) == 200)  { // If the token is @
 	  while (valB > cellSize)  { // If valB overflows
 	    valB -= cellSize; // Decrease valB
 	  }
 	  valB = *(cell + valB); // Set the value to the value at the absolute location pointed at.
 	}
 	if (*(tokens + i2) == 201)  { // If the token is +
 	  while ((valB + currCell) > cellSize)  { // If valB overflows
 	    valB -= cellSize; // Decrease valB
 	  }
 	  valB = *(cell + valB + currCell); // Set the value to the value x to the right
 	}
 	if (*(tokens + i2) == 202)  { // If the token is -
 	  while ((currCell - valB) < 0)  { // If valB underflows
 	    valB += cellSize; // Increase valB
 	  }
 	  valB = *(cell + currCell - valB); // Set the value to the value x to the left
 	}
 	*(tokens + i2) = 0; // Sets the value pointed at to 0
      }
      
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
	*(tokens + currLoc) = *(pastCell + valA + (valB * cellSize));
	*(tokens + currLoc) = (*(tokens + currLoc) + 1) * -1;
 	for (i = tokenAmount; (*(tokens + i) > 306 || *(tokens + i) < 300) && *(tokens + i) != 10; i--); // Finds the last MultiOp
      }
    }
    
    else { // If no MultiOp remains in the argument
      for (i = tokenAmount; *(tokens + i) >= 0; i--);  // Find the last constant value
      valA = (*(tokens + i) * -1) - 1; // Set valA to that value
      while (*(tokens + i) != 10)  { // While we haven't hit the end of the token array
 	if (*(tokens + i) == 200)  { // If the token is @
 	  while (valA > cellSize)  { // If valA overflows
 	    valA -= cellSize; // Decrease valA
 	  }
 	  valA = *(cell + valA); // Set the value to the value at the absolute location pointed at.
 	} 
 	if (*(tokens + i) == 201)  { // If the token is +
 	  while (valA + currCell > cellSize)  { // If valA overflows
 	    valA -= cellSize; // Increase valA
 	  }
 	  valA = *(cell + valA + currCell); // Set the value to the value x to the right
 	}
 	if (*(tokens + i) == 202)  { // If the token is -
 	  while (currCell - valA < 0)  { // If valA overflows
 	    valA -= cellSize; // Increase valA
 	  }
 	  valA = *(cell + currCell - valA); // Set the value to the value x to the left
 	}
 	i--;
      }
      return valA;
    }
  }
}

int main (void)  {
  srand(time(0)); // Seed the Random Number Generator with the current time

  int D = 2; // Dimensions (1 or 2)

  int i; // General Purpose Pointer
  int i2; // General Purpose Pointer
  int i3; // General Purpose Pointer
  
  // Variables
  int tokenAmount = 1; //Amount of tokens in the token array
  int *tokens = malloc(sizeof(int) * (tokenAmount + 1)); // Pointer to the list of tokens
  *tokens = 10;
  *(tokens + 1) = 1738;
  if (D == 1)  {
    int cellSize = 12; // Amount of cells in the array
    int *cell = malloc(sizeof(int) * cellSize); // Pointer to the cells (1D)
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
    int currCell = 3; // Cell being operated upon
    int *pastCell = malloc(sizeof(int) * cellSize * pastAmount);
    for (i = 0; i != cellSize; i++)  { // Fill in pastCell
      for (i2 = 0; i2 != pastAmount; i2++)  {
	*(pastCell + i + (i2 * cellSize)) = 0;
      }
    }
    printf("Result: %i\n", AE1D(tokens, tokenAmount, cell, cellSize, currCell, pastCell));
  }
  else if (D == 2)  {
    int cellXSize = 3; // Array X size
    int cellYSize = 3; // Array Y size
    int *cell = malloc(sizeof(int) * (cellXSize * cellYSize)); // Pointer to the cells (2D)
    *cell = 1;
    *(cell + 1) = 2;
    *(cell + 2) = 3;
    *(cell + 3) = 7;
    *(cell + 4) = 8;
    *(cell + 5) = 9;
    *(cell + 6) = 10;
    *(cell + 7) = 11;
    *(cell + 8) = 6;
    int currCellX = 1; // Operating cell x coordinate
    int currCellY = 2; // Operating cell y coordinate
    int *pastCell = malloc(sizeof(int) * cellXSize * cellYSize * pastAmount);
    for (i = 0; i != cellXSize; i++)  { // Fill in pastCell
      for (i2 = 0; i2 != cellYSize; i2++)  {
	for (i3 = 0; i3 != pastAmount; i3++)  {
	  *(pastCell + i + (i2 * cellXSize) + (i3 * cellXSize * cellYSize)) = 0;
	}
      }
    }
    printf("Result: %i\n", AE2D(tokens, tokenAmount, cell, cellXSize, cellYSize, currCellX, currCellY, pastCell));
  }
}
