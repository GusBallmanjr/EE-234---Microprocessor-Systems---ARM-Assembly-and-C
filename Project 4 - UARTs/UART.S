.text
.global configure_UART1
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
.set LED_CTL, 0x41210000


UART1_reset:
    ldr r1, =UART1_CR
    mov r2, #3
    str r2, [r1]
    bx lr

configure_UART1:
    push {lr}
    bl UART1_reset
    pop {lr}

	ldr r1, =UART1_BAUD_RATE_D
    mov r2, #6
    str r2, [r1]

    ldr r1, =UART1_BAUDGEN
    mov r2, #0x7C
    str r2, [r1]

    ldr r1, =UART1_MR
    mov r2, #0x020
    str r2, [r1]

    bx lr

UART1_empty_tx:
    ldr r1, =UART1_SR
    ldr r2, [r1]
    and r2, r2, #16
    cmp r2, #16
    moveq r3, #1
    movne r3, #0
    bx lr

UART1_empty_rx:
    ldr r1, =UART1_SR
    ldr r2, [r1]
    and r2, r2, #0x2
    cmp r2, #0x2
    moveq r3, #0
    movne r3, #1
    bx lr

UART1_putC:
	tx_empty_loop:
    push {lr}
    bl UART1_empty_tx
    pop {lr}

    cmp r3, #1
    beq tx_empty_loop

    ldr r1, =UART1_FIFO
    strb r0, [r1]
    bx lr

UART1_getC:
	rx_empty_loop:
    push {lr}
    bl UART1_empty_rx
    pop {lr}

    cmp r3, #1
    beq rx_empty_loop

    ldr r1, =UART1_FIFO
    ldrb r0, [r1]
    bx lr

UART1_putS:
  @ ldr r4, =UART1_data
  mov r4, r0
  mov r5, #128
  ldrb r0, [r4]

putS_loop:
    push {r4, r5, lr}
    bl UART1_putC
    pop {r4, r5, lr}

    ldrb r0, [r4, #1]!
    cmp r0, #0
    beq putS_done

    sub r5, r5, #1
    cmp r5, #0
    bne putS_loop

putS_done:
    mov r1, #0x00
    bx lr

UART1_getS:
    @ ldr r4, =UART1_data
	mov r4, r0
    mov r5, #128

getS_loop:
    push {r4, r5, lr}
    bl UART1_getC
    pop {r4, r5, lr}

    strb r0, [r4]
    cmp r0, #0x00
    beq endLoop
    add r4, #1

    sub r5, r5, #1
    cmp r5, #0
    beq endLoop
    b getS_loop

endLoop:
    bx lr

end:

.end
