.data
welcome: .asciiz "Programa de Raiz Quadrada - Newton Raphson\nDesenvolvedores: Matheus Darde Holdefer, Pedro Henrique Corral Livi, Eduardo Castilhos de Castilho\n"
prompt: .asciiz "Digite os parametros x e i para calcular sqrt(x, i) ou -1 para abortar a execucao\n"
response1: .asciiz "sqrt_nr("
response2: .asciiz ", "
response3: .asciiz ") = "
nl: .asciiz "\n"

.macro get_user_input(%register)
	li	$v0, 5
	syscall
	move	%register, $v0
	bltz	%register, exit
.end_macro

.macro print_string(%str)
	li	$v0, 4
	la	$a0, %str
	syscall
.end_macro

.macro print_int(%int)
	move	$a0, %int
	li	$v0, 1
	syscall
.end_macro

.macro print_result(%x, %i, %result)
	print_string(response1)
	print_int(%x)
	print_string(response2)
	print_int(%i)
	print_string(response3)
	print_int(%result)
	print_string(nl)
.end_macro

.macro function_prelude
	addi	$sp, $sp, -12
	sw	$ra, 0($sp)
	sw	$a0, 4($sp)
	sw	$a1, 8($sp)
.end_macro

.text
	.globl main

main:
print_string(welcome)

loop:
print_string(prompt)
get_user_input($t5)	# a0 -> X
get_user_input($t1)	# a1 -> I
move	$a0, $t5
move	$a1, $t1
jal	sqrt_nr
move	$t2, $v0
print_result($t5, $t1, $t2)
j	loop

sqrt_nr:
function_prelude
bne	$a1, $zero, recurs
li	$v0, 1
j	sqrt_exit

recurs:
addi	$a1, $a1, -1
jal	sqrt_nr
div	$a0, $v0
mflo	$t0
add	$v0, $v0, $t0
srl	$v0, $v0, 1
j	sqrt_exit

#Function Postlude
sqrt_exit:
lw	$ra, 0($sp)
lw	$a0, 4($sp)
lw	$a1, 8($sp)
addi	$sp, $sp, 12
jr	$ra

exit:
li	$v0, 10
syscall



