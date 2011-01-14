#ifndef HSVKEYER_H
#define HSVKEYER_H

#include "keyer.h"

class HSVKeyer : public Keyer
{
public:
    HSVKeyer();

protected:
    void computeMatte();
};

#endif // HSVKEYER_H
