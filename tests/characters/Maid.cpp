#include "Maid.h"

using namespace std::string_literals;

Maid::Maid() : Character{}, m_master{ std::move(Character{}) }
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
    std::string str = "Je m'appelle ";
    str += m_name;
    str += ", j'ai ";
    str += std::to_string(m_age);
    str += "ans. Je suis la servante de ";
    str += m_master.gender() == Gender::WOMAN ? "madame"s : "monsieur"s;
    str += m_master.name() + ".";
    return str;
}
