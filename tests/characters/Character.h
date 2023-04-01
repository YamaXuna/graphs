#pragma once

#include <string>
#include <ostream>
class Character
{
	public:
		enum class Gender{WOMAN, MAN};

	protected:
		std::string m_name;
		unsigned int m_age;
		Gender m_gender;

		std::string m_activity;

		
	public:
		Character();
		Character(std::string name, unsigned int age, Gender g, std::string activity);
		Character(const Character& c);
		Character(Character&& c) = default;

		virtual~Character();
		virtual std::string introduce()const noexcept;

		std::string name()const noexcept;
		Gender gender()const noexcept;


		friend Character operator+(Character c, int v);

		friend std::ostream& operator <<(std::ostream& os, const Character& c);
		friend void swap(Character& c1, Character& c2);
};




