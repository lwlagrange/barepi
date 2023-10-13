#ifndef IO_H
#define IO_H

// Enumerations and function prototypes for GPIO and UART operations
// Define enumeration for GPIO function types
typedef enum
{
  GPIO_FUNC_INPUT = 0,  // Output mode for GPIO pin
  GPIO_FUNC_OUTPUT = 1, // Input mode for GPIO pin
  GPIO_FUNC_ALT5 = 2    // Alternate function 5 for GPIO pin
} GPIOFunctionType;

// Define enumeration for GPIO pull types
typedef enum
{
  GPIO_PULL_NONE = 0, // No pull-up or pull-down resistor enabled
  GPIO_PULL_LOW = 1,  // Enable pull-down resistor for GPIO pin
  GPIO_PULL_HIGH = 2  // Enable pull-up resistor for GPIO pin
} GPIOPullType;

// Function to initialize the UART (Universal Asynchronous Receiver-Transmitter)
void initUART();

// Function to send a string through UART
// buffer: Null-terminated string to be sent
void sendUARTString(char *buffer);

// Function to set a GPIO pin's function (e.g., input, output, alt function)
// pinNumber: Number of the GPIO pin to be configured
// functionType: Desired function for the GPIO pin, using values from GPIOFunctionType
void setGPIOFunction(unsigned int pinNumber, int functionType);

// Function to configure the pull-up/pull-down settings of a GPIO pin
// pinNumber: Number of the GPIO pin to be configured
// pullType: Desired pull-up/pull-down setting, using values from GPIOPullType
void setGPIOPull(unsigned int pinNumber, int pullType);

// Function to set a GPIO pin's output to low (0V)
// pinNumber: Number of the GPIO pin to be set to low
void setGPIOLow(unsigned int pinNumber);

// Function to set a GPIO pin's output to high (3.3V or 5V, depending on the board)
// pinNumber: Number of the GPIO pin to be set to high
void setGPIOHigh(unsigned int pinNumber);

// Function to toggle a GPIO pin's output between high and low
// pinNumber: Number of the GPIO pin to be toggled
void toggleGPIO(unsigned int pinNumber);

// Function to read the current state of a GPIO pin
// pinNumber: Number of the GPIO pin you need the sate of
int readGPIOValue(unsigned int pinNumber);

// Funtion to test the GPIO and braodcast it over UART
void initUARTMessage(void);

#endif // IO_H