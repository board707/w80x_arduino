/**
 * @file Printable.h
 *
 * @brief   IPAddress Module
 *
 * @author Huangleilei
 *
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 */
#ifndef Printable_h
#define Printable_h

#include <stdlib.h>

class Print;

class Printable
{
public:
    /**
     * @brief         This pure virtual function is used to called by print/println function.
     * @param[in] p Specify the Print object.
     *
     * @return 		bool 
     * 
     * @note The length of print successfully.
     */ 
    virtual size_t printTo(Print &p) const = 0;
};

#endif