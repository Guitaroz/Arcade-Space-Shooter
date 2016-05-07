#pragma once
#include "Image2D.h"

class Sprite : public Image2D
{
	string m_name;
	int m_left, m_top;
	bool alive;
	int m_score;
	int avoidMove[2];
	int m_bosshealth = 100;

public:
	Sprite();
	Sprite(ConsoleColor _fg, ConsoleColor _bg, char const * const _text, char const * const _name, int _left, int _top);
	Sprite(Sprite const& _cpy);

	virtual ~Sprite();
	Sprite& operator =(const Sprite& _assign);

	/* A's */
	const char * GetName()const { return m_name.c_str(); }
	const int GetLeft()const { return m_left; }
	const int GetTop()const { return m_top; }
	const bool IsAlive() const { return alive; }
	const int GetScore() const { return m_score; }
	void GetAvoidMove(int& _x, int& _y) const { _x = avoidMove[0]; _y = avoidMove[1]; }
	int GetBossHealth() { return m_bosshealth; }
	//virtual const string GetID()const { return GetID(); }

	/* M's */
	void SetLeft(const int _left) { m_left = _left; }
	void SetTop(const int _top) { m_top = _top; }
	void SetName(const char*const _name);
	void SetAlive(const bool _alive) { alive = _alive; }
	void SetScore(const int _score) { m_score += _score; }
	void SetAvoidMove(int _x, int _y) { avoidMove[0] = _x; avoidMove[1] = _y; }
	void SetBossHealth(int diff) { m_bosshealth += diff; }
	//void SetID(const char* const _ID) { SetID(_ID); }

	virtual void Show() const;
	bool OutOfBounds(int x, int y) const;
	bool Collides(int x, int y, unsigned short w, unsigned short h) const;

};

