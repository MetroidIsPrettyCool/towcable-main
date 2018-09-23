#include <stdio.h>

#define SIZE 1
#define VARS 2
#define FUNC 3
#define IFEQ 4
#define BEGN 5
#define CALL 6
#define DEST 7
#define LOOP 8
#define OUTP 9
#define USER 10
#define RAND 11

int baseConvert (int number);

int main (void)  {
  FILE *fp;
  fp = fopen("./twcbl.cbi", "r"); // need to add a way to chose what file to open from the command line or something.
  int i = 0;
  int a = 0;
  int c = 0;
  int done = 0;
  int input = 0;
  int currCommand = 0;
  int complete = 0;
  int finding = 1;
  int tokened [10000];
  int arraySize = 0;
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
    0,0,1, //VARS has no standard argument number, can't have & as a value, and does allow infinite arguments
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
    while (finding)  {
      input = fgetc(fp);
      if (input == commandsInit [0] [i])  {
	i++;
	currCommand = 1;
	complete++;
	if (complete == 4) {
	  while (finding)  {
	    input = fgetc(fp);
	    if (input == characters [3]) {
	      while (finding) {
		input = fgetc(fp);
		if (input != whitespace [0] && input != whitespace [1] && input != whitespace [2])  {
		  if (baseConvert (input) != -1)  {
		    finding = 0;
		    arraySize = baseConvert (input);
		    tokened [0] = 1;
		    tokened [1] = arraySize;
		    c = 2;
		  }
		}
	      }
	    }
	    else if (input != whitespace [0] && input != whitespace [1] && input != whitespace [2])  {
	      printf("Error: Illegal Character in Argument: %c\n", input);
	      fclose(fp);
	      return 0;
	    }
	  }
	}
      }
      else if (input != whitespace [0] && input != whitespace [1] && input != whitespace [2])  {
	printf("Error: Illegal Character in Function: %c\n", input);
	fclose(fp);
	return 0;
      }
    }
    if (arraySize == 0)  {
      printf("Error: Array Size is 0\n");
      fclose(fp);
      return 0;
    }
    finding = 1;
    i = 0;
    complete = 0;
    while (finding)  {
      input = fgetc(fp);
      if (input == commandsInit [1] [i])  {
	i++;
	currCommand = 1;
	complete++;
	tokened [2] = 2;
	c = 3;
	if (complete == 4) {
	  while (finding)  {
	    input = fgetc(fp);
	    if (input == characters [6])  {
	      finding = 0;
	    }
	    else if (input == characters [3]) {
	      while (finding) {
		input = fgetc(fp);
		if (input != whitespace [0] && input != whitespace [1] && input != whitespace [2])  {
		  if (baseConvert (input) != -1)  {
		    tokened [c] = baseConvert (input);
		    c++;
		    break;
		  }
		}
	      }
	    }
	    else if (input != whitespace [0] && input != whitespace [1] && input != whitespace [2])  {
	      printf("Error: Illegal Character in Argument: %c\n", input);
	      fclose(fp);
	      return 0;
	    }
	  }
	}
      }
      else if (input != whitespace [0] && input != whitespace [1] && input != whitespace [2])  {
	printf("Error: Illegal Character in Function: %c\n", input);
	fclose(fp);
	return 0;
      }
    }
    if (c - 3 != arraySize)  {
      printf("Error: Incorrect Number of Arguments for VALS\n");
      fclose(fp);
      return 0;
    }
    done = 1;
  }
}

int baseConvert (int number)  {
  if (number >= 48 && number <= 57)  {
    return (number - 48);
  }
  if (number >= 97 && number <= 122)  {
    return (number - 87);
  }
  if (number >= 65 && number <= 90)  {
    return (number - 29);
  }
  if (number == 43)  {
    return 62;
  }
  if (number == 45)  {
    return 63;
  }
  else {
    return -1;
  }
}
