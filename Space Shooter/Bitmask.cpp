#include "Bitmask.hpp"

Bitmask::Bitmask() : bits(0)
{
}

Bitmask::Bitmask(const uint32_t bits) : bits(bits)
{
}

void Bitmask::setMask(const Bitmask& other)
{
	bits = other.getMask();
}

uint32_t Bitmask::getMask() const
{
	return bits;
}

bool Bitmask::getBit(const int pos) const
{
	return (bits & (1 << pos)) != 0;
}

void Bitmask::setBit(const int pos, const bool on)
{
	if (on)
	{
		setBit(pos);
	}
	else
	{
		clearBit(pos);
	}
}

void Bitmask::setBit(const int pos)
{
	bits = bits | 1 << pos;
}

void Bitmask::clearBit(const int pos)
{
	bits = bits & ~(1 << pos);
}

void Bitmask::clear()
{
	bits = 0;
}
