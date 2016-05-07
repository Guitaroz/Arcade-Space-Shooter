#pragma once
#include "Sprite.h"
#include "stdafx.h"

class Missile : public Sprite
{

public:
	Missile();
	~Missile();

	typedef signed char Delta;
	Missile(ConsoleColor fg, ConsoleColor bg, char const * const text, char const * const name, int left, int top, Delta dX, Delta dY);

	/*A's*/
	void GetDeltas(Delta& _x, Delta& _y) const;
//	const string GetID()const { return GetID(); }

	/*M's*/
	void SetDeltas(Delta _x, Delta _y);
	//void SetID(const char* const _ID) { SetID(_ID); }
	void Show() const;

private:
	vector<Delta> m_delta;
	//Delta m_delta[2];

};

