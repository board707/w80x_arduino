/**
 * @file Stream.h 
 *
 * @brief   Stream Module
 *
 * @author Huang Leilei
 *
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 */
#ifndef _STREAM_H_
#define _STREAM_H_
//#include "Arduino.h"
#include <Common.h>
#include "debug.h"

#include "Print.h"
#include "WString.h"

class Stream : public Print
{
protected:
    unsigned long _timeout;      // number of milliseconds to wait for the next char before aborting timed read
    unsigned long _startMillis;  // used for timeout measurement
    int timedRead();    // private method to read stream with timeout
    int timedPeek();    // private method to peek stream with timeout
    int peekNextDigit(); // returns the next numeric digit in the stream or -1 if timeout

public:

/**
 * @defgroup Stream Stream
 * @brief Stream
 */

/**
 * @addtogroup Stream
 * @{
 */

/**
 * @defgroup Stream Stream
 * @brief Stream
 */

/**
 * @addtogroup Stream
 * @{
 */

    /**
     * @brief       available() gets the number of bytes available in the
     *              stream. This is only for bytes that have already arrived.
     *
     *              This function is part of the Stream class, and is called
     *              by any class that inherits from it (Wire, Serial, etc).
     *              See the Stream class main page for more information.
     *
     * @param[in] None
     *
     * @return 		stream : an instance of a class that inherits from Stream 
     * 
     * @note 
     */ 
    virtual int available() = 0;
    /**
     * @brief       read() reads characters from an incoming stream to the buffer.
     *
     *              This function is part of the Stream class, and is called
     *              by any class that inherits from it (Wire, Serial, etc).
     *              See the Stream class main page for more information.
     * @param[in] None
     *
     * @return 		the first byte of incoming data available (or -1 if no data is available) 
     * 
     * @note 
     */ 
    virtual int read() = 0;
    /**
     * @brief       Read a byte from the file without advancing to the 
     *              next one. That is, successive calls to peek() will
     *              return the same value, as will the next call to read().
     *
     *              This function is part of the Stream class, and is 
     *              called by any class that inherits from it (Wire, 
     *              Serial, etc). See the Stream class main page for
     *              more information.
     *
     * @param[in] None
     *
     * @return 		None 
     * 
     * @note 
     */ 
    virtual int peek() = 0;

    Stream() {
        _timeout = 1000;
    }

// parsing methods

    /**
     * @brief       setTimeout() sets the maximum milliseconds
     *              to wait for stream data, it defaults to 1000
     *              milliseconds. This function is part of the 
     *              Stream class, and is called by any class that
     *              inherits from it (Wire, Serial, etc). See the
     *              Stream class main page for more information.
     *
     * @param[in] timeout : timeout duration in milliseconds (long).
     *
     * @return 		None 
     * 
     * @note 
     */ 
    void setTimeout(unsigned long timeout);  // sets maximum milliseconds to wait for stream data, default is 1 second

