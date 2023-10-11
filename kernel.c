#include "io.h"
#include "tm.h"

int main(void)
{
  uart_init();
  while (1)
  {
    uart_writeText("Hello world!\n");
    delay(5000000);
  }
}