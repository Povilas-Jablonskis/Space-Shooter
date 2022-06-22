#ifndef Bitmask_hpp
#define Bitmask_hpp

#include <stdint.h>

class Bitmask
{
public:
    Bitmask();

    Bitmask(uint32_t);

    void setMask(Bitmask&);
    uint32_t getMask() const;

    bool getBit(int) const;
    void setBit(int, bool);

    void setBit(int);
    void clearBit(int);

    void clear();

private:
    uint32_t bits;
};

#endif