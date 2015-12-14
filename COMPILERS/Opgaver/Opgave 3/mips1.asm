.data
	a: .word 8
	b: .word 33
.text
main:
lw $t0, a				# load 8
lw $t1, b				# load 33
LoopStart:				# LABEL
beq $t1, $0, END			# Checking if t1 == 0
div $t0, $t1				# dividing to get modulus
mfhi $t2				# Getting the remainder, moving to $t2
beq $t2, $0, END			# checking if t2 == 0
sub $t3, $t1, $t0			# t3 = t1 - t0 
bgez $t3, ELSE				# t3 >= 0
sub $t0, $t0, $t1			# first then statement a = a - b
j LoopStart				# jumping to loopstart
ELSE:					# Now else statement
sub $t1, $t1, $t0			# b = b - a
j LoopStart
END:
					# tinyurl.com/neve79o
li $v0, 1				# Vi vil gerne printe vores to variable ud, som vi har udregnet.  
add $a0, $t0, $zero			# 
syscall					# 

li $v0, 11				# 
li $a0, 10				# 
syscall					# 

li $v0, 1				# 
add $a0, $t1, $zero			# 
syscall					# 