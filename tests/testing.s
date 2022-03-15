addi x1,x2,4
call:	li x12,10
	jr ra

main:
	li x10,-32
	li x11,-50
	j  call

