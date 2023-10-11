
//delay by variable count
void delay(unsigned int count) {
    while(count--) {
        asm volatile("nop");  // NOP: No Operation
    }
}