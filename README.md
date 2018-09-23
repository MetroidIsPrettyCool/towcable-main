The Waggish Cell-Based Language '18:

TWCBL is a free, open source programming language built around creating 1-d cellular automata.

Commands:

SIZE #                              -  Sets the size of the one dimensional plane the cells reside in. All programs MUST start with SIZE.

VALS # # # # # #...                 -  Gives each cell a starting value. Numbers must be a one-digit base 64 value.

FUNC # IFEQ [...] IFEQ [...]... # # -  Creates a rule with a one-digit base 64 id. If all if statements contained evaluate to true, set the cell the rule is being called upon to the second value, third if not.

IFEQ #=#                            -  If statement can compare cells with values or cells to constants. Must be after a RULE statement

BEGN                                -  Ends initialization. All above commands can no longer be used.

CALL # (# or &)                     -  Runs the function selected by the first value, on the cell selected by the second. Use & to run the rule on ALL cells.

DEST #                              -  Specifies a return point for a loop, with a base 64 id. Allways preceds a loop statement.

LOOP # #                            -  Loops an amount of times as defined by the first value, going to tthe DEST whose i.d. matches the second value.

OUTP (# or &)                       -  Outputs the value at cell #, or all cells. Always prints a newline at the end.

USER #                              -  User inputs a base 64 value to a cell.

RAND # #                            -  Generates a random number between value one and value two, inclusive. This can take the place of any number in the  program. e.x. @RAND0S +RAND25 -RAND04 %RANDpP

Characters of Note:

"@"                                 -  Absolute location for a cell.

"+"                                 -  Cell ahead of the cell the rule has been called upon.

"-"                                 -  Cell behind the cell the rule has been called upon.

"%"                                 -  Indicates a constant. ONLY THIS CAN BE USED PRE-BEGN (excepting IFEQ statements). e.x. SIZE %6 SET %0 %8 %g %0 %0 %5

"&"                                 -  Means all cells, or means the loop is infinte.

"!"                                 -  Starts and ends comments.

whitespace                          -  Ignored entirely. RULE%6IF@k=%0IF+1=%j%6%1 is valid, but not recomended.

More notes:

All values are base 64. However, I use my own notation scheme for the base. Starting from 0 to 63: 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'"

(Sorry if that's confusing)

Compilers and Interperters should fail if a loop refers to a return point that is further down in the code than itself.

Compilers and Interperters should fail if given a number that is non base 64 and more that one digit.

Compilers and Interperters should fail if DEST is given a cell as an i.d.

Compilers and Interperters should fail if +0 is written -0. +0 is the only way to express the current cell.

Value characters (@,+,-,%) can stack. However, it's bad form to use % when stacking. +%%%%%%6 is the same as +6, so just use the shortest length. +@3 is fine, and so is ++9, +-@+4, etc.

The 1-d plane should wrap around at the edges.

USER & is stupid, and should never be implemented by anybody.
