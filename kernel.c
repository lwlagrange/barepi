#include "io.h" //gpio init and control
#include "tm.h" //timer functions
#include "ut.h" //utilities for data manipulation

int main(void)
{

  initUART(); // initialize the serial bus

  while (1)
  {
    initUARTMessage(); // A UART message that indicates the program is running.
  }
}