.text
.global UART1_configure
.global UART1_putC
.global UART1_getC
.global UART1_putS
.global UART1_getS

.set UART1_CR, 0xE0001000
.set UART1_MR, 0xE0001004
.set UART1_BAUDGEN, 0xE0001018
.set UART1_SR, 0xE000102C
.set UART1_FIFO, 0xE0001030
.set UART1_BAUD_RATE_D, 0xE0001034

UART1_reset:
    ldr r1, =UART1_CR
    mov r2, #3
    str r2, [r1]

    mov r2, #0
    str r2, [r1]
    bx lr

configure_UART1:
    push {lr}
    bl UART1_reset
    pop {lr}

    ldr r1, =UART1_MR
    mov r2, #0x20
    str r2, [r1]

    ldr r1, =UART1_BAUDGEN
    ldr r2, #0x7C
    str r2, [r1]

    ldr r1, =UART1_BAUD_RATE_D
    mov r2, #6
    str r2, [r1]

    bx lx

UART1_empty_tx:
    ldr r1, =UART1_SR
    ldr r2, [r1]
    ands r2, #0x10 @ check bit 4
    movne r3, #0
    moveq r3, #1
    bx lr

UART1_empty_rx:
    ldr r1, =UART1_SR
    ldr r2, [r1]
    ands r2, #0x4 @ check bit 2
    movne r3, #0
    moveq r3, #1
    bx lr

UART1_putC:
    rx_empty_loop:
        push {r0, lr}
        bl UART1_empty_rx
        pop {r0, lr}

        cmp r3, #1 @ if r3 = 1, rx full
        beq rx_empty_loop
    ldr r1, =UART1_FIFO
    str r0, [r1]
    bx lr

UART1_getC:
    tx_empty_loop:
        push {lr}
        bl UART1_empty_tx
        pop {lr}

        cmp r3, #1 @ if r3 = 1, tx is full
        bne tx_empty_loop
    ldr r1, =UART1_FIFO
    ldr r0, [r1]
    bx lr


UART1_putS:
    ldr r4, =stored_data
    mov r5, #64
	ldrb r0, [r4], #1

    putS_loop:
        push {r4, r5, lr}
        bl UART1_putC
        pop {r4, r5, lr}
        ldrb r0, [r4], #1
        subs r5, r5, #1
        bne putS_loop

    mov r1, #0x00
    bx lr

UART1_getS:
    ldr r4, =UART1_data
    mov r5, #128

    getS_loop:
        push {r4, r5, lr}
        bl UART1_getC
        pop {r4, r5, lr}
        strb r0, [r4], #1
        cmp r0, #0x00
        beq end
        subs r5, r5, #1
        beq end
        b getS_loop
	end:
	bx lr
