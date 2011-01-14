#ifndef DIFFERENCEKEYER_H
#define DIFFERENCEKEYER_H

#include "keyer.h"

class DifferenceKeyer : public Keyer
{
public:
    DifferenceKeyer();

protected:
    void computeMatte();
    void setIndexes(int& a, int& b, int& c);
};

#endif // DIFFERENCEKEYER_H
