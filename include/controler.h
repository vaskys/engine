#ifndef CONTROLER_H
#define CONTROLER_H


#include "config.h"

class Controler
{
    public:
        Controler();
        virtual ~Controler();

        virtual void init();
        virtual void update();
        virtual void clear();
};


#endif
