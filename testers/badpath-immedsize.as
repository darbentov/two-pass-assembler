; This file contains tests for errors in the range of immediate operands.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; Depending on implementation, these errors are detectable
; either in the first or second pass.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; 1. this line is ok
DATA1: .data  100

; 2. this line is ok (largest positive immediate that fits in 8 bits)
       cmp #127,DATA1

; 3. invalid first operand (positive immediate too large to fit in 8 bits)
       cmp #128,DATA1

; 4. this line is ok (lowest negative immediate that fits in 8 bits)
       cmp r2,#-128

; 5. invalid second operand (negative immediate too large to fit in 8 bits)
       cmp r2,#-129

; 6. this line is ok
       stop