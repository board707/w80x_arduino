/**
 * @file Print.h 
 *
 * @brief   Print Module
 *
 * @author Huang Leilei
 *
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 */
#ifndef _wiring_PRINT_H_
#define _wiring_PRINT_H_

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <Common.h>

#include "debug.h"
#include "WString.h"
#include "Printable.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Print
{
private:
    int write_error;
    size_t printNumber(unsigned long, uint8_t );
    size_t printFloat(double, uint8_t);

protected:
    void setWriteError( int err = 1 ) { write_error = err; }
    
public:
    Print() : write_error(0) {}
    /**
     * @brief       This function is used to get write error number.
     * @param[in] None
     *
     * @return      The write error set by the object.
     * 
     * @note 
     */ 
    int getWriteError() { return write_error; }
    /**
     * @brief       This function is used to clear write error number.
     * @param[in] None
     *
     * @return      None 
     * 
     * @note 
     */ 
    void clearWriteError() { setWriteError(0); }
    
    /**
     * @brief       This pure virtual function is used to define the operation that writes binary data.
     * @param[in] val: a value to send as a single byte
     * @return      The length of write successfully (1 byte).
     * 
     * @note 
     */ 
    virtual size_t write(uint8_t c) = 0;
    /**
     * @brief       This function is used to write buffer to the interface defined by the object.
     * @param[in] buffer Specify the buffer.
     * @param[in] size Specify the size.
     *
     * @return      The length of write successfully. 
     * 
     * @note 
     */ 
    virtual size_t write(const uint8_t *buffer, size_t size);
    /**
     * @brief       This function is used to write buffer to the interface defined by the object.
     * @param[in] str Specify the buffer of string.
     *
     * @return      The length of write successfully. 
     * 
     * @note 
     */ 
    size_t write(const char *str)
    {
        if (NULL == str) return 0;
        return write((const uint8_t *)str, strlen(str));
    }
    /**
     * @brief       This function is used to write buffer to the interface defined by the object.
     * @param[in] buffer Specify the buffer of string.
     * @param[in] size Specify the size.
     *
     * @return      The length of write successfully. 
     * 
     * @note 
     */ 
    size_t write(const char *buffer, size_t size)
    { return write((const uint8_t *)buffer, size); }

    /**
     * @brief       This function is used to print buffer to the interface defined by the object.
     * @param[in] s Specify the String.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print( const String & );
    size_t print(const __FlashStringHelper *rhs);
    /**
     * @brief       This function is used to print buffer to the interface defined by the object.
     * @param[in] s Specify the string buffer.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(const char []);
    /**
     * @brief       This function is used to print buffer to the interface defined by the object.
     * @param[in] c Specify the target - char.
     *
     * @return      The length of print successfully (1 byte). 
     * 
     * @note 
     */ 
    size_t print(char);
    /**
     * @brief       This function is used to print target to the interface defined by the object.
     * @param[in] b Specify the target - char.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(unsigned char, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object.
     * @param[in] n Specify the target - int.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(int, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object.
     * @param[in] n Specify the target - unsigned int.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(unsigned int, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object.
     * @param[in] n Specify the target - long.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(long, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object.
     * @param[in] n Specify the target - unsigned long.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(unsigned long, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object.
     * @param[in] n Specify the target - double.
     * @param[in] digits Specify the digits.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(double, int = BIN);
    /**
     * @brief       This function is used to print target to the interface defined by the object.
     * @param[in] x Specify the target - Printable.
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t print(const Printable &);
    
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] None
     *
     * @return      The length of print successfully. 
     * 
     * @note 
     */ 
    size_t println(void);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] s Specify the String.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(const String &s);
    size_t println(const __FlashStringHelper *rhs);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] c Specify the string buffer.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(const char []);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] c Specify the target - char.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(char);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] n Specify the target - int.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(unsigned char, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] num Specify the target - int.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(int, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] num Specify the target - unsigned int.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(unsigned int, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] num Specify the target - long.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(long, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] num Specify the target - unsigned long.
     * @param[in] base Specify the base.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(unsigned long, int = DEC);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] num Specify the target - double.
     * @param[in] digits Specify the digits.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(double, int = BIN);
    /**
     * @brief       This function is used to print target to the interface defined by the object with carriage ret ('\r') and new line ('\n').
     * @param[in] x Specify the target - Printable.
     *
     * @return      The length of print successfully.  
     * 
     * @note 
     */ 
    size_t println(const Printable &);
};

#endif
