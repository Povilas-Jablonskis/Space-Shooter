#ifndef C_PlayerLivesAndScore_hpp
#define C_PlayerLivesAndScore_hpp

#include "Component.hpp"

#include <memory>
#include <string>

class C_PlayerLivesAndScore : public Component
{
public:
    C_PlayerLivesAndScore(Object*);

    void update(float) override;

    void updatePlayerLivesAndScore();
    void setLivesIcon(const std::string&);
private:
    bool m_needToUpdateScoreAndHealth{ true };
    int m_playerScore{0};
    int m_playerLives{3};
    std::string m_livesIcon{};
};

#endif