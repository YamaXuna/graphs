#pragma once
#include "Character.h"
class Maid :
    public Character
{
    private:
        const Character& m_master;

    public:
        Maid();
        Maid(std::string name, unsigned int age, const Character &master);
        Maid(const Maid& m) = default;
        Maid(Maid&& m) = default;
        ~Maid();

        const Character& master()noexcept;

        virtual std::string introduce()const noexcept override;
};

