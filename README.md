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

Numerical Operators
  
  RAND 300
  ADDN 301
  SUBN 302
  MULN 303
  DIVN 304
  MODN 305
  PAST 306

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

Flat MultiOps

MultiOp ARG ARG

Examples:
ADDN 8i 7i
MULN @ 3i 7i 78i
DIVN RAND 6i 8i 2i

Scaling MultiOps

MultiOp ARG ARG ARG (Agument number is equal to the number of dimensions the program is running in)

Examples:
@ 8i
+ 9i 8i 7i
- 8i 9i 10i 100i @ 8i 9i 7i 10i 89i

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

IF ARG ARG (Used only by conditional statements. Cannot be operated on by MultiOps)

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
CMPX 66i 2i GE 6i - 2i @ 6i GT 1i + 0i.

& Argument Structure

COMMAND ARG ARG/&. (only LOOP uses &, so this is the only way this can be written)

Examples:

LOOP 9i 7i.
LOOP 194i &.

STRO Argument Structure:

STRO STRING.

Extra Notes:

Whitespace is ignored entirely
Periods MUST end commands
Some commands, such as SIZE, have changing numbers of arguments based off other commands. Make sure to check those values!
STRO  is the ONLY command that deals with strings
