/**
 * Data.h
 *
 * Copyright(c) 2018
 * Cristiano Arbex Valle
 * All rights reserved.
 */

#ifndef DATA_H
#define DATA_H

#include "Util.h"

/**
 * Data data
 */

class DefaultObject {

public:

    DefaultObject() {};
    virtual ~DefaultObject() {};
};

class Data {

    protected:
        
        int debug;

    public:

        Data();
        virtual ~Data();

        virtual void readData() {}
        virtual void print() {}

        virtual void readObjectData() {}

        DefaultObject* request;
};

#endif
