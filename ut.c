// -------------- UTILITIES --------------

// REVERSE A STRING
void reverse(char *str, int length)
{
  int start = 0;
  int end = length - 1;
  while (start < end)
  {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}

// convert int to string
char *itoa(int num, char *str)
{
  int i = 0;
  int isNegative = 0;

  // Handle 0 explicitly, otherwise empty string is printed
  if (num == 0)
  {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  // Handle negative numbers only if num is negative
  if (num < 0)
  {
    isNegative = 1;
    num = -num;
  }

  // Process individual digits
  while (num != 0)
  {
    int rem = num % 10;
    str[i++] = rem + '0';
    num = num / 10;
  }

  // Append negative sign for negative numbers
  if (isNegative)
  {
    str[i++] = '-';
  }

  str[i] = '\0'; // Null-terminate string

  // Reverse the string
  reverse(str, i);

  return str;
}

typedef unsigned int size_t;

// Safe strcpy
void stringcpy(char *dst, const char *src, size_t dstSize)
{
  size_t i = 0;

  while (i < dstSize - 1 && src[i] != '\0')
  {
    dst[i] = src[i];
    i++;
  }

  dst[i] = '\0';
}

// Safe strcat
void stringcat(char *dst, const char *src, size_t dstSize)
{
  size_t dstLen = 0;
  while (dst[dstLen] != '\0')
  {
    dstLen++;
  }

  size_t i;

  for (i = 0; i < dstSize - dstLen - 1 && src[i] != '\0'; ++i)
  {
    dst[dstLen + i] = src[i];
  }

  dst[dstLen + i] = '\0';
}

// Compute the length of a string
size_t stringlen(const char *s)
{
  size_t len = 0;
  while (*s++)
    len++;
  return len;
}
