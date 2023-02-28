#include "Bitmask.hpp"

Bitmask::Bitmask() : m_bits(0)
{
}

Bitmask::Bitmask(const uint32_t bits) : m_bits(bits)
{
}

void Bitmask::setMask(const Bitmask& bitMask)
{
	m_bits = bitMask.getMask();
}

uint32_t Bitmask::getMask() const
{
	return m_bits;
}

bool Bitmask::getBit(const int pos) const
{
	return (m_bits & (1 << pos)) != 0;
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
	m_bits = m_bits | 1 << pos;
}

void Bitmask::clearBit(const int pos)
{
	m_bits = m_bits & ~(1 << pos);
}

void Bitmask::clear()
{
	m_bits = 0;
}
