
// delay by variable count
void busyDelay(unsigned int count)
{
    while (count--)
    {
        __asm__ volatile("nop"); // NOP: No Operation
    }
}