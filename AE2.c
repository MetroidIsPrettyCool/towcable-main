#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
  Nothing       : 0
  Numbers       : 100 + value. I.E. 0 = 100, 1 = 101, a = 110, G = 42, etc.
  Symbols       : @ = 200 + = 201 - = 202 
  Numerical Ops : RAND = 300 ADDN = 301 SUBN = 302 MULN = 303 DIV = 304 MODN = 305 PAST = 306
  
  Special       : -10 is the beginning of the array, and is ignored. 0 is a blank value.
*/

int main (void)  {
  srand(time(0)); // Seed the random Number Generator with the current itme
  
  // Variables
  int tokens [] = {-10, 302, 301, 200, 100, 200, 101, 200, 102}; // Tokenized arguments. Eventually should be modifeid to fit variable sizes of tokens.
  int tokenAmount = 8; // Amount of tokens in the token array.
  int cell [] = {1, 7, 8, 9, 10, 31, 6, 21, 43, 51, 7, 13}; // Placeholder for the real array of cells
  int cellSize = 12; // Amount of cells in the array.
  int currCell = 3; // Cell being operated upon.
  int i;
  int i2; // Various multi-purpose counters
  int currMultiOp; // Stores the current multiOp being executed
  int currLoc; // Stores the location of the current MultiOp
  int valA;
  int valB; // Store the values the MultiOp is acting on.
  int exitNow = 0; // If this is one, stop parsing the input

  // The code below simply prints the tokens and cells in a readable format. It will be deleted later
  for (i = 1;  i != tokenAmount + 1; i++)  { // Tokens
    if (tokens [i] >= 100 && tokens [i] < 110)  { // Numbers
        printf("%i ", tokens [i] - 100);
    }
    if (tokens [i] > 109 && tokens [i] < 135)  { // Lowercase
        printf("%c ", tokens [i] - 13);
    }
    if (tokens [i] > 134 && tokens [i] < 162)  { // Uppercase
        printf("%c ", tokens [i] - 70);
    }
    if (tokens [i] == 162)  {
        printf("# ");
    }
    if (tokens [i] == 163)  {
        printf("_ ");
    }
    if (tokens [i] == 200)  {
        printf("@ ");
    }
    if (tokens [i] == 201)  {
        printf("+ ");
    }
    if (tokens [i] == 202)  {
        printf("- ");
    }
    if (tokens [i] == 300)  {
        printf("RAND ");
    }
    if (tokens [i] == 301)  {
        printf("ADDN ");
    }
    if (tokens [i] == 302)  {
        printf("SUBN ");
    }
    if (tokens [i] == 303)  {
        printf("MULN ");
    }
    if (tokens [i] == 304)  {
        printf("DIVN ");
    }
    if (tokens [i] == 305)  {
        printf("MODN ");
    }
    if (tokens [i] == 306)  {
        printf("PAST ");
    }
  }
  printf("\n");

  for (i = tokenAmount; (tokens [i] > 306 || tokens [i] < 300) && tokens [i] != -10; i--); // Finds the last MultiOp
  while (exitNow != 1)  {
    currMultiOp = tokens [i]; // Sets the current MultiOp to the right token value
    currLoc = i; // Sets the location of the last MultiOp
    if (currMultiOp != -10)  { // If there is a MultiOp left in the argument
      for (i = currLoc; tokens [i] > 163 || tokens [i] < 100; i++); // Find the first argument
      valA = tokens [i] - 100; // Sets the first value of the MultiOp
            printf("AF%i\n", valA);
      tokens [i] = 0; // Sets the value pointed at to 0
      for (i2 = i - 1; tokens [i2] <= 202 && tokens [i2] >= 200; i2++) { // Loop to find all UniOps applying to the current value
	if (tokens [i2] == 200)  { // If the token is @
	  while (valA > cellSize)  { // If i2 overflows
	    valA -= cellSize; // Move i2 back
	  }
	  valA = cell [valA]; // Set the value to the value at the absolute location pointed at.
	}
	if (tokens [i2] == 201)  { // If the token is +
	  while ((currCell + valA) > cellSize)  { // If i2 overflows
	    valA -= cellSize; // Move i2 back
	  }
	  valA = cell [valA + currCell]; // Set the value to the value x to the right
	}
	if (tokens [i2] == 202)  { // If the token is -
	  while ((currCell - valA) < 0)  { // If i2 underflows
	    valA += cellSize; // Move i2 forwards
	  }
	  valA = cell [currCell - valA]; // Set the value to the value x to the left
	}
	tokens [i2] = 0; // Sets the value pointed at to 0
                }
printf("VA%i\n", valA);
            
      for (i = currLoc; tokens [i] > 163 || tokens [i] < 100; i++); // Find the second argument
      valB = tokens [i] - 100; // Sets the first value of the MultiOp
            printf("BF%i\n", valB);
      tokens [i] = 0; // Sets the value pointed at to 0
      for (i2 = i - 1; tokens [i2] <= 202 && tokens [i2] >= 200; i2++) { // Loop to find all UniOps applying to the current value
	if (tokens [i2] == 200)  { // If the token is @
	  while (valB > cellSize)  { // If i2 overflows
	    valB -= cellSize; // Move i2 back
	  }
	  valB = cell [valB]; // Set the value to the value at the absolute location pointed at.
	}
	if (tokens [i2] == 201)  { // If the token is +
	  while ((valB + currCell) > cellSize)  { // If i2 overflows
	    valB -= cellSize; // Move i2 back
	  }
	  valB = cell [valB + currCell]; // Set the value to the value x to the right
	}
	if (tokens [i2] == 202)  { // If the token is -
	  while ((currCell - valB) < 0)  { // If i2 underflows
	    valB+= cellSize; // Move i2 forwards
	  }
	  valB = cell [currCell - valB]; // Set the value to the value x to the left
	}
	tokens [i2] = 0; // Sets the value pointed at to 0
      }
            printf("VB%i\n", valB);
      
      if (currMultiOp == 300)  { // If the MultiOp is RAND
	tokens [currLoc] = (rand() % (valB)) + valA;
	tokens [currLoc] += 100;
	for (i = tokenAmount; (tokens [i] > 306 || tokens [i] < 300) && tokens [i] != -10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 301)  { // If the MultiOp is ADDN
	tokens [currLoc] = valA + valB;
	while (tokens [currLoc] > 63)  { // If the value overflows
	  tokens [currLoc] -= 64;
	}
	tokens [currLoc] += 100;
	for (i = tokenAmount; (tokens [i] > 306 || tokens [i] < 300) && tokens [i] != -10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 302)  { // If the MultiOp is SUBN
	tokens [currLoc] =valA - valB;
	while (tokens [currLoc] < 0)  { // If the value underflows
	  tokens [currLoc] += 64;
	}
	tokens [currLoc] += 100;
	for (i = tokenAmount; (tokens [i] > 306 || tokens [i] < 300) && tokens [i] != -10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 303)  { // If the MultiOp is MULN
	tokens [currLoc] = valA * valB;
	while (tokens [currLoc] > 63)  { // If the value overflows
	  tokens [currLoc] -= 64;
	}
	tokens [currLoc] += 100;
	for (i = tokenAmount; (tokens [i] > 306 || tokens [i] < 300) && tokens [i] != -10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 304)  { // If the MultiOp is DIVN
	tokens [currLoc] = valA / valB;
	tokens [currLoc] += 100;
	for (i = tokenAmount; (tokens [i] > 306 || tokens [i] < 300) && tokens [i] != -10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 305)  { // If the MultiOp is MODN
	tokens [currLoc] =valA % valB;
	tokens [currLoc] += 100;
	for (i = tokenAmount; (tokens [i] > 306 || tokens [i] < 300) && tokens [i] != -10; i--); // Finds the last MultiOp
      }
      if (currMultiOp == 306)  { // If the MultiOp is PAST (Will be implemented later)
      
      }
            printf("RS%i\n", tokens [currLoc] - 100);
    }
        
    else { // If no MultiOp remains in the argument
      for (i = tokenAmount; tokens [i] > 164 || tokens [i] < 100; i--);  // Find the last constant value
      valA = tokens [i] - 100; // Set valA to that value
      while (tokens [i] != -10)  { // While we haven't hit the end of the token array
	if (tokens [i] == 200)  { // If the token is @
	  while (valA > cellSize)  { // If val overflows
	    valA -= cellSize; // Move val back
	  }
	  valA = cell [valA]; // Set the value to the value at the absolute location pointed at.
	}
	if (tokens [i] == 201)  { // If the token is +
	  while (valA + currCell > cellSize)  { // If val overflows
	    valA -= cellSize; // Move val back
	  }
	  valA = cell [valA + currCell]; // Set the value to the value x to the right
	}
	if (tokens [i] == 202)  { // If the token is -
	  while (currCell - valA < 0)  { // If val overflows
	    valA -= cellSize; // Move val back
	  }
	  valA = cell [currCell - valA]; // Set the value to the value x to the left
	}
	i--;
      }
      exitNow = 1;
      printf("%i\n", valA);
    }
  }
}
