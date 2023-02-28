#ifndef Bitmask_hpp
#define Bitmask_hpp

#include <stdint.h>

class Bitmask
{
public:
	Bitmask();

	explicit Bitmask(uint32_t);

	void setMask(const Bitmask&);
	[[nodiscard]] uint32_t getMask() const;

	[[nodiscard]] bool getBit(int) const;
	void setBit(int, bool);

	void setBit(int);
	void clearBit(int);

	void clear();

private:
	uint32_t m_bits;
};

#endif
