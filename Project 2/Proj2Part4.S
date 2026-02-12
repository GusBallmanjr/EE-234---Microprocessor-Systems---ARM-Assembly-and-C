.text
.global main

.set PWM_BASE, 0x43C00000
.set SW_DATA, 0x41220000

main:
    ldr r4, =SW_DATA
    ldr r1, [r4]
    bl pwm_window
    b main

delay:
	subs r0,r0,#1
	bne delay
	bx lr

pwm_window:
    push {r4,r5,r6,r7,r8,r9,r10,r11,lr}

    mov r0, r1, lsr #9 @ shift to get top 3 bits ( 3 leftmost switches)
    and r0, r0, #0x7 @ clear other switch bits
    cmp r0, #5

    mov r2, r0
    mov r0, #0

    disable_loop:
        bl pwm_disable
        add r0, r0, #1 @ incrament
        cmp r0, #6
        blt disable_loop

    mov r0, r2
    bl pwm_width
    and r1, r1, #0xFF @ clear any bits other than lower 8
    bl pwm_period
    bl pwm_enable

    pop {r4,r5,r6,r7,r8,r9,r10,r11,pc}

pwm_enable:
    ldr r1, =PWM_BASE
    cmp r0, #0
    blo leave_enable @ branch if lower
    cmp r0, #5
    bhi leave_enable @ branch if higher

    mov r3, r0
    mov r4, #10
    mul r3, r3, r4
    ldr r0, [r1, r3]

    orr r0,r0, #1
    str r0, [r1]
    leave_enable:
    bx lr

pwm_disable:
    ldr r1, =PWM_BASE
    cmp r0, #0
    blo leave_disable @ branch if lower
    cmp r0, #5
    bhi leave_disable @ branch if higher

    mov r3, r0
    mov r4, #10
    mul r3, r3, r4
    mul r3, r4
    ldr r0, [r1, r3]

    orr r0,r0, #0
    str r0, [r1]
    leave_disable:
    bx lr

pwm_width:
    ldr r1, =PWM_BASE
    cmp r0, #0
    blo leave_width @ branch if lower
    cmp r0, #5
    bhi leave_width @ branch if higher

    mov r3, r0
    mov r4, #10
    mul r3, r3, r4
    add r1, r1, r3
    ldr r0, [r1, #8]

    mov r0, #10000
    str r0, [r1, #8]
    leave_width:
    bx lr

pwm_period:
    ldr r2, =PWM_BASE
    cmp r0, #0
    blo leave_period @ branch if lower
    cmp r0, #5
    bhi leave_period @ branch if higher

    mov r3, r0
     mov r4, #10
    mul r3, r3, r4

    add r2, r2, r3
    ldr r0, [r2, #8]

    mov r0, r1
    str r0, [r2, #8]
    leave_period:
    bx lr

.end