    /**
     * @brief       find() reads data from the stream until the
     *              target string of given length is found The 
     *              function returns true if target string is 
     *              found, false if timed out.
     *
     *              This function is part of the Stream class, 
     *              and is called by any class that inherits from
     *              it (Wire, Serial, etc). See the Stream class 
     *              main page for more information.
     *
     * @param[in] target : the string to search for (char)
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool find(const char *target);   // reads data from the stream until the target string is found
    /**
     * @brief       find() reads data from the stream until the
     *              target string of given length is found The 
     *              function returns true if target string is 
     *              found, false if timed out.
     *
     *              This function is part of the Stream class, 
     *              and is called by any class that inherits from
     *              it (Wire, Serial, etc). See the Stream class 
     *              main page for more information.
     *
     * @param[in] target : the string to search for (char)
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool find(uint8_t *target) {
        return find((char *) target);
    }
    // returns true if target string is found, false if timed out (see setTimeout)

    /**
     * @brief       find() reads data from the stream until the
     *              target string of given length is found The 
     *              function returns true if target string is 
     *              found, false if timed out.
     *
     *              This function is part of the Stream class, 
     *              and is called by any class that inherits from
     *              it (Wire, Serial, etc). See the Stream class 
     *              main page for more information.
     *
     * @param[in] target : the string to search for (char)
     * @param[in] length : Specify the length.
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool find(const char *target, size_t length);   // reads data from the stream until the target string of given length is found
    /**
     * @brief       find() reads data from the stream until the
     *              target string of given length is found The 
     *              function returns true if target string is 
     *              found, false if timed out.
     *
     *              This function is part of the Stream class, 
     *              and is called by any class that inherits from
     *              it (Wire, Serial, etc). See the Stream class 
     *              main page for more information.
     *
     * @param[in] target : the string to search for (char)
     * @param[in] length : Specify the length.
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool find(const uint8_t *target, size_t length) {
        return find((char *) target, length);
    }
    // returns true if target string is found, false if timed out

    /**
     * @brief       find() reads data from the stream until the
     *              target string of given length is found The 
     *              function returns true if target string is 
     *              found, false if timed out.
     *
     *              This function is part of the Stream class, 
     *              and is called by any class that inherits from
     *              it (Wire, Serial, etc). See the Stream class 
     *              main page for more information.
     *
     * @param[in] target : the string to search for (char)
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool find(char target) { return find (&target, 1); }

    /**
     * @brief       findUntil() reads data from the stream until the target
     *              string of given length or terminator string is found.
     *
     *              The function returns true if target string is found,
     *              false if timed out
     *
     *              This function is part of the Stream class, and is
     *              called by any class that inherits from it (Wire, Serial,
     *              etc). See the Stream class main page for more information.
     *
     * @param[in] target : the string to search for (char)
     * @param[in] terminal : the terminal string in the search (char)
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool findUntil(const char *target, const char *terminator);   // as find but search ends if the terminator string is found
    /**
     * @brief       findUntil() reads data from the stream until the target
     *              string of given length or terminator string is found.
     *
     *              The function returns true if target string is found,
     *              false if timed out
     *
     *              This function is part of the Stream class, and is
     *              called by any class that inherits from it (Wire, Serial,
     *              etc). See the Stream class main page for more information.
     *
     * @param[in] target : the string to search for (char)
     * @param[in] terminal : the terminal string in the search (char)
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool findUntil(const uint8_t *target, const char *terminator) {
        return findUntil((char *) target, terminator);
    }

    /**
     * @brief       findUntil() reads data from the stream until the target
     *              string of given length or terminator string is found.
     *
     *              The function returns true if target string is found,
     *              false if timed out
     *
     *              This function is part of the Stream class, and is
     *              called by any class that inherits from it (Wire, Serial,
     *              etc). See the Stream class main page for more information.
     *
     * @param[in] target : the string to search for (char)
     * @param[in] terminal : the terminal string in the search (char)
     * @param[in] terminate : Specify the terminate
     * @param[in] termLen : Specify the termLen
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool findUntil(const char *target, size_t targetLen, const char *terminate, size_t termLen);   // as above but search ends if the terminate string is found
    /**
     * @brief       findUntil() reads data from the stream until the target
     *              string of given length or terminator string is found.
     *
     *              The function returns true if target string is found,
     *              false if timed out
     *
     *              This function is part of the Stream class, and is
     *              called by any class that inherits from it (Wire, Serial,
     *              etc). See the Stream class main page for more information.
     *
     * @param[in] target : the string to search for (char)
     * @param[in] terminal : the terminal string in the search (char)
     * @param[in] terminate : Specify the terminate
     * @param[in] termLen : Specify the termLen
     *
     * @return 		boolean 
     * 
     * @note 
     */ 
    bool findUntil(const uint8_t *target, size_t targetLen, const char *terminate, size_t termLen) {
        return findUntil((char *) target, targetLen, terminate, termLen);
    }

    /**
     * @brief       parseInt() returns the first valid (long) integer number
     *              from the serial buffer. Characters that are not integers
     *              (or the minus sign) are skipped.
     * 
     *              In particular:
     *
     *                  -- Initial characters that are not digits or a minus
     *                     sign, are skipped;
     *                  -- Parsing stops when no characters have been read 
     *                     for a configurable time-out value, or a non-digit
     *                     is read;
     *                  -- If no valid digits were read when the time-out 
     *                     (see Stream.setTimeout()) occurs, 0 is returned;
     *
     *              This function is part of the Stream class, and is called
     *              by any class that inherits from it (Wire, Serial, etc). 
     *              See the Stream class main page for more information.
     *
     * @param[in] None
     *
     * @return 		long 
     * 
     * @note 
     */ 
    long parseInt(); // returns the first valid (long) integer value from the current position.
    // initial characters that are not digits (or the minus sign) are skipped
    // integer is terminated by the first character that is not a digit.

    /**
     * @brief       parseFloat() returns the first valid floating point 
     *              number from the current position. Initial characters
     *              that are not digits (or the minus sign) are skipped.
     *              parseFloat() is terminated by the first character that
     *              is not a floating point number.
     *
     *              This function is part of the Stream class, and is 
     *              called by any class that inherits from it (Wire, Serial,
     *              etc). See the Stream class main page for more information.
     *
     * @param[in] None
     *
     * @return 		float 
     * 
     * @note 
     */ 
    float parseFloat();               // float version of parseInt

