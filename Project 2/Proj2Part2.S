.text
.global main

.set LED_CTL, 0x41210000

main:
    blinky_loop:
		bl led0_toggle
		ldr r0,=0x3F9C2E0 @ Divide 667 MHz clock to 10 Hz
		bl delay 
	b blinky_loop

delay:
	subs r0,r0,#1
	bne delay
	bx lr
    
led0_toggle:
    ldr r1, =LED_CTL
    ldr r0, [r1]
    eor r0,r0,#1
    str r0,[r1]
    bx lr

.end