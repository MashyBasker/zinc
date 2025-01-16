.data

a = 255
c = 253

.text

ldi A 5
mov M A %a
mov A M %a
ldi B 7
add
mov M A %c
mov A M %a
ldi B 5
je %if_true_0
jmp %if_end_1
if_true_0:
mov A M %a
ldi B 1
add
mov M A %a
if_end_1:

hlt
