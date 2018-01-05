fun [40] 2	; enter numbers until reaches 10 or is equal to 0 
110			; start of array
119			; end of array

fun [80] 3	; add numbers and return sum to 151 (return by reference)
[150]		; end of array
110			; start of array
151			; return sum here
mov ax [151]
put
halt



















mov bx [39]		; get start of array
get				; get a number
cmp ax 0		; check if it is below or equals 0
jbe [42]		; if yes enter again
cmp ax 100		; check if it is above or equals 100
jae [42]		; if yes enter again
mov [bx] ax		; get location
add bx 1		; increment bx
cmp bx [38]		; check if it is the end of array
ja [60]			; if it is end the loop
jmp [42]		; jump to beginning
mov ax bx		; get the end of array
ret [150]		; return the end of array

















mov ax 0		
mov bx [78]		; get the start of array
mov cx [bx]		; get the value of array
add ax cx		; add values
add bx 1		; increment	
cmp bx [79]		; check if it is the end of array
jbe [84]	
mov bx [77]		; get the location where to store the sum 
mob [bx] ax		; return sum by reference
ret [160]		; junk location