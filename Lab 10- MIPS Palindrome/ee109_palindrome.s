# ee109_palindrome.s

	.data
Buf :   .space  22                 # receive original input in this buffer
counts: .byte 0,0,0,0,0,0,0,0,0,0  # array of counts for digits 0-9
                                   # useful for finding the mode digit

# the following are constant strings that you can use
# for your prompts and messages... use syscall with $v0=4
msgin:  .asciiz "Enter up to 20 digits without spaces: "
msg1:   .asciiz "Your string: "
msg2:   .asciiz " IS a palindrome\n"
msg3:   .asciiz " IS NOT a palindrome\n"
msg4:   .asciiz "The mode digit is: "

# print this string for a newline character
nline:  .asciiz "\n"


	.text
main:
	li	$v0, 4		# Print the prompt
	la	$a0, msgin
	syscall
	
	li	$v0, 8		# Get the string from the user
	la	$a0, Buf
	li	$a1, 22
	syscall

	# this syscall will keep the newline character on the end of the string
	# so we will know we're at the end of a string when we hit the newline
	# newline = '\n' = 0x0a	

	# a lot more of your code

	
	li	$t0, 0x0a		# Place END OF LINE for analysis loop
	la	$t2, Buf		# $t2 contains inputted number
	la	$s7, counts		# Load counts
InputC:	lbu	$t3, 0($t2)		# $t2 contains bits of inputted number based on bit count
	beq	$t3, $t0, PCompare	# If 0x0a found, proceed to compare individual bits
	add 	$t3, $t3, $s7		# Add counts pointer to bit
	lb 	$s0, -0x30($t3)		# Load counts()
	add 	$s0, $s0, 1		# Add one to respective counts location
	sb 	$s0, -0x30($t3)	 	# Update counts()
	addi	$t2, $t2, 1		# Increases bit number of inputted number for comparison
	b	InputC			# Repeat
	
PCompare: addi	$t1, $t2, -0x10010000
	  beq	$t1, 1, PTrue		# If input is just one number, it is automatically a palindrome
	  la	$t3, Buf		# Reset $t3 to original inputted number
CompLp:	  lb	$t4, 0($t3) 		# $t5 contains bits of inputted nubmer based on upwards bit count
	  lb	$t5, -1($t2)		# $t6 contains bits of inputted number based on downwards bit count; Offset needed to remove End of Line from comparison
	  bne	$t4, $t5, PFalse	# If upward bit does not equal its downward bit at any time, inputted number is not palindrome, so proceed to declare false
	  bge	$t3, $t2, PTrue		# If upward count becomes greater than or equal to the downward count without false detections, palindrome is true, so proceed to declare true
	  addi	$t2, $t2, -1		# Downward Bit Count
	  addi	$t3, $t3, 1		# Upward bit count
	  b	CompLp
	  
PTrue:	li	$v0, 4		# Print the prompt
	la	$a0, msg1
	syscall
	la	$a0, Buf
	syscall
	la	$a0, msg2
	syscall
	b	Mode

PFalse:	li	$v0, 4		# Print the prompt
	la	$a0, msg1
	syscall
	la	$a0, Buf
	syscall
	la	$a0, msg3
	syscall
	b	Mode

Mode:	# $s2 Mode count
	# $s1 General Count
	# $s3 Mode number
	la 	$s1, counts	# Load counts into #s1
	addi	$a1, $s1, 9	# Starts counts address at [9]

FindMode:  blt 	$a1, $s1, PMode 	# If finished, print
	   lb 	$s4, ($a1) 		# Load count()
	   bgt 	$s4, $s2, SaveMode 	# If number of occurences greater, save new mode
	   addi $a1, $a1, -1 		# Moves count downward
	   b 	FindMode

SaveMode:  add 	$s2, $s4, $zero 	# stores the highest number mode
	   sub 	$s3, $a1, $s1		# stores mode number
	   addi $a1, $a1, -1 		# moves pointer array upward
	   b 	FindMode

PMode: 	la 	$a0, msg4
	syscall
	li 	$v0, 1
	move 	$a0, $s3	# Prints mode digit
	syscall
	
Ending:	# exit
	li	$v0, 10
	syscall
