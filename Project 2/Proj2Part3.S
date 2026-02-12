.text
.global main

.set LED_CTL, 0x41210000
.set SW_DATA, 0x41220000

main:
    mov r2, =SW_DATA
    mov r0, [r2]
    mov r0, r0, lsl #24
    mov r1, #1000
    main_loop:
    bl pwm_window
    b main_loop

delay:
	subs r0,r0,#1
	bne delay
	bx lr


led0_on:
    ldr r1, =LED_CTL
    ldr r0, [r1]
    orr r0,r0,#1
    str r0,[r1]
    bx lr

led0_off:
    ldr r1, =LED_CTL
    ldr r0, [r1]
    orr r0,r0,#0
    str r0,[r1]
    bx lr

pwm_window:
    push {r4,r5,r6,r7,r8,r9,r10,r11, lr}
    mov r4, r0
    mov r5, r1
    bl led0_on
    mov r0, r5
    bl delay
    bl led0_off
    sub r0, r4, r5
    bl delay

    pop {r4,r5,r6,r7,r8,r9,r10,r11, pc}

.end