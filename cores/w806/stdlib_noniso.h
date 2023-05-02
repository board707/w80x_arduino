#ifndef _STDLIB_NONISO_H_
#define _STDLIB_NONISO_H_

#ifdef __cplusplus
extern "C"{
#endif
// Not implemented yet
int atoi(const char *s);

//long atol(const char* s);

//double atof(const char* s);

char* itoa (int val, char *s, int radix);

char* ltoa (long val, char *s, int radix);

char* utoa (unsigned int val, char *s, int radix);

char* ultoa (unsigned long val, char *s, int radix);
 
char* dtostrf (double val, signed char width, unsigned char prec, char *s);

void reverse(char* begin, char* end);

#ifdef __cplusplus
} // extern "C"
#endif


#endif//_STDLIB_NONISO_H_