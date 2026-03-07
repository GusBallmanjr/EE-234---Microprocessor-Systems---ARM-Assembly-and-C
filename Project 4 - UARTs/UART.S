.text
.global UART1_configure
.global UART1_putC
.global UART1_getC
.global UART1_putS
.global UART1_getS

.set UART1_BASE, 0xE0001000
.set BAUD_DIV, 6
.set BAUDGEN, 0x70

UART1_configure:
    push {lr}
    bl UART1_reset
    pop {lr}

    ldr r0, =UART1_BASE
    ldr r1, =BAUD_DIV
    str r1, [r0, 0x34] @ Baud divider register

    ldr r1, =BAUDGEN
    str r1, [r0, 0x18]  @ Baudgen register

    bic r1, r1
    mov r1, #0x020      @ Normal Mode, 1 stop bit, no parity, 8 bit length
    str r1, [r0, #0x4]  @ Mode Register
    bx lr

UART1_reset:
    ldr r1, =UART1_BASE

    mov r0, #3
    str r0, [r1]

    reset_loop:
        ldr r2, [r1]
        teq r2, r0
        bne reset_loop
    bx lr

UART1_putC:
    and r1, r0, #0xFF

    check_full_tx:
        push {r1, lr}
        bl UART1_empty_tx
        pop {r1, lr}
        cmp r0, #1          @ check return value
        beq check_full_tx

    ldr r2, =UART1_BASE
    add r2, #0x30 @ FIFO register offset
    str r1, [r2]
    bx lr


UART1_empty_tx:
    mov r0, #0
    ldr r1, =UART1_BASE
    add r1, #0x2C  @ offste to Status register
    ldr r2, [r1]

    ands r1, #0x10 @ check bit 4 (Transmitter full bit)
    movne r0, #1  @ return 1 if bit 4 is set

    bx lr

UART1_putS:
    ldr r4, =stored_data
    mov r5, #64

    putS_loop:
        push {r4, r5, lr}
        bl UART1_putC
        pop {r4, r5, lr}
        ldrb r1, [r4], #1
        sub r5, #1
        cmp r5, #1
        bne putS_loop

    mov r1, 0x00
    ldrb r1, [r4]
    bx lr

UART1_empty_rx:
    mov r0, #0
    ldr r1, =UART1_BASE
    add r1, #0x2C  @ offset to Status register
    ldr r2, [r1]

    ands r1, #0x4 @ check bit 2 (Receiver full bit)
    movne r0, #1  @ return 1 if bit 2 is set

    bx lr

UART1_getC:

    check_full_rx:
        push {lr}
        bl UART1_empty_rx
        pop {lr}
        cmp r0, #0          @ check return value
        beq check_full_rx

    ldr r2, =UART1_BASE
    ldr r1, [r2, #0x30] @ FIFO register offset
    bx lr

UART1_getS:
    ldr r4, =UART1_data
    mov r5, #128

    getS_loop:
        push {r4, r5, lr}
        bl UART1_getC
        pop {r4, r5, lr}
        strb r1, [r4], #1
        cmp r1, #0x00
        beq lr
        subs r5, #1
        beq lr
        b getS_loop
