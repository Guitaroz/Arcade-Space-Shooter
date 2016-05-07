#pragma once
#include "stdafx.h"
#include "Sprite.h"


class Player : public Sprite
{
private:
	//int m_score = 0;
	int m_lives = 10;
	

public:
	Player();
	Player(ConsoleColor _fg, ConsoleColor _bg, char const * const _text, char const * const _name, int _left, int _top);
	~Player();

	/*A's*/
	const int GetLives() const { return m_lives; }
	
	void Show() const;

	/*M's*/
	void SetLives(int _diff) { m_lives += _diff; }
	Player& operator=(const Player& _assign);
	



	
};

