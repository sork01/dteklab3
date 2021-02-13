  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.global delay
.global hexasc
.global time2string
.global enable_interrupt

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  # You can add your own code here
  #

hexasc:
	andi    $v0, $a0, 0x0f  # v0 = a0 & 0x0f (take only 4 bits)
	slti    $t0, $v0, 10    # t0 = (v0 < 10) ? 1 : 0
	beq     $t0, $0, alpha  # if t0 = 0 (i.e v0 >= 10) goto alpha
	nop
	addi    $v0, $v0, 48    # v0 = v0 + 48
	j ret                   # return
	nop
  alpha:
        addi $v0, $v0, 55       # v0 = v0 + 55
  ret:
  	jr $ra                  # return
  	nop

delay:
	slti $t0, $a0, 1
	beq $t0, 1, done              # while (ms > 0)
	nop
	addi $a0, $a0, -1             # ms = ms - 1
	
	li $t0, 20000                   # for i = 4711..0
  reloop:                               # {
	nop                           # 
	addi $t0, $t0, -1
	bne $t0, $0, reloop           # }
	nop
	
	j delay
	nop
  done:
	jr $ra
	nop

quit:
	nop

time2string:                    # a0: memory destination
                                # a1: four NBCD digits in lo
    PUSH $ra
    PUSH $s0
    
    move $s0, $a0           # grab destination address
    
    andi $a0, $a1, 0xf000   # grab/mask first digit
    srl $a0, $a0, 12        # ... and shift it to the bottom 4 bits
    jal hexasc              # call hexasc with arg in a0
    nop
    sb $v0, 0($s0)          # write resulting character
    
    andi $a0, $a1, 0x0f00   # grab/mask second digit
    srl $a0, $a0, 8         # ... and shift it to the bottom 4 bits
    jal hexasc              # call hexasc with arg in a0
    nop
    sb $v0, 1($s0)          # write resulting character
    
    li $t0, 0x3a            # load colon character
    sb $t0, 2($s0)          # store colon character
    
    andi $a0, $a1, 0x00f0   # grab/mask third digit
    srl $a0, $a0, 4         # ... and shift it to the bottom 4 bits
    jal hexasc              # call hexasc with arg in a0
    nop
    sb $v0, 3($s0)          # write resulting character
    
    andi $a0, $a1, 0x000f   # grab/mask fourth digit
    jal hexasc              # call hexasc with arg in a0
    nop
    sb $v0, 4($s0)          # write resulting character
    
    sb $0, 5($s0)           # terminate string with null
    
    POP $s0
    POP $ra
    jr $ra
    nop

enable_interrupt:
    ei
    jr $ra
