#pragma once

#include "Image2D.h"
#include "Sprite.h"
#include "Cell.h"
#include "DList.h"
#include "FmodSound.h"
struct BinaryData
{
	char b_name[32];
	int b_score;
	int b_secs;

public:
	void setbinname(const char * name){ strcpy_s(b_name, name); }
};


class Game
{
private:
	char * m_playerName = NULL;
	int m_frame = 0, m_numPlayers, m_secs = 0, m_stage = 0, m_subStage = 0;

	bool m_play;
	list<Sprite *> Sprites;
	Cell<> * cells;
	Cell<> * shield;
	vector<Cell<>> LabsCells;
	int aimX, aimY;
	int m_howManyStars;
	Sound Music;
	Sound MusicBoss;
	SoundEffect * pShot;
	SoundEffect * oppShot;
	SoundEffect * oppShot2;
	SoundEffect * pdead;
	SoundEffect * BossDead;



	static unsigned char m_cheats;
	/* 0: Stars Move Fast
	   1: Player is Ghost
	   2: Static Enemies
	   3: Harmless Enemies
	*/


public:
	Game();
	~Game();
	Game(Game const& obj) = delete;
	Game& operator=(Game const& obj) = delete;
	
	void Run();

	void Refresh() const;
	bool IsCheatActive(short i) const;
	void LookNice() const;
	void EOLWrap(bool on) const;
	void ShowFlags() const;



	void Input();
	void Aim();
	void Update();
	void UpdateShield();
	void DisplayTopScores();
	void WriteOutScores(BinaryData a);
	void UpdateHighScoresList();
	void InitializeShield();
	void UpdateStars();
	void ActivateCheat(short i) { m_cheats |= (1 << i); }
	void DeactivateCheat(short i) { m_cheats &= ~(1 << i); }
	void ToggleCheat(short i) { m_cheats ^= (1 << i); }
	void ReadFromFile(int _minIndex, int _quantity);
	void SortByList( vector<BinaryData>& BinArr, const int binCount, int option);

	void Stage0();
	void Stage1();
	void StageBoss();

};

bool SortByScore(const BinaryData& a, const BinaryData& b);
bool operator >(const BinaryData& a, const BinaryData& b);
bool operator<(const BinaryData& a, const BinaryData& b);



