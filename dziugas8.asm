fun [20] 2
149
140

fun [40] 2
140
149

halt



mov bx [18]		; take the start of array
get				; get the number
mov [bx] ax		; put ax in memory
cmp bx [19]		; check if it is the end of array
je [32]			; if it is the end jump to ret
add bx 1		; increment bx
jmp [22]		; jump to begginning
ret [177]		; junk location






mov ax [39]		; get the start of array
cmp ax [38]		; compare with the end of array
je [82]			; end function, jump to ret
mov bx [39]		; get the location of the first member 
mov cx [bx]		; get the value from array
cmp cx [bx+1]	; compare with the next value
ja [61]			; if next value is lower jump to A
add bx 1		; increment bx
cmp bx [38]		; check if reached the end of array 
ja [74]			; if yes, jump to B
jmp [48]
mov ax [bx+1]	; swap values
mov [bx] ax
add bx 1
mov [bx] cx
cmp bx [38]
je [74]			; jump to B
jmp [40]		; jump to the beggining

mov ax [38]		; B
sub ax 1		; subtract
mov [38] ax		; change the value of the begginning of array
jmp [40]		; jump to ...
ret [177]		; junk location