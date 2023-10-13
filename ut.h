#ifndef UT_H
#define UT_H

// utilities header file
char *itoa(int num, char *str);
typedef unsigned int size_t;
void stringcpy(char *dst, const char *src, size_t dstSize);
void stringcat(char *dst, const char *src, size_t dstSize);
void appendIntToString(char *str, int num);
size_t stringlen(const char *s);

#endif // UT_H