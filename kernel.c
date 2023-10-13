#include "io.h"
#include "tm.h"
#include "ut.h"

int main(void)
{

  initUART(); // initialize the serial bus

  while (1)
  {
    initUARTMessage(); // A UART message that indicates the program is running.
  }
}