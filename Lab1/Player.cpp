#include "stdafx.h"
#include "Player.h"


Player::Player(ConsoleColor _fg, ConsoleColor _bg, char const * const _text, char const * const _name, int _left, int _top) : Sprite(_fg, _bg, _text, _name, _left, _top)
{
}

Player::Player()
{}

Player::~Player()
{

}

void Player::Show() const
{
	Sprite::Show();
	Console::SetCursorPosition(0, 0);
	Console::ForegroundColor = ConsoleColor::White;
	cout << "Player: ";
	Console::ForegroundColor = ConsoleColor::Red;
	cout << GetName();
	Console::SetCursorPosition(35, 0);
	Console::ForegroundColor = ConsoleColor::White;
	cout << "Score: ";
	Console::ForegroundColor = ConsoleColor::Green;
	cout << GetScore();

	Console::SetCursorPosition(75, 0);
	Console::ForegroundColor = ConsoleColor::Yellow;
	cout << "Lives: ";

	float hpRatio = static_cast<float>(m_lives) / 10;
	if (hpRatio >= 0.70f)
		Console::ForegroundColor = ConsoleColor::Green;
	else if (hpRatio >= 0.40f)
		Console::ForegroundColor = ConsoleColor::Yellow;
	else if (hpRatio >= 0.0f)
		Console::ForegroundColor = ConsoleColor::Red;

	wchar_t x = 0x2302;
	for (int i = 0; i < m_lives; i++)
	{
		Console::Write(x);
		cout << " ";
	}
	Console::ResetColor();
}
Player& Player::operator=(const Player& _ass)
{
	Sprite::operator=(_ass);
	return *this;
}