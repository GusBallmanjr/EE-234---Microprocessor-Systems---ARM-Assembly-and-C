.text
.global main

.set SVN_SEG_CNTL,      0x43C10000
.set SVN_SEG_DATA_BASE, 0x43C10004
.set SW_DATA,           0x41220000
.set BTN_DATA,          0x41200000

main:
    b Seven_Segment

@ Read switch value
Switch_Data:
    ldr r2, =SW_DATA
    ldr r1, [r2]
    bx  lr

@ Read button value
Button_Data:
    ldr r2, =BTN_DATA
    ldr r1, [r2]
    and r1, r1, #0xF
    bx  lr

@ Seven segment display
Seven_Segment:
    ldr r6, =SVN_SEG_CNTL
    mov r5, #1
    str r5, [r6]

    ldr r6, =SVN_SEG_DATA_BASE

loop:
    @Read switches
    mov r11, lr
    push {r5, r6, r11}
    bl  Switch_Data
    pop {r5, r6, r11}
    mov lr, r11

    mov r3, r1
    mov r1, #0

    @ digit1 = switches[3:0]
    mov r4, r3
    and r4, r4, #0xF
    add r1, r1, r4

    @ digit2 = switches[7:4]
    mov r4, r3
    lsr r4, r4, #4
    and r4, r4, #0xF
    lsl r4, r4, #8
    add r1, r1, r4

    @ digit3 = switches[11:8]
    mov r4, r3
    lsr r4, r4, #8
    and r4, r4, #0xF
    lsl r4, r4, #16
    add r1, r1, r4

    mov r8, r1              @ save first 3 digits

    @ Read buttons
    mov r11, lr
    push {r5, r6, r11}
    bl  Button_Data
    pop {r5, r6, r11}
    mov lr, r11

    @ digit4 = buttons[3:0]
    and r1, r1, #0xF
    lsl r1, r1, #24
    add r8, r8, r1

    @ write all 4 digits
    str r8, [r6]

    b loop

