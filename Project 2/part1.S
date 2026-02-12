.text
.global main

.set LED_CTL, 0x41210000

main:
    bl led0_on
    bl led0_off
    bl led_toggle
    bl led_toggle

led0_off:
    ldr r1, =LED_CTL
    ldr r0, [r1]
    orr r0,r0,#0
    str r0,[r1]
    bx lr

led0_on:
    ldr r1, =LED_CTL
    ldr r0, [r1]
    orr r0,r0,#1
    str r0,[r1]
    bx lr

led0_toggle:
    ldr r1, =LED_CTL
    ldr r0, [r1]
    eor r0,r0,#1
    str r0,[r1]
    bx lr

.end
