#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite() : Image2D()
{
	alive = true;
}

Sprite::Sprite(ConsoleColor _fg, ConsoleColor _bg, char const * const _text, char const * const _name, int _left, int _top) : Image2D(_fg, _bg, _text)
{
	SetName(_name);
	m_left = _left;
	m_top = _top;
	m_score = 0;
	avoidMove[0] = 0;
	avoidMove[1] = 1;
	alive = true;
}
Sprite::~Sprite()
{

}
Sprite::Sprite(Sprite const& _cpy)
{
	SetForegroundColor(_cpy.GetForegroundColor());
	SetBackgroundColor(_cpy.GetBackgroundColor());
	SetText(_cpy.GetText());
	SetName(_cpy.GetName());
	m_left = _cpy.GetLeft();
	m_top = _cpy.GetTop();
	avoidMove[0] = 0;
	avoidMove[1] = 1;
}
Sprite& Sprite::operator =(const Sprite& _assign)
{
	if (this != &_assign)
	{
		SetForegroundColor(_assign.GetForegroundColor());
		SetBackgroundColor(_assign.GetBackgroundColor());
		SetText(_assign.GetText());
		SetName(_assign.GetName());
		m_left = _assign.GetLeft();
		m_top = _assign.GetTop();
		avoidMove[0] = 0;
		avoidMove[1] = 1;

	}
	return *this;
}
void Sprite::SetName(const char*const _name)
{
	m_name = _name;
}
void Sprite::Show() const
{
	if (alive)
	{
		string tempname = GetText();
		int length = strlen(tempname.c_str());
		Console::SetCursorPosition(m_left, m_top);
		Console::ForegroundColor = Image2D::GetForegroundColor();
		Console::BackgroundColor = Image2D::GetBackgroundColor();

		int count = 0;
		for (int i = 0; i < length; i++)
		{
			cout << tempname[i];
			if (tempname[i] == '\n')
			{
				count++;
				Console::SetCursorPosition(m_left, m_top + count);
			}
		}
		if (!_strcmpi(m_name.c_str(), "Boss"))
		{
			Console::SetCursorPosition(m_left + GetWidth() - 4, m_top);
			Console::ForegroundColor = ConsoleColor::Cyan;
			cout << "hp:";
			float hpRatio = static_cast<float>(m_bosshealth) / 100;
			if (hpRatio >= 0.70f)
				Console::ForegroundColor = ConsoleColor::Green;
			else if (hpRatio >= 0.35f)
				Console::ForegroundColor = ConsoleColor::Yellow;
			else if (hpRatio >= 0.0f)
				Console::ForegroundColor = ConsoleColor::Red;
			cout << m_bosshealth;
		}

		Console::ResetColor();
	}
}

bool Sprite::OutOfBounds(int x, int y) const
{
	if (x >= 1 && x <= (Console::WindowWidth - 1) - GetWidth())
	{
		if (y >= 2 && y <= (Console::WindowHeight - 2) - GetHeight())
			return false;
	}
	return true;
}

bool Sprite::Collides(int _x, int _y, unsigned short _w, unsigned short _h) const
{
	for (int i = GetHeight(); i > -1; i--)
	{
		if (_x + _w == m_left || _x == m_left + GetWidth())
		{
			if (_y + _h == m_top + (GetHeight() - i) || (_y == m_top + (GetHeight() - i)))
				return true;

			for (int k = _h; k; k--)
			{
				if (k + _y == m_top + (GetHeight() - i))
					return true;
			}
		}
	}
	for (int i = GetWidth(); i > -1; i--)
	{
		if (_y + _h == m_top || _y == m_top + GetHeight())
		{
			if (_x + _w == m_left + (GetWidth() - i) || (_x == m_left + (GetWidth() - i)))
				return true;

			for (int k = _w; k; k--)
			{
				if (k + _x == m_left + (GetWidth() - i))
					return true;
			}
		}
	}
	return false;
}
