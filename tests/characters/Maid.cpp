#include "Maid.h"
#include <format>

using namespace std::string_literals;

Maid::Maid() : Character{}, m_master{ Character{} }
{
   
}

Maid::Maid(std::string name, unsigned int age, const Character& master) : 
    Character{ name, age, Gender::WOMAN, "maid" }, m_master{ master }
{
}

Maid::~Maid()
{
}

const Character& Maid::master() noexcept
{
    return m_master;
}

std::string Maid::introduce() const noexcept
{
    return std::format("Je m'appelle {:s}, j'ai {:d} ans. Je suis la servante de {:s} {:s}", 
        m_name, m_age, m_master.gender() == Gender::WOMAN ? "madame"s : "monsieur"s, m_master.name());
}
