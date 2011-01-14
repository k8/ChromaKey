#ifndef KEYERFACTORY_H
#define KEYERFACTORY_H

#include "keyer.h"
#include "keyingparameters.h"

class KeyerFactory
{
public:
    KeyerFactory();
    static Keyer* createKeyer(KeyingParameters::KeyingAlgorithm keyingAlgorithm);
};

#endif // KEYERFACTORY_H
