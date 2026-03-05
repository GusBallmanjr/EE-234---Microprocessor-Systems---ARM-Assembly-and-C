.text
.global main

.set SVN_SEG_CNTL,      0x43C10000
.set SVN_SEG_DATA_BASE, 0x43C10004
.set SW_DATA,           0x41220000
.set BTN_DATA,          0x41200000

main:
    mov r7, #0              @ BCD counter
    b Seven_Segment


@ Seven-segment display
Seven_Segment:
    ldr r6, =SVN_SEG_CNTL
    mov r5, #1              @ EN=1, M=0 (BCD mode)
    str r5, [r6]

    ldr r6, =SVN_SEG_DATA_BASE

loop:
    @ BCD counter
    add r7, r7, #1
    cmp r7, #0x3E8
    moveq r7, #0

    mov r1, r7
    mov r4, #0              @ accumulator
    mov r0, #12             @ 12 shifts for 12-bit number

    mov r11, lr
    push {r5, r6, r11}
    bl  Double_Dabble
    pop {r5, r6, r11}
    mov lr, r11

    @ r4 now contains BCD
    mov r3, r4

    mov r1, #0              @ accumulator for display

    @ digit1 = BCD[0]
    mov r4, r3
    and r4, r4, #0xF
    add r1, r1, r4

    @ digit2 = BCD[1]
    mov r4, r3
    lsr r4, r4, #4
    and r4, r4, #0xF
    lsl r4, r4, #8
    add r1, r1, r4

    @ digit3 = BCD[2]
    mov r4, r3
    lsr r4, r4, #8
    and r4, r4, #0xF
    lsl r4, r4, #16
    add r1, r1, r4

    @ digit4 = BCD[3]
    mov r4, r3
    lsr r4, r4, #12
    and r4, r4, #0xF
    lsl r4, r4, #24
    add r1, r1, r4

    @ write all 4 digits
    str r1, [r6]

    mov r9, #0x20000000
delay:
    subs r9, #1
    bne delay

    b loop

Double_Dabble:
    mov r4, #0
    mov r3, #0
    mov r2, #0
    mov r0, r1

dd_loop:
    cmp r0, #10
    blt dd_done

    subs r0, r0, #10
    add r2, r2, #1
    cmp r2, #10
    blt dd_loop

    mov r2, #0
    add r3, r3, #1
    cmp r3, #10
    blt dd_loop

    mov r3, #0
    add r4, r4, #1
    b dd_loop

dd_done:
    lsl r4, r4, #12
    lsl r3, r3, #8
    lsl r2, r2, #4
    add r4, r4, r3
    add r4, r4, r2
    add r4, r4, r0
    bx lr

