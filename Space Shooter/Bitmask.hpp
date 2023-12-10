#ifndef Bitmask_hpp
#define Bitmask_hpp

#include <stdint.h>

class Bitmask
{
public:
	Bitmask();

	explicit Bitmask(uint32_t bits);

	void setMask(const Bitmask& bitMask);
	[[nodiscard]] uint32_t getMask() const;

	[[nodiscard]] bool getBit(const int pos) const;
	void setBit(const int pos, const bool on);

	void setBit(const int pos);
	void clearBit(const int pos);

	void clear();

private:
	uint32_t m_bits;
};

#endif
