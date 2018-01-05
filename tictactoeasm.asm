1				; player's number
get				; get a number
cmp ax 1		; check if it is in range
jb [1]			; get another number if not in range
cmp ax 9		; check if it is in range
ja [1]			; get another number if not in range

cmp ax 1		; compare every number
jne [27]		; not this number
mov dx 0
cmp dx [491]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [491] dx	; put player's number
jmp [153]
cmp ax 2		; compare every number
jne [43]		; not this number
mov dx 0
cmp dx [492]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [492] dx	; put player's number
jmp [153]
cmp ax 3		; compare every number
jne [59]		; not this number
mov dx 0
cmp dx [493]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [493] dx	; put player's number
jmp [153]
cmp ax 4		; compare every number
jne [75]		; not this number
mov dx 0
cmp dx [529]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [529] dx	; put player's number
jmp [153]
cmp ax 5		; compare every number
jne [91]		; not this number
mov dx 0
cmp dx [530]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [530] dx	; put player's number
jmp [153]
cmp ax 6		; compare every number
jne [107]		; not this number
mov dx 0
cmp dx [531]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [531] dx	; put player's number
jmp [153]
cmp ax 7		; compare every number
jne [123]		; not this number
mov dx 0
cmp dx [567]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [567] dx	; put player's number
jmp [153]
cmp ax 8		; compare every number
jne [139]		; not this number
mov dx 0
cmp dx [568]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [568] dx	; put player's number
jmp [153]
cmp ax 9		; compare every number
jne [153]		; not this number
mov dx 0
cmp dx [569]	; check if this place is already taken
jne [1]			; jump to beginning
mov dx [0]		; get player's number
mov [569] dx	; put player's number
mov dx [0]		; get player's number	
add dx [0]		; player's number * 3 = won
add dx [0]
mov [532] dx	; store the value
mov bx 0
mov dx 0
cmp bx [491]
je [187]		; jump to next check
add dx [491]
cmp bx [492]
je [187]		; jump to next check
add dx [492]
cmp bx [493]
je [187]		; jump to next check
add dx [493]
cmp dx [532]	; check if won
je [397]
mov bx 0		; next check 2|
mov dx 0
cmp bx [529]
je [213]		; jump to next check
add dx [529]
cmp bx [530]
je [213]		; jump to next check
add dx [530]
cmp bx [531]
je [213]		; jump to next check
add dx [531]
cmp dx [532]	; check if won
je [397]
mov bx 0		; next check 3|
mov dx 0
cmp bx [567]
je [239]		; jump to next check
add dx [567]
cmp bx [568]
je [239]		; jump to next check
add dx [568]
cmp bx [569]
je [239]		; jump to next check
add dx [569]
cmp dx [532]	; check if won
je [397]
mov bx 0		; next check 1_
mov dx 0
cmp bx [491]
je [265]		; jump to next check
add dx [491]
cmp bx [529]
je [265]		; jump to next check
add dx [529]
cmp bx [567]
je [265]		; jump to next check
add dx [567]
cmp dx [532]	; check if won
je [397]
mov bx 0		; next check 2_
mov dx 0
cmp bx [492]
je [291]		; jump to next check
add dx [492]
cmp bx [530]
je [291]		; jump to next check
add dx [530]
cmp bx [568]
je [291]		; jump to next check
add dx [568]
cmp dx [532]	; check if won
je [397]
mov bx 0		; next check 3_
mov dx 0
cmp bx [493]
je [317]		; jump to next check
add dx [493]
cmp bx [531]
je [317]		; jump to next check
add dx [531]
cmp bx [569]
je [317]		; jump to next check
add dx [569]
cmp dx [532]	; check if won
je [397]
mov bx 0		; next check 1\
mov dx 0
cmp bx [491]
je [343]		; jump to next check
add dx [491]
cmp bx [530]
je [343]		; jump to next check
add dx [530]
cmp bx [569]
je [343]		; jump to next check
add dx [569]
cmp dx [532]	; check if won
je [397]
mov bx 0		; next check 1/
mov dx 0
cmp bx [493]
je [369]		; change player
add dx [493]
cmp bx [530]
je [369]		; change player
add dx [530]
cmp bx [567]
je [369]		; change player 
add dx [567]
cmp dx [532]	; check if won
je [397]
mov dx [0]		; get player's number
cmp dx 1		; check if it is 1
jne [382]		; if it is not 1 jump to 382
mov dx 2		; if it is 1, change it to 2
mov [0] dx		; put new value to [0]
dump			; print the board
jmp [387]		; jump to increment cx
mov dx 1		; 382 - change 2 to 1
mov [0] dx		; put new value to [0]
dump			; print the board
add cx 1		; increment cx
cmp cx 9		; check if the game ended
je [397]		; if yes jump to halt
jmp [1]			; else jump to begginning



halt