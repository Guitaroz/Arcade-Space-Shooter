#include "stdafx.h"
#include "Missile.h"


Missile::Missile()
{
	SetForegroundColor(ConsoleColor::Red);
	SetBackgroundColor(ConsoleColor::Black);
	SetText("|");
	SetAlive(true);
	m_delta.push_back(Delta(0));
	m_delta.push_back(Delta(0));

}
Missile::~Missile()
{
	/*for (int i = 0; i < m_delta.size(); i++)
	{
		vector<Delta*>::iterator iter = m_delta.begin();
		m_delta.erase(iter--);
		iter++;
	}*/

	/*for (size_t i = 0; i < m_delta.size(); i++)
	{
		delete m_delta[i];
	}
*/
}
Missile::Missile(ConsoleColor fg, ConsoleColor bg, char const * const text, char const * const name, int left, int top, Delta dX, Delta dY) : Sprite(fg, bg, text, name, left, top)
{

	m_delta.push_back(Delta(dX));
	m_delta.push_back(Delta(dY));
}

void Missile::GetDeltas(Delta& _x, Delta& _y) const
{
	_x = m_delta[0];
	_y = m_delta[1];

	
}
void Missile::SetDeltas(Delta _x, Delta _y)
{
	m_delta[0] = _x;
	m_delta[1] = _y;
	if (m_delta[1] == -1)
	{
		if (m_delta[0] == -1)
			SetText("\\");
		else if (m_delta[0] == 1)
			SetText("/");
		else if (m_delta[0] == 0)
			SetText("|");
	}
	if (m_delta[1] == 1)
	{
		if (m_delta[0] == -1)
			SetText("/");
		else if (m_delta[0] == 1)
			SetText("\\");
		else if (m_delta[0] == 0)
			SetText("|");
	}
}
void Missile::Show() const
{
	if (IsAlive())
		Sprite::Show();
}