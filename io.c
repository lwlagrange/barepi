#include "io.h"
#include "ut.h"
#include "tm.h"
// -------------- GPIO --------------
// GPIO register offsets
#define GPFSEL0 (PERIPHERAL_BASE + 0x200000)   // GPIO Function Select
#define GPSET0 (PERIPHERAL_BASE + 0x20001C)    // GPIO Pin Output Set
#define GPCLR0 (PERIPHERAL_BASE + 0x200028)    // GPIO Pin Output Clear
#define GPSET1 (PERIPHERAL_BASE + 0x200020)    // GPIO Pin Output Set 1
#define GPCLR1 (PERIPHERAL_BASE + 0x20002C)    // GPIO Pin Output Clear 1
#define GPPUPPDN0 (PERIPHERAL_BASE + 0x2000E4) // GPIO Pin Pull-up/down Enable Enable for pins 1-15
#define GPPUPPDN1 (PERIPHERAL_BASE + 0x2000E8) // GPIO Pin Pull-up/down Enable for pins 16-31
#define GPIO_MAX_PIN 53                        // Maximum GPIO Pin number

// Write a value to a memory-mapped IO address
void writeMMIO(unsigned long address, unsigned int value)
{
  *(volatile unsigned int *)address = value;
}

// Read a value from a memory-mapped IO address
unsigned int readMMIO(unsigned long address)
{
  return *(volatile unsigned int *)address;
}

// Configure GPIO pins: set functions, pull-ups, etc.
unsigned int configGPIO(unsigned int pinNumber, unsigned int pinFunc, unsigned int base, unsigned int fieldSize, unsigned int maxPin)
{
  unsigned int fieldMask = (1 << fieldSize) - 1;

  if (pinNumber > maxPin)
    return 0;
  if (pinFunc > fieldMask)
    return 0;

  unsigned int numFields = 32 / fieldSize;
  unsigned int reg = base + ((pinNumber / numFields) * 4);
  unsigned int shift = (pinNumber % numFields) * fieldSize;

  unsigned int currVal = readMMIO(reg);
  currVal &= ~(fieldMask << shift);
  currVal |= pinFunc << shift;
  writeMMIO(reg, currVal);

  return 1;
}

// Set a GPIO pin's function (e.g., input, output, alt function)
void setGPIOFunction(unsigned int pinNumber, int functionType)
{
  configGPIO(pinNumber, functionType, GPFSEL0, 3, GPIO_MAX_PIN);
}

// Configure the pull up/down setting of a GPIO pin
void setGPIOPull(unsigned int pinNumber, int pullType)
{
  unsigned int reg;

  // Check for GPIO pin number to decide which register to use
  if (pinNumber < 16)
  {
    reg = GPPUPPDN0;
  }
  else if (pinNumber < 32)
  {
    reg = GPPUPPDN1;
    pinNumber -= 16;
  }
  else
  {
    return; // Unsupported pin number for pull configuration
  }

  configGPIO(pinNumber, pullType, reg, 2, GPIO_MAX_PIN);
}

// Function to read the GPIO value
int readGPIOValue(unsigned int pinNumber)
{
  unsigned int reg = (pinNumber < 32) ? (PERIPHERAL_BASE + 0x200034) : (PERIPHERAL_BASE + 0x200038); // GPLEV0 or GPLEV1
  return (readMMIO(reg) & (1 << pinNumber)) ? 1 : 0;
}

void setGPIOHigh(unsigned int pinNumber)
{
  if (pinNumber < 32)
  {
    writeMMIO(GPSET0, 1 << pinNumber);
  }
  else if (pinNumber <= GPIO_MAX_PIN)
  { // <= because the maximum pin number is inclusive
    writeMMIO(GPSET1, 1 << (pinNumber - 32));
  }
}

void setGPIOLow(unsigned int pinNumber)
{
  if (pinNumber < 32)
  {
    writeMMIO(GPCLR0, 1 << pinNumber);
  }
  else if (pinNumber <= GPIO_MAX_PIN)
  { // <= because the maximum pin number is inclusive
    writeMMIO(GPCLR1, 1 << (pinNumber - 32));
  }
}

void toggleGPIO(unsigned int pinNumber)
{
  int currentState = readGPIOValue(pinNumber);
  currentState ? setGPIOLow(pinNumber) : setGPIOHigh(pinNumber);
}

// Configure a GPIO pin to use its "Alternate 5" function
void useGPIOAsAlt5(unsigned int pinNumber)
{
  setGPIOPull(pinNumber, GPIO_PULL_NONE);
  setGPIOFunction(pinNumber, GPIO_FUNC_ALT5);
}

// Function to test the GPIO ports configures the GPIO pins as outputs and then cylces
char *testGPIO(void)
{
  static char statusChart[18][50]; // Static so memory persists outside function

  int gpioPins[] = {4, 27, 21, 13, 26, 23, 22, 12, 20, 19, 24, 25, 5, 6, 16, 17, 18};
  int pinCount = sizeof(gpioPins) / sizeof(gpioPins[0]);

  for (int i = 0; i < pinCount; i++)
  {
    char pinNumChar[4];
    itoa(gpioPins[i], pinNumChar);

    setGPIOPull(gpioPins[i], GPIO_PULL_LOW);
    busyDelay(1000000);
    setGPIOFunction(gpioPins[i], GPIO_FUNC_OUTPUT);
    busyDelay(1000000);
    setGPIOLow(gpioPins[i]);
    busyDelay(1000000);

    setGPIOHigh(gpioPins[i]);
    busyDelay(5000000);

    if (readGPIOValue(gpioPins[i]) == 1)
    {
      stringcpy(statusChart[i], "Write Success / ", sizeof(statusChart[i]));
    }
    else
    {
      stringcpy(statusChart[i], "Write Fail / ", sizeof(statusChart[i]));
    }

    setGPIOLow(gpioPins[i]);
    busyDelay(5000000);

    if (readGPIOValue(gpioPins[i]) == 0)
    {
      stringcat(statusChart[i], "Read Success", sizeof(statusChart[i]) - stringlen(statusChart[i]));
    }
    else
    {
      stringcat(statusChart[i], "Read Fail", sizeof(statusChart[i]) - stringlen(statusChart[i]));
    }
  }

  // Construct the chart as a string
  static char result[1000];
  stringcpy(result, "\nGPIO Testing Results:\nPin  | Status\n-----|-------------------------\n", sizeof(result));

  for (int i = 0; i < pinCount; i++)
  {
    char pinNumChar[4];
    itoa(gpioPins[i], pinNumChar);

    stringcat(result, pinNumChar, sizeof(result) - stringlen(result));
    stringcat(result, "  | ", sizeof(result) - stringlen(result));
    stringcat(result, statusChart[i], sizeof(result) - stringlen(result));
    stringcat(result, "\n", sizeof(result) - stringlen(result));
  }
  stringcat(result, "GPIO Testing Complete!", sizeof(result) - stringlen(result));

  return result;
}

// -------------- UART --------------

// Base address for AUX (Auxiliary) peripherals
#define AUX_BASE (PERIPHERAL_BASE + 0x215000)
// UART register offsets
#define AUX_ENABLES (AUX_BASE + 4)       // Control register for auxiliary peripherals
#define AUX_MU_IO_REG (AUX_BASE + 64)    // Mini UART I/O Data Register
#define AUX_MU_IER_REG (AUX_BASE + 68)   // Mini UART Interrupt Enable Register
#define AUX_MU_IIR_REG (AUX_BASE + 72)   // Mini UART Interrupt Identify Register
#define AUX_MU_LCR_REG (AUX_BASE + 76)   // Mini UART Line Control Register
#define AUX_MU_MCR_REG (AUX_BASE + 80)   // Mini UART Modem Control Register
#define AUX_MU_LSR_REG (AUX_BASE + 84)   // Mini UART Line Status Register
#define AUX_MU_CNTL_REG (AUX_BASE + 96)  // Mini UART Control Register
#define AUX_MU_BAUD_REG (AUX_BASE + 104) // Mini UART Baud Rate Register
#define AUX_UART_CLOCK 500000000         // Clock frequency for the UART
#define UART_MAX_QUEUE (16 * 1024)       // Maximum queue size for UART data buffering
#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK / (baud * 8)) - 1)

void initUART()
{
  writeMMIO(AUX_ENABLES, 1); // enable UART1
  writeMMIO(AUX_MU_IER_REG, 0);
  writeMMIO(AUX_MU_CNTL_REG, 0);
  writeMMIO(AUX_MU_LCR_REG, 3); // 8 bits
  writeMMIO(AUX_MU_MCR_REG, 0);
  writeMMIO(AUX_MU_IER_REG, 0);
  writeMMIO(AUX_MU_IIR_REG, 0xC6); // disable interrupts
  writeMMIO(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
  useGPIOAsAlt5(14);
  useGPIOAsAlt5(15);
  writeMMIO(AUX_MU_CNTL_REG, 3); // enable RX/TX
}

// Check if UART is ready to send a byte
unsigned int isUARTReadyToWrite()
{
  return readMMIO(AUX_MU_LSR_REG) & 0x20;
}

// Send a single byte via UART
void sendUARTByte(unsigned char byte)
{
  while (!isUARTReadyToWrite())
    ; // Wait until UART is ready to write
  writeMMIO(AUX_MU_IO_REG, (unsigned int)byte);
}

// Send a string via UART
void sendUARTString(char *str)
{
  while (*str)
  {
    if (*str == '\n')
      sendUARTByte('\r'); // Sending '\r' before '\n' for newlines
    sendUARTByte(*str);
    str++;
  }
}

void initUARTMessage(void)
{
  sendUARTString("BarePI 1.0 - Bare Metal OS For Raspberry Pi\n"); // welcome message
  sendUARTString(" Bare Metal Program Running!");
  sendUARTString(".");
  busyDelay(1000000);
  sendUARTString(".");
  busyDelay(1000000);
  sendUARTString(".");
  busyDelay(1000000);
  sendUARTString(".");
  busyDelay(1000000);
  sendUARTString(".");
  busyDelay(1000000);
  sendUARTString(".");
  busyDelay(1000000);
  sendUARTString(".");
  busyDelay(1000000);
  // sendUARTString("\033[2J\033[H"); // ANSI escape sequence that clears uart and moves cursor to top left
}