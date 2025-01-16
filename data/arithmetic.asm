.data

a = 255
b = 253
c = 251

.text

ldi A 5
mov M A %a
ldi A 6
mov M A %b
mov A M %a
mov B M %b
add
mov M A %c
mov A M %c
ldi B 11
je %if_true_0
jmp %if_end_1
if_true_0:
mov A M %a
ldi B 1
add
mov M A %a
if_end_1:

hlt
