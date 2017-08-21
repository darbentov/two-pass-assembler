; This file is intended to check the bad-path of the assembler.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the first pass of the assembler.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not exhaustive; 
;             you are encouraged to identify additional errors.

; 1. this line is ok (DATA4 is declared later)
Start:  dec DATA4

; 2. missing operand
        sub #5

; 3. missing operand
        red

; 4. this line is ok
       .extern DATA4

; 5. invalid target operand (immediate)
        add  #5,#6

; 6. this line is ok (immediate target operand allowed for cmp)
Next:   cmp  #5,#6

; 7. invalid operand (immediate)
        inc  #50

; 8. undefined instruction
        and  r1,r2

; 9. undefined instruction (case sensitivity)
        jSr  Start

; 10. this line is ok (r9 is a label declared later)
        add  r1,r9

; 11. invalid characters (,r2)
        cmp  r1,,r2

; 12. invalid characters (,r3)
        add  #5,r1,r3

; 13. invalid characters (blabla)
        prn r1 blabla

; 14. invalid characters (123)
        inc  123

; 15. invalid characters (xyz)
        cmp  #xyz,r1

; 16. label previously declared
Next:   clr  r2

; 17. label does not start in first column (the label declaration is ignored)
     label1:   sub r1,r7

; 18. invalid source operand (register)   
        lea  r3,r1

; 19. invalid source operand (immediate)   
        lea  #3,r5

; 20. invalid characters (Start)
        stop Start

; 21. invalid characters (400) 
        .data   200 400

; 22. this line is ok (arbitrary whitespace between operands)
DATA1:  .data   100,   200 ,  -500, 300

; 23. invalid characters (,3)
        .data   1, ,3

; 24. invalid character (,)
        .data   4,

; 25. invalid characters (#123)
        .data   #123

; 26. invalid characters (.4)
        .data   12.4

; 27. invalid characters (-5)
        .data   --5

; 28. this line is ok (case sensitive labels)
Data1:  .data   100, +200, -300

; 29. invalid label (cannot be an instruction)
mov:    .data   5

; 30. invalid label (cannot be a register)
r1:     .data   200,300

; 31. label previously declared
DATA1:  .data   300

; 32. invalid label(non-alphabetic first character)
1DATA:  .data   300

; 33. this line is ok (r9 is not the name of a register)
r9:     .data   200

; 34. this line is ok (label X declaration ignored - warning may be issued)
X:      .entry  DATA1

; 35. this line is ok (it is ok to declare the same external more than once)
        .extern DATA4

; 36. local label cannot be declared as external
        .extern Start    
		
; 37. this line is ok (label Y declaration ignored - warning may be issued)
Y:      .extern DATA8

; 38. this line is ok (STR1 is declared later)
        .entry STR1

; 39. label is already designated as external
        .entry  DATA4

; 40. undefined instruction (note: DATA2 is not a label declaration)
DATA2   .data   4

; 41. undefined directive (case sensitivity)
        .DATA   5

; 42. This line is ok (it is ok to designate the same entry more than once)
        .entry  STR1

; 43. invalid characters (blabla is not a string)
        .string blabla

; 44. invalid characters (blabla)
        .string "abcdefg" blabla

; 45. this line is ok (comma within string is not a separator)
STR1:   .string "abc, ,defg"

; 46. invalid label (too long)
SuperCalifragilisticExpiAlidocious: .data	4	
          
; 47. missing argument in directive
        .data

; 48. missing argument in directive
        .entry

; 49. undefined directive
        .invalid 85,90

; 50. this line is ok
        stop