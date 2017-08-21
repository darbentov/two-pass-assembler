; This file contains tests for errors in .mat directives and matrix operands.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the first pass of the assembler.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not exhaustive; 
;             you are encouraged to identify additional errors.

; 1. this line is ok
MAT1:   .mat [2][3] 1,2,3,4,5,6

; 2. this line is ok 
STR1:   .string "abcdefg"

; 3. this line is ok (no need to provide a label or init values)
        .mat [2][3]

; 4. invalid characters(,7,8) 
        .mat [2][3] 1,2,3,4,5,6,7,8

; 5. this line is ok (no need to provide all init values) 
        .mat [2][3] 1,2,-3,-4

; 6. invalid characters (,1,1,1,1) 
        .mat [2][2],1,1,1,1
				
; 7. invalid characters (r5,6) 
        .mat [2][3] 1,2,3,4,r5,6

; 8. invalid characters (-2]) 
        .mat [5][-2]

; 9. invalid characters (r4][r1]) 
        .mat [r4][r1]

; 10. invalid argument (missing dimension) 
        .mat [3]

; 11. invalid characters([4]) 
        .mat [2][3][4]
				
; 12. invalid characters (1][2])
        cmp r1,MAT1[1][2]

; 13. invalid characters (8])
        cmp r1,MAT1[r2][r8]

; 14. this line is ok (label need not come from .mat directive)
        cmp r1,STR1[r4][r4]

; 15. invalid operand (missing dimension)
        prn MAT1[r5]

; 16. invalid characters([r6])
        prn MAT1[r5][r4][r6]

; 17. invalid characters ([r1][r2])
        cmp #3,MAT1 [r1][r2]

; 18. invalid characters ([r3][r2])
        add #3,[r3][r2]
		
; 19. invalid characters(xyz])
        cmp  r1,MAT1[r3][xyz]
