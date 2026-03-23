.text
.global UART1_configure
.global UART1_putC
.global UART1_getC
.global UART1_putS
.global UART1_getS

.set UART1_BASE, 0xE0001000
.set UART1_SR, 0xE000102C
.set UART1_DATA, 0xE0001030

UART1_configure:
    ldr r0, =UART1_BASE

	mov r1, #3
    str r1, [r0] @ reset

    mov r1, #0x20      @ Normal Mode, 1 stop bit, no parity, 8 bit length
    str r1, [r0, #0x4]  @ Mode Register

	mov r1, #0x7C @ Baudgen
    str r1, [r0, #0x18]  @ Baudgen register

   	mov r1, #6 @ Baud_Div
    str r1, [r0, #0x34] @ Baud divider register

    mov r1, #0x28        @ TXRST | RXRST
    str r1, [r0]

    mov r1, #0x14        @ TXEN | RXEN
    str r1, [r0]

    bx lr

UART1_putC:
	push {r4,lr} 			@ backup the registers used
	and r4,r0,#0xFF 		@ save bottom byte R0 (input) to r4

check_full_loop:
	bl uart1_tx_empty 		@ check if uart1 tx has room
	cmp r0,#1 			@ check the return value
	beq check_full_loop		@ keep checking until room in FIFO
	ldr r1, =UART1_DATA
	str r4,[r1]			@write to tx fifo
	pop {r4,lr}			@restore registers
	bx lr

uart1_tx_empty:
	mov r0,#0	@init r0
	ldr r2, =UART1_SR
	ldr r1, [r2]	@get status flags
	ands r1, #16	@check if 'bit 4' is set
	movne r0,#1	@return 1 if set
	bx lr		@return


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

UART1_empty_rx:
    mov r0, #0
    ldr r1, =UART1_BASE
    add r1, #0x2C  @ offset to Status register
    ldr r2, [r1]

    ands r2, r2, #0x4 @ check bit 2 (Receiver full bit)
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
    ldr r0, [r2, #0x30] @ FIFO register offset
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
