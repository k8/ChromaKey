#include "keyerfactory.h"

#include "hsvkeyer.h"
#include "differencekeyer.h"

KeyerFactory::KeyerFactory()
{
}

Keyer* KeyerFactory::createKeyer(KeyingParameters::KeyingAlgorithm keyingAlgorithm)
{
    Keyer* keyer = 0;
    switch (keyingAlgorithm)
    {
    case KeyingParameters::KA_HSV:
        keyer = new HSVKeyer();
        break;
    case KeyingParameters::KA_DM:
        keyer = new DifferenceKeyer();
        break;
    }
    return keyer;
}