    /**
     * @brief       readBytes() read characters from a stream into a buffer.
     *              The function terminates if the determined length has been
     *              read, or it times out (see setTimeout()).
     *
     *              readBytes() returns the number of bytes placed in the 
     *              buffer. A 0 means no valid data was found.
     *
     *              This function is part of the Stream class, and is called
     *              by any class that inherits from it (Wire, Serial, etc).
     *              See the Stream class main page for more information.
     *
     * @param[in] buffer: the buffer to store the bytes in (char[] or byte[])
     * @param[in]length : the number of bytes to read (int)
     *
     * @return 		The number of bytes placed in the buffer 
     * 
     * @note 
     */ 
    virtual size_t readBytes(char *buffer, size_t length); // read chars from stream into buffer
    /**
     * @brief       readBytes() read characters from a stream into a buffer.
     *              The function terminates if the determined length has been
     *              read, or it times out (see setTimeout()).
     *
     *              readBytes() returns the number of bytes placed in the 
     *              buffer. A 0 means no valid data was found.
     *
     *              This function is part of the Stream class, and is called
     *              by any class that inherits from it (Wire, Serial, etc).
     *              See the Stream class main page for more information.
     *
     * @param[in] buffer: the buffer to store the bytes in (char[] or byte[])
     * @param[in]length : the number of bytes to read (int)
     *
     * @return 		The number of bytes placed in the buffer 
     * 
     * @note 
     */ 
    virtual size_t readBytes(uint8_t *buffer, size_t length) {
        return readBytes((char *) buffer, length);
    }
    // terminates if length characters have been read or timeout (see setTimeout)
    // returns the number of characters placed in the buffer (0 means no valid data found)

    /**
     * @brief       readBytesUntil() reads characters from a stream into a 
     *              buffer. The function terminates if the terminator 
     *              character is detected, the determined length has been
     *              read, or it times out (see setTimeout()).
     * 
     *              readBytesUntil() returns the number of bytes placed in
     *              the buffer. A 0 means no valid data was found.
     * 
     *              This function is part of the Stream class, and is called
     *              by any class that inherits from it (Wire, Serial, etc).
     *              See the Stream class main page for more information.
     * 
     * @param[in] character : the character to search for (char)
     * @param[in] buffer: the buffer to store the bytes in (char[] or byte[])
     * @param[in] length : the number of bytes to read (int)
     *
     * @return 		The number of bytes placed in the buffer 
     * 
     * @note 
     */ 
    size_t readBytesUntil(char terminator, char *buffer, size_t length); // as readBytes with terminator character

   /**
    * @brief       readBytesUntil() reads characters from a stream into a 
    *              buffer. The function terminates if the terminator 
    *              character is detected, the determined length has been
    *              read, or it times out (see setTimeout()).
    * 
    *              readBytesUntil() returns the number of bytes placed in
    *              the buffer. A 0 means no valid data was found.
    * 
    *              This function is part of the Stream class, and is called
    *              by any class that inherits from it (Wire, Serial, etc).
    *              See the Stream class main page for more information.
    * 
    * @param[in] character : the character to search for (char)
    * @param[in] buffer: the buffer to store the bytes in (char[] or byte[])
    * @param[in] length : the number of bytes to read (int)
    *
    * @return      The number of bytes placed in the buffer 
    * 
    * @note 
    */ 
   size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length) {
        return readBytesUntil(terminator, (char *) buffer, length);
    }
    // terminates if length characters have been read, timeout, or if the terminator character  detected
    // returns the number of characters placed in the buffer (0 means no valid data found)

    // Arduino String functions to be added here
    /**
     * @brief       readString() reads characters from a stream into a string.
     *              The function terminates if it times out (see setTimeout()).
     * 
     *              This function is part of the Stream class, and is called by
     *              any class that inherits from it (Wire, Serial, etc). See 
     *              the Stream class main page for more information.
     * 
     * @param[in] none
     *
     * @return 		A string read from a stream 
     * 
     * @note 
     */ 
    String readString();
    /**
     * @brief       readStringUntil() reads characters from a stream into a 
     *              string. The function terminates if the terminator character
     *              is detected or it times out (see setTimeout()).
     *
     *              This function is part of the Stream class, and is called
     *              by any class that inherits from it (Wire, Serial, etc).
     *              See the Stream class main page for more information.
     *
     * @param[in] terminator : the character to search for (char)
     *
     * @param[out]
     *
     * @return 		The entire string read from a stream, until the terminator character is detected 
     * 
     * @note 
     */ 
    String readStringUntil(char terminator);

/**
 * @}
 */

/**
 * @}
 */
protected:
    long parseInt(char skipChar); // as above but the given skipChar is ignored
    // as above but the given skipChar is ignored
    // this allows format characters (typically commas) in values to be ignored

    float parseFloat(char skipChar);  // as above but the given skipChar is ignored
};

#endif//_STREAM_H_