#include <stdio.h>

int baseConvert (int number);

int main (void)  {
  FILE *fp;
  fp = fopen("./twcbl.cbi", "r"); // need to add a way to chose what file to open from the command line or something.
  int a = 0;
  int c = 0;
  int l = 1;
  int b = 0;
  int x = 0;
  int errorTemp = 0; //Used for various error checkers
  int currCommand = 0;
  int complete = 0;
  int done = 0;
  int input = 0;
  int tokened [10000];
  for (c = 0; c != 10000; c++)  {
    tokened [c] = -1;
  }
  int array [64];
  int temp = 0;
  char commandsInit [5] [4] = {
    'S','I','Z','E',
    'V','A','L','S',
    'F','U','N','C',
    'I','F','E','Q',
    'B','E','G','N'
  };
  char commands [6] [4] = {
    'C','A','L','L',
    'D','E','S','T',
    'L','O','O','P',
    'O','U','T','P',
    'U','S','E','R',
    'R','A','N','D'
  };
  char characters [7] = {
    '@','+','-','%','&','!','.'
  };
  char whitespace [3] = {' ','\n','\t'};
  int commandArgs [11] [3] = {
    1,0,0, //SIZE has one argument, and it can't be &, and doesn't allow for infinite arguments
    0,0,1, //VALS has no standard argument number, can't have & as a value, and does allow infinite arguments
    3,0,0, //FUNC has three arguments, can't have & be a number, and doesn't allow for infinite arguments (note, IFEQ arguments are checked by a special bit of code that only runs for FUNC statements)
    2,0,0, //IFEQ has two arguments
    0,0,0, //BEGN has no arguments
    2,1,0, //CALL has two arguments, and the last one can be &
    1,0,0, //DEST has one argument
    2,1,0, //LOOP has two arguments, and the last one can be &
    1,1,1, //OUTP has one argument, which can be &. If it isn't &, infinite possible arguments
    1,0,0, //USER needs only one argument
    2,0,0  //RAND needs two arguments
  };
  while (!done)  {
    input = fgetc(fp);
    if (input == commandsInit [currCommand - 1] [complete] && complete < 4 && complete != 0)  { //If it detects we're entering another bit o' command, it'll increment the completeness of the command.
      complete++;
      if (complete == 4) {
	tokened [a] = 100 + currCommand;
	if (currCommand == 5)  {
	  done = 1;
	}
	a++;
      }
    }
    else if (input == characters [6] && commandArgs [currCommand - 1] [2] == 1 && complete == 4)  { //If it finds a period
      currCommand = 0;
      complete = 0;
      b = 0;
    }
    else if (input == whitespace [1])  { //Increment line count if a newline is found.
      l++;
      x = 0;
    }
    else if (input == -1)  {  //EOF finder
      printf("ERROR: FILE TERMINATED EARLY\n");
      fclose(fp);
      return 0;
    }
    else if (complete == 0)  {  //Find out which command we're supposed to be running
      for (c = 0; c != 6; c++)  {
	if (input == commandsInit [c] [0]) {
	  currCommand = c + 1;
	}
	else if (currCommand == 0 || currCommand == -1) {
	  currCommand = -1;
	}
      }
      if (currCommand == -1)  {
	printf("ERROR: ILLEGAL CHARACTER STARTING/ENDING COMMAND AT LINE %d, CHARACTER %d : %c\n", l, x, input);
	fclose(fp);
	return 0;
      }
      complete++;
    }
    else if ((input == characters [0] || input == characters [1] || input == characters [2] || input == characters [3]) && (complete == 4 || complete == 5))  { //If the input is a value modifier
      if (complete == 5)  {
	complete = 4;
      } 
      if (currCommand != 3 && input != characters [3])  {
	printf("ERROR: ILLEGAL CHARACTER IN ARGUMENT AT LINE %d, CHARACTER %d : %c\n", l, x, input);
	fclose(fp);
	return 0;
      }
      if (input == characters [0])  {
	tokened [a] = 200;
      }
      else if (input == characters [1])  {
	tokened [a] = 201;
      }
      else if (input == characters [2])  {
	tokened [a] = 202;
      }
      else if (input == characters [3])  {
	tokened [a] = 203;
      }
      a++;
      complete++;
    }
    else if (baseConvert(input) != -1 && complete == 5)  { //If the input is a value
      b++;
      tokened [a] = baseConvert(input);
      a++;
      if (currCommand == 2)  {
	errorTemp++;
      }
      if (b == commandArgs [currCommand - 1] [0] && commandArgs [currCommand - 1] [2] != 1)  {
	currCommand = 0;
	complete = 0;
	b = 0;
      }
      else {
	complete = 4;
      }
    }
    else if (input != whitespace [0] && input != whitespace [2]) {  //Catch-all error
      printf("ERROR: ILLEGAL CHARACTER IN FILE AT LINE %d, CHARACTER %d : %c\n", l, x, input);
      fclose(fp);
      return 0;
    }
    //Error Checks
    if (tokened [2] == 0)  { //Checks to see if the array size is 0
      printf("ERROR: PLANE HAS DIMENSION 0\n");
      fclose(fp);
      return 0;
    }
    if (tokened [0] != 101 && tokened [0] != -1)  { //Checks to see if the first command is SIZE
      printf("ERROR: COMMAND 1 IS NOT \"SIZE\"\n");
      fclose(fp);
      return 0;
    }
    if (tokened [3] != 102 && tokened [3] != -1)  { //Checks to see if the second command is VALS
      printf("ERROR: COMMAND 2 IS NOT \"VALS\"\n");
      fclose(fp);
      return 0;
    }
    x++;
  }
  done = 0;
  if (errorTemp != tokened [2])  { //Checks to see if VALS has the correct number of arguments
    printf("ERROR: VALS HAS AN INCORRECT NUMBER OF ARGUMENTS\n");
    fclose(fp);
    return 0;
  }
  errorTemp = 0;
  /* for (c = 0; c != a; c++)  { //Prints out all Tokens */
  /*   printf("%d\n", tokened [c]); */
  /* } */
}

int baseConvert (int number)  {
  if (number >= 48 && number <= 57)  {  //If it's a number
    return (number - 48);
  }
  if (number >= 97 && number <= 122)  { //If it's a lowercase letter
    return (number - 87);
  }
  if (number >= 65 && number <= 90)  {  //If it's an uppercase letter
    return (number - 29);
  }
  if (number == 43)  {                  //If it's a plus.
    return 62;
  }
  if (number == 45)  {                  //If it's a minus.
    return 63;
  }
  else {                                //Error
    return -1;
  }
}
