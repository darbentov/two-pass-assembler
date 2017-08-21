main: add r4, M5[r1][r2]
str: .string "bla bla"
.entry main
.extern external
not r5
mov external, M5[r5][r3]
cmp main, r7
M5: .mat [1][3] 1,-2,3
prn M5[r1][r2]
rts
.data -3,14
cmp #-2, str
label: red external2
.extern external2
jmp label
lea main, M5[r2][r4]

