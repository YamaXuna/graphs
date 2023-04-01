#include "Character.h"
#include <print>
#include <iostream>

using namespace std::string_literals;

Character::Character() : m_name{ ""s }, m_age{ 0 }, m_gender{ Gender::WOMAN }, m_activity{ "aucune"s }
{

}

Character::Character(std::string name, unsigned int age, Gender g, std::string activity) : 
	m_name{ name }, m_age{ age }, m_gender{ g }, m_activity{ activity }
{
}

Character::Character(const Character& c) : Character(c.m_name, c.m_age, c.m_gender, c.m_activity)
{
	//std::cout << "-> " << c.m_name << '\n';
}

Character::~Character()
{
}

std::string Character::introduce()const noexcept
{
	return std::("Je suis {:s}, j'ai {:d} ans et je suis {:s}.",
		m_name, m_age, m_activity);
}

std::string Character::name() const noexcept
{
	return m_name;
}

Character::Gender Character::gender() const noexcept
{
	return m_gender;
}

Character operator+(Character c, int v)
{
	//std::cout << c.m_name << '\n';
	c.m_age += v;
	//std::cout << c.m_name << '\n';
	return c;
}

std::ostream& operator<<(std::ostream& os, const Character& c)
{
	return os << c.m_name;
}

void swap(Character& c1, Character& c2)
{

	using std::swap;
	swap(c1.m_name, c2.m_name);
	swap(c1.m_age, c2.m_age);
	swap(c1.m_activity, c2.m_activity);

	auto tmp = c1.m_gender;
	c1.m_gender = c2.m_gender;
	c2.m_gender = tmp;

	
}
