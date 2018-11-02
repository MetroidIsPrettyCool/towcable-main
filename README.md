TOWCBL Syntax (right now this is mostly for me, and if it's not readable to others, let me know.)

READ_ME_TODO: add descriptions to commands

Commands

  Setup
    SIZE x
    SETP ...
    RULE x x x iii
    CMPX x ...
    BEGN

  Runtime
    OUPT ... or &
    INPT x
    CALL x ... or &
    CALX x ... or &
    LOOP x x or &
    LOTO x 
    COND iii
    ENDC
    
    

Numerical Operators
  
  RAND x x
  ADDN x x
  SUBN x x
  MULN x x
  DIVN x x
  MODN x x
  PAST x x

Location Operators

  @ x
  + x
  - x
  % x

Comparatives

  IFEQ x x
  IFNQ x x
  IFGT x x
  IFGE x x
  IFLT x x
  IFLE x x
