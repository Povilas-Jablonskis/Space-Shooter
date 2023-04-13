#ifndef C_PlayerLivesAndScore_hpp
#define C_PlayerLivesAndScore_hpp

#include "Component.hpp"

#include <string>

class C_PlayerLivesAndScore final : public Component
{
public:
	explicit C_PlayerLivesAndScore(Object*);

	void update(float) override;

	void setLivesIcon(const std::string&);

private:
	bool m_needToUpdateScoreAndHealth{true};
	int m_playerScore{0};
	int m_playerLives{3};
	std::string m_livesIcon{};
};

#endif
