fun [20] 3
100			; linked list starts here
180			; keeping track of head
180			; max memory for linked list
halt













mov bx [19]
mov cx 0
mov dx [18]
get				; add head
cmp ax 0
jbe [71]		; if equals to 0
mov [bx] ax		; put value to [bx]
mov [16] bx		; hold value's address
add bx 1
mov [bx] dx		; link to memory that holds head
mov [180] 100	; head is first value
add bx 1
add cx 1
get
cmp ax 0
jbe [71]
mov [bx] ax		; put value to [bx]
mov dx [16]		; hold previous value's memory location before incrementing
mov [16] bx		; move current value's memory location
mov [180] bx	; link to location that holds head's address
add bx 1
mov [bx] dx		; link to previous value
add bx 1
cmp bx [17]
jae [71]
add cx 1
jmp [44]		; repeat
ret [181]		; junk location