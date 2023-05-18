/**
 * @file Print.cpp
 *
 * @brief   Print Module
 *
 * @author Huang Leilei
 *
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 */

#include <stdio.h>
#include <math.h>

#include "Print.h"

/**
 * @brief       This function is used to write buffer to the interface defined by the object.
 * @param[in] buffer Specify the buffer.
 * @param[in] size Specify the size.
 *
 * @return      The length of write successfully (1 byte). 
 * 
 * @note 
 */ 
size_t Print::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while (size--)
    {
        n += write((const uint8_t)*buffer++);
    }
    return n;
}

size_t Print::printNumber(unsigned long n, uint8_t base)
{
    char buf[ 8 * sizeof(long) + 1 ];
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    if (base < BIN)
        base = DEC;

    do
    {
        unsigned long m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

size_t Print::printFloat(double number, uint8_t digits)
{
    size_t n = 0;

    if (isnan(number))
        return print("nan");
    else if (isinf(number))
        return print("inf");
    else if (number >  4294967040.0
        ||   number < -4294967040.0)
        return print("ovf");
    
    if (number < 0.0)
    {
        n += print('-');
        number = -number;
    }

    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; i++)
        rounding /= 10.0;

    unsigned long int_part = (unsigned long )number;
    double remainder = number - (double)int_part;
    n += print(int_part);

    if (digits > 0)
    {
        n += print('.');
    }

    while (digits-- > 0)
    {
        remainder *= 10.0;
        int toPrint = int(remainder);
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}

/**
 * @brief       This function is used to print buffer to the interface defined by the object.
 * @param[in] s Specify the String.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 

size_t Print::print(const String &s)
{
    return write(s.c_str(), s.length());
}

size_t Print::print(const __FlashStringHelper *rhs) {
    String a;
    a.concat(rhs);
    return print(a);
}
/**
 * @brief       This function is used to print buffer to the interface defined by the object.
 * @param[in] s Specify the string buffer.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(const char str[])
{
    //AR_DBG();
    return write(str);
}

/**
 * @brief       This function is used to print buffer to the interface defined by the object.
 * @param[in] c Specify the target - char.
 *
 * @return      The length of print successfully (1 byte). 
 * 
 * @note 
 */ 
size_t Print::print(char c)
{
    return write(c);
}

/**
 * @brief       This function is used to print target to the interface defined by the object.
 * @param[in] b Specify the target - char.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(unsigned char b, int base)
{
    return print((unsigned long)b, base);
}

/**
 * @brief       This function is used to print target to the interface defined by the object.
 * @param[in] n Specify the target - int.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(int n, int base)
{
    return print((long)n, base);
}

/**
 * @brief       This function is used to print target to the interface defined by the object.
 * @param[in] n Specify the target - unsigned int.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(unsigned int n, int base)
{
    return print((long)n, base);
}

/**
 * @brief       This function is used to print target to the interface defined by the object.
 * @param[in] n Specify the target - long.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(long n, int base)
{
    if (0 == base)
    {
        return write(n);
    } else if (DEC == base) {
        if (n < 0)
        {
            int t = print('-');
            n = -n;
            return printNumber(n, 10) + t;
        }
        return printNumber(n, 10);
    } else {
        return printNumber(n, base);
    }
}

/**
 * @brief       This function is used to print target to the interface defined by the object.
 * @param[in] n Specify the target - unsigned long.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(unsigned long n, int base)
{
    if (0 == base)
        return write(n);
    else
        return printNumber(n, base);
}

/**
 * @brief       This function is used to print target to the interface defined by the object.
 * @param[in] n Specify the target - double.
 * @param[in] digits Specify the digits.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(double n, int digits)
{
    return printFloat(n, digits);
}

/**
 * @brief       This function is used to print target to the interface defined by the object.
 * @param[in] x Specify the target - Printable.
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::print(const Printable &x)
{
    return x.printTo(*this);
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] None
 *
 * @return      The length of print successfully. 
 * 
 * @note 
 */ 
size_t Print::println()
{
    return write("\r\n");
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] c Specify the string buffer.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(const char c[])
{
    size_t n = print(c);
    n += println();
    return 0;
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] s Specify the String.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 

size_t Print::println(const String &s)
{
    size_t n = print(s);
    n += println();
    return n;
}

size_t Print::println(const __FlashStringHelper *rhs) 
{
    size_t n = print(rhs);
    n += println();
    return n;
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] c Specify the target - char.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(char c)
{
    size_t n = print(c);
    n += println();
    return n;
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] n Specify the target - int.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(unsigned char b, int base)
{
    size_t n = print(b, base);
    n += println();
    return n;
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] num Specify the target - int.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] num Specify the target - unsigned int.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(unsigned int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] num Specify the target - long.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}    

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] num Specify the target - unsigned long.
 * @param[in] base Specify the base.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(unsigned long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}  

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] num Specify the target - double.
 * @param[in] digits Specify the digits.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(double num, int digits)
{
    size_t n = print(num, digits);
    n += println();
    return n;
}

/**
 * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
 * @param[in] x Specify the target - Printable.
 *
 * @return      The length of print successfully.  
 * 
 * @note 
 */ 
size_t Print::println(const Printable &x)
{
    size_t n = print(x);
    n += println();
    return n;
}
