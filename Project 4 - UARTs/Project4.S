.global main


.data
.comm UART1_data, 128

stored_data:
.byte 0, 1, 2, 3

.text

.set A_ASCII_CONST, 63

main:
    bl UART1_configure
    mov r0, A_ASCII_CONST
    bl UART1_putC
    b.
