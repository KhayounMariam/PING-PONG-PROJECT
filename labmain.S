# hub that calls our pong() in C
.globl main, handle_interrupt, pong
.text
main:
    jal pong
loop: j loop
handle_interrupt:
    jr ra
