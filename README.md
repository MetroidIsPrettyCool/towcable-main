# Hey!
I'd like to apologize for the contents of this repository, and I'll hope you don't judge me too harshly for them.

These are the workings of an unmedicated, self-taught, impulsive, 13-year-old me.

This was an attempt to develop a programming language around the concept of cellular automata (something I'm still interested in doing right)

This was also the first time I had even attempted to do such a thing.

In fact, this wasn't just my first real interpreter (bf and deadfish don't count), it was my first time using git, and my first time doing memory management in C.

I'd bitten off the entire back half of an elephant, and all I could chew was the pinkie toes.

The internal workings of this software are inane, the syntax of the language is worse, and there's a memory leak *somewhere* in the spaghetti that I cannot be bothered to find.

I'm leaving this (and the rest of the OG readme/personal language reference) for posterity, and as a monument to my sins.

Here be dragons.

---

TOWCBL Syntax (right now this is mostly for me, and if it's not readable to others, let me know.)

READ_ME_TODO: add descriptions to commands

Commands

  Setup
    DIMN 500
    SIZE 501
    SETP 502
    RULE 503
    CMPX 504
    BEGN 505
    REGI 506

  Runtime
    OUPT 600
    CRPT 601
    INPT 602
    CALL 603
    CALX 604
    LOOP 605
    LOTO 606
    COND 607
    ENDC 608
    STRO 609
    EXIT 610
    SETR 611

Numerical Operators
  
  RAND 300
  ADDN 301
  SUBN 302
  MULN 303
  DIVN 304
  MODN 305
  PAST 306
  LOCN 307
  ATLO 308
  RVAL 309

Location Operators

  @,+,- 200, 201, 202

Comparatives

  EQ 400
  NQ 401
  GT 402
  GE 403
  LT 404
  LE 405

Other:
  
  i
  & 100
  r 1738
  . 101
  , 102
  
Basic Argument Structure

Numbers

NUMi (numbers must be whole numbers)

Examples:

10i
13i
7i
234i
124345i
902390i

Location Modifiers:

LOCMOD

Examples:
@92i
+5i
-8i

Flat MultiOps

MultiOp ARG ARG

Examples:
ADDN 8i 7i
MULN @ 3i 7i 78i
DIVN RAND 6i 8i 2i

LOCN

LOCN LCMOD ARG LCMOD ARG (number of LCMOD's and ARGs is equal to dimensions LCMODs MUST be followed by an ARG)

Examples:

LOCN +8i -0i
LOCN @5i
LOCN @ ADDN 5i 9i +3i

PAST

PAST ARG ARG  ARG ARG (Argument number is equal to dimensions  + 1)

Examples:

PAST 1i 8i
PAST 2i 8i 7i @ 9i 78i
PAST 6i 8i 23i

STRING

"STRING" (only used by STRO. They cannot contain periods or double quotes, I'm too lazy to implement escape characters. They alos canot be operated on by MultiOps)

Examples:

"HELLO WORLD!"
"Similes vs Metaphors"
"TOWCBL"

IF

IF ARG ARG (Used only by conditional statements. Shouldn't be operated on by MultiOps (i.e. You can, but why would you?))

Examples:

EQ 7i 8i
NQ 1i @1i
LT 8i 9i
GE 7i 19i

Basic Command Structure

Standard Command
COMMAND ARG ARG ARG. (Argument number is just an example, each command has different amounts of arguments)

Examples:
INPT @ 5i.
OUPT.
CALL ADDN @ 7i @ @ 5i.
SETP 4i 6i 17i 200i.

Conditional Argument Structure
COMMAND ARG IF IF ARG ARG. (Again, argument number and makeup varies, this is just an example. Number of IFs in a row is arbitrary, can be any amount)

Examples:
COND EQ 7i @ 8i 7i 9i.
RULE 79i 17i + 2i NQ 58i +7i LT + 1i - 1i.

& Argument Structure

COMMAND ARG ARG/&.

Examples:

LOOP 9i 7i.
LOOP 194i &.

STRO Argument Structure:

STRO STRING.

Examples:

STRO "Hello World\n".
STRO "Stringy\tmcStringface\n".

Complex argument structure

CMPX ARG ARG ARG IF... , ARG IF... , ...

Examples:

CMPX 0i 2i 4i EQ 2i 2i 5i EQ LOCN @ 4i 2i.

Extra Notes:

Whitespace is ignored entirely
Periods MUST end commands
Some commands, such as SIZE, have changing numbers of arguments based off other commands. Make sure to check those values!
STRO  is the ONLY command that deals with strings

Error messages:
ERR 00: PROGRAM LACKS NECCESSARY SETUP COMMANDS
ERR 01: ILLEGAL COMMAND (command)
ERR 02: UNEXPECTED EOF
ERR 03: INVALID PARAMETER AMOUNT
ERR 04: ONE OR MORE ARGUMENTS TERMINATED EARLY
ERR 05: ILLEGAL OR UNEXPECTED CHARACTER (character)
ERR 06: DIMENSIONS 0
ERR 07: LENGTH 0
ERR 08: MULTIPLE CALLS TO DIMN
ERR 09: MULTIPLE CALLS TO SIZE
ERR 10: MULTIPLE CALLS TO SETP
ERR 11: NON-BOOLEAN RESULT FOR TEST
ERR 12: RULE I.D. ALREADY IN USE
ERR 13: USE OF LOCN OUTSIDE OF RULE OR CMPX
ERR 14: NEWLINE IN STRING
ERR 15: I/O ACCESSING ILLEGAL LOCATION
ERR 16: LOOPING TO NON-EXISTANT LOCATION
ERR 17: CALLING NON-EXISTANT RULE OR CMPX
ERR 18: RULE OR CMPX ACCESSING ILLEGAL LOCATION
ERR 19: NUMBER TERMINATED BY ILLEGAL CHARACTER (character)
ERR 20: MISSING DOUBLE QUOTE
ERR 21: CMPX I.D. ALREADY IN USE
ERR 22: ILLEGAL ESCAPE SEQUENCE
ERR 23: & IN ILLEGAL LOCATION
ERR 24: COMMA OUTSIDE CMPX
ERR 25: REGISTER ALREADY EXISTS
ERR 26: ATTEMPTING TO SET NON-EXISTANT REGISTER
ERR 27: ATTEMPTING TO READ VALUE OF NON-EXISTANT REGISTER
ERR 42: I'M A TEAPOT

Interpreter only errors

ERR 0I: INVALID SOURCE FILE
ERR 1I: ILLEGAL FILE EXTENSION
ERR 2I: INVALID ARGUMENTS
