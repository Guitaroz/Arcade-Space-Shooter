#include "stdafx.h"
#include "Game.h"
#include "Sprite.h"
#include "Missile.h"
#include "Player.h"
#include "Cell.h"
#include "DList.h"
#include "FmodSound.h"

void ScoresHeader()
{
	string topscores = "Top Scores";
	Console::SetCursorPosition(Console::WindowWidth / 2 - topscores.size() / 2, 2);
	cout << topscores;
	Console::ForegroundColor = ConsoleColor::Gray;
	Console::SetCursorPosition(35, 5);
	cout << "Player:";
	Console::SetCursorPosition(54, Console::CursorTop);
	cout << "Score:";
	Console::SetCursorPosition(62, Console::CursorTop);
	cout << "Time(secs):" << "\n\n";

	int y = Console::CursorTop;

	Console::SetCursorPosition(1, Console::WindowHeight - 10);
	Console::ForegroundColor = ConsoleColor::Blue;

	cout << "\t     1) Sort By Time    2) Sort By Name (A-Z)    3) Sort By Name (Z-A)    4) Sort By Score";
	Console::ForegroundColor = ConsoleColor::Green;
	Console::SetCursorPosition(1, Console::WindowHeight - 8);
	cout << "\t\t\t\t      Press Escape to go Proceed";
	Console::SetCursorPosition(1, Console::WindowHeight - 7);
	cout << "\t\t\t\t   Press Enter to save to a file";


	Console::SetCursorPosition(1, y);
}

void Game::ShowFlags() const
{
	ConsoleColor activated = ConsoleColor::DarkGreen, deactivated = ConsoleColor::DarkGray;
	Console::SetCursorPosition(1, Console::WindowHeight - 1);
	Console::ForegroundColor = ConsoleColor::Yellow;
	string flags[] {"LightSpeed", "Untouchable", "Frozen Ai", "No Bullets"};

	for (int i = 0; i < 4; i++)
	{
		if (IsCheatActive(i))
			Console::ForegroundColor = activated;
		else
			Console::ForegroundColor = deactivated;
		cout << flags[i] << '\t';
	}


}
void DrawFrame();

Game::Game()
{
	m_numPlayers = 0;
	srand(static_cast<unsigned int>(time(nullptr)));
	LookNice();
}

unsigned char Game::m_cheats = 0;

Game::~Game()
{
	auto iter = Sprites.begin();
	for (; iter != Sprites.end(); iter++)
	{
		delete (*iter);
		iter = Sprites.erase(iter);
		if (iter == Sprites.end())
			break;
	}
	delete[] cells;
}

bool Game::IsCheatActive(short i) const
{
	if (m_cheats & (1 << i))
		return true;
	else
		return false;
}

bool operator>(const BinaryData& a, const BinaryData& b)
{
	return _strcmpi(a.b_name, b.b_name) > 0;
}
bool operator<(const BinaryData& a, const BinaryData& b)
{
	return _strcmpi(a.b_name, b.b_name) < 0;
}
bool SortByScore(const BinaryData& a, const BinaryData& b)
{

	if (b.b_score == a.b_score)
	{
		return b.b_secs > a.b_secs;
	}
	else
		return b.b_score < a.b_score;
}
bool SortByTime(const BinaryData& a, const BinaryData& b)
{
	return b.b_secs < a.b_secs;
}

void Game::Run()
{

	//Making Stars
	int size = 100;
	m_howManyStars = size;
	cells = new Cell<>[size];
	COORD * Cords = new COORD[size];

	//Randomize *Stars* position
	for (int i = 0; i < size; i++)
	{
		Cords[i].X = rand() % 104 + 1;
		Cords[i].Y = rand() % 45 + 2;

		for (int j = 0; j < i; j++)
		{
			if (Cords[i].X == Cords[j].X && Cords[i].Y == Cords[i].Y)
			{
				i--;
				continue;
			}
		}
	}
	ConsoleColor colorArray[] = { ConsoleColor::White, ConsoleColor::DarkRed, ConsoleColor::White, ConsoleColor::White };
	wchar_t wideArray2[8] = { 0x00B8, 0x00B8, 0x00B8, 0x00B8, 0x00B8, 0x00B8, 0x00B8, 0x00B8, };
	for (int i = 0; i < size; i++)
	{
		cells[i] = Cell<>((signed char)Cords[i].X, (signed char)Cords[i].Y, /*ConsoleColor::White fgArr[i]*/ colorArray[rand() % 4],
			ConsoleColor::Black, wideArray2[rand() % 8]);
	}
	delete[] Cords;
	for (int i = 0; i < m_howManyStars; i++)
	{
		cells[i].Show(0, 0);
	}


	/*Reading CrossHairs from File*/ //(Extra Credit portion - no real functionality)
	string everything;
	ifstream CellsFile;
	CellsFile.open("Lab6 cells.txt");

	if (CellsFile.is_open())
	{
		int _x[9], _y[9];
		int _fg[9], _bg[9], _sym[9];
		getline(CellsFile, everything, '\0');
		CellsFile.close();
		istringstream iss(everything);
		for (int i = 0; i < 9; i++)
		{
			iss >> _x[i];
			iss >> _y[i];
			iss >> _fg[i];
			iss >> _bg[i];
			iss >> _sym[i];
			LabsCells.push_back(Cell<>((signed char)_x[i], (signed char)_y[i], (ConsoleColor)_fg[i], (ConsoleColor)_bg[i], (wchar_t)_sym[i]));
		}
	}

	//Play Musicx
	Music.Init(8);
	Music.Load("NewGameMusic.mp3");
	Music.Play(false);

	// Draw Intro
	DrawFrame();
	int width = Console::WindowWidth / 2, height = Console::WindowHeight / 2;
	Console::ForegroundColor = ConsoleColor::DarkGray;
	string message = "Press F1 through F4 to activate the cheats.";
	Console::SetCursorPosition(width - (message.size() / 2) - 1, height + 15);
	cout << message;
	string tempname;
	message = "Welcome to this nonsense, enter your name : ";
	Console::SetCursorPosition(width - (message.size() / 2), height++);
	Console::ForegroundColor = ConsoleColor::Magenta;
	cout << message;

	getline(cin, tempname, '\n');

	if (tempname.length() < 1)
		tempname = "I'm Batman";

	/*Read in Sprites from File*/
	ifstream ReadIn;
	ReadIn.open("SpritesList.txt");

	if (ReadIn.is_open())
	{
		int  fgColor, bgColor, left, top;
		//ReadIn.ignore(LLONG_MAX, '\n');

		while (!ReadIn.eof())
		{
			string text;
			string name;
			getline(ReadIn, text, '\t');
			if (ReadIn.eof())
				break;
			ReadIn >> fgColor;
			ReadIn >> bgColor;
			ConsoleColor fg = (ConsoleColor)fgColor;
			ConsoleColor bg = (ConsoleColor)bgColor;
			ReadIn.ignore(LLONG_MAX, '\t');
			getline(ReadIn, name, '\t');
			ReadIn >> left;
			ReadIn >> top;
			if (name == "Player")
			{
				Player * p = new Player(fg, bg, text.c_str(), tempname.c_str(), left, top);
				p->SetID("Player");
				Sprites.push_back(p);
			}
			else
			{
				Sprite * o = new Sprite(fg, bg, text.c_str(), name.c_str(), left, top);
				o->SetID("Opponent");
				Sprites.push_back(o);
			}
			m_numPlayers++;
			ReadIn.ignore(LLONG_MAX, '\n');
		}
		ReadIn.close();
	}

	/* Initializing First Move depending on Opponent's Row*/
	{
		auto iter = next(Sprites.begin()); //(Player is always Sprites[0])
		int count = 1;
		for (; count < m_numPlayers && iter != Sprites.end(); iter++, count++)
		{
			if ((*iter)->GetTop() == 1 || (*iter)->GetTop() == 9)
			{
				(*iter)->SetAvoidMove(-1, 0);
			}
			if ((*iter)->GetTop() == 5)
			{
				(*iter)->SetAvoidMove(1, 0);
			}
		}
	}

	message = " would you like to see the high scores? y/n ";

	Console::SetCursorPosition(width - (message.size() / 2), height++);
	Console::ForegroundColor = ConsoleColor::Magenta;
	cout << (*Sprites.begin())->GetName() << message;
	while (true)
	{
		if (GetAsyncKeyState('Y') & 0x8000)
		{
			DisplayTopScores();
			break;
		}
		else if (GetAsyncKeyState('N') & 0x8000)
		{
			break;
		}
	}                 

	//Initialize SoundEffects;
	oppShot = new  SoundEffect("OpponentLasers1.mp3");
	pShot = new SoundEffect("PlayerLaser.mp3");
	oppShot2 = new SoundEffect("PlayerLasers.mp3");
	pdead = new SoundEffect("PlayerHit.mp3");
	BossDead = new SoundEffect("BossDead.mp3");
	m_play = true;
	aimX = 45, aimY = 20; //Initial Position for Crosshairs
	time_t timer = time(0);

	InitializeShield();

	/*Game Loop*/
	EOLWrap(false);
	for (; m_play; m_frame++)
	{

		time_t timer2 = time(0);
		m_secs = (int)(timer2 - timer);


		UpdateStars();
		Aim();
		Refresh();
		Input();
		if (m_frame != 0)
		{
			if (!(m_frame % 2))
			{
				UpdateShield();
			}
		}
		Update();


		if (m_numPlayers == 1 && m_stage == 2)
		{
			break; // Go to boss Level
		}
		if (m_frame == 0)
		{
			Util::Pause();
		}
	}

	/*Intended to do something here to emphasize boss*/

	// Boss Level
	for (; m_play; m_frame++)
	{
		time_t timer2 = time(0);
		m_secs = (int)(timer2 - timer);

		UpdateStars();
		Aim();
		Refresh();
		Input();
		if (m_frame != 0)
		{
			if (!(m_frame % 2))
			{
				UpdateShield();
			}
		}
		Update();
		if (m_numPlayers == 1 && m_stage == 2)
		{
			break;
		}
		if (m_frame == 0)
		{
			Util::Pause();
		}
	}

	EOLWrap(true);

	time_t timer2 = time(0);
	m_secs = (int)(timer2 - timer);

	Console::Clear();

	// WIN Screen
	while (true)
	{
		for (int i = 1; i < 16; i++)
		{
			Console::SetCursorPosition(Console::WindowWidth / 2 - 15, Console::WindowHeight / 2);
			Console::ForegroundColor = (ConsoleColor)i;
			cout << (*Sprites.begin())->GetName() << '\t' << (*Sprites.begin())->GetScore() << '\t' << m_secs;
			Console::CursorVisible = false;
		}
		Console::ForegroundColor = ConsoleColor::White;
		Console::SetCursorPosition(Console::WindowWidth / 2 - 15, Console::WindowHeight - 1);
		cout << "Press Enter To Proceed";
		Util::FlushKeys();
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			break;
	}

	// Update & Display HighScores
	UpdateHighScoresList();
	DisplayTopScores();


}

void Game::InitializeShield()
{
	/*int left = (*Sprites.begin())->GetLeft();
	int width = (*Sprites.begin())->GetWidth();
	int top = (*Sprites.begin())->GetTop();
	int height = (*Sprites.begin())->GetHeight();

	COORD spots[12];
	spots[0] = { left + 1, top - 2 };
	spots[1] = { left - 2, top - 1 };
	spots[2] = { left - 3, top };
	spots[3] = { left - 4, top + height / 2 };
	spots[4] = { left - 3, top + height - 1 };
	spots[5] = { left - 2, top + height };
	spots[6] = { left + 1, top + height + 1 };
	spots[7] = { left + width + 1, top + height };
	spots[8] = { left + width + 2, top + height - 1 };
	spots[9] = { left + width + 3, top + height / 2 };
	spots[10] = { left + width + 2, top };
	spots[11] = { left + width + 1, top - 1 };

	shield = new Cell<>[8];
	wchar_t sym = 'o';
	shield[0] = Cell<>((signed char)0, (signed char)0, ConsoleColor::Yellow, ConsoleColor::Black, sym);
	shield[0].m_position.X = spots[0].X;;
	shield[0].m_position.Y = spots[0].Y;
	shield[1] = Cell<>((signed char)1, (signed char)0, ConsoleColor::Yellow, ConsoleColor::Black, sym);
	shield[1].m_position.X = spots[1].X;
	shield[1].m_position.Y = spots[1].Y;
	shield[2] = Cell<>((signed char)3, (signed char)0, ConsoleColor::DarkYellow, ConsoleColor::Black, sym);
	shield[2].m_position.X = spots[3].X; 1;
	shield[2].m_position.Y = spots[3].Y;;
	shield[3] = Cell<>((signed char)5, (signed char)0, ConsoleColor::Yellow, ConsoleColor::Black, sym);
	shield[3].m_position.X = spots[5].X;
	shield[3].m_position.Y = spots[5].Y;
	shield[4] = Cell<>((signed char)6, (signed char)0, ConsoleColor::DarkYellow, ConsoleColor::Black, sym);
	shield[4].m_position.X = spots[6].X;
	shield[4].m_position.Y = spots[6].Y;
	shield[5] = Cell<>((signed char)7, (signed char)0, ConsoleColor::Yellow, ConsoleColor::Black, sym);
	shield[5].m_position.X = spots[7].X;
	shield[5].m_position.Y = spots[7].Y;
	shield[6] = Cell<>((signed char)9, (signed char)0, ConsoleColor::Yellow, ConsoleColor::Black, sym);
	shield[6].m_position.X = spots[9].X;
	shield[6].m_position.Y = spots[9].Y;
	shield[7] = Cell<>((signed char)11, (signed char)0, ConsoleColor::Yellow, ConsoleColor::Black, sym);
	shield[7].m_position.X = spots[11].X;
	shield[7].m_position.Y = spots[11].Y;*/

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::UpdateShield()
{

	//int left, width, top, height, x, y;
	//int mainoffset = 4;
	//COORD spots[12];
	//left = (*Sprites.begin())->GetLeft();								//         .             
	//width = (*Sprites.begin())->GetWidth();								//      .      .         
	//top = (*Sprites.begin())->GetTop();									//     .  ----  .        
	//height = (*Sprites.begin())->GetHeight();							//    .   ----   .       
	//spots[0] = { left + 1, top - 2 };									//     .  ----  .         
	//spots[1] = { left - 2, top - 1 };									//      .      .          
	//spots[2] = { left - 3, top };										//         .          
	//spots[3] = { left - 4, top + height / 2 };
	//spots[4] = { left - 3, top + height - 1 };
	//spots[5] = { left - 2, top + height };
	//spots[6] = { left + 1, top + height + 1 };
	//spots[7] = { left + width + 1, top + height };
	//spots[8] = { left + width + 2, top + height - 1 };
	//spots[9] = { left + width + 3, top + height / 2 };
	//spots[10] = { left + width + 2, top };
	//spots[11] = { left + width + 1, top - 1 };

	//for (size_t i = 0; i < 8; i++)
	//{
	//	switch (shield[i].m_oX)
	//	{
	//	case 0:
	//		x = spots[0].X;
	//		y = spots[0].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 1:
	//		x = spots[1].X;
	//		y = spots[1].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 2:
	//		x = spots[2].X;
	//		y = spots[2].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 3:
	//		x = spots[3].X;
	//		y = spots[3].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 4:
	//		x = spots[4].X;
	//		y = spots[4].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 5:
	//		x = spots[5].X;
	//		y = spots[5].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 6:
	//		x = spots[6].X;
	//		y = spots[6].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 7:
	//		x = spots[7].X;
	//		y = spots[7].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 8:
	//		x = spots[8].X;
	//		y = spots[8].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 9:
	//		x = spots[9].X;
	//		y = spots[9].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 10:
	//		x = spots[10].X;
	//		y = spots[10].Y;
	//		shield[i].m_oX++;
	//		break;
	//	case 11:
	//		x = spots[11].X;
	//		y = spots[11].Y;
	//		shield[i].m_oX++;
	//		break;
	//	}

	//	if (shield[i].m_oX > 11)
	//		shield[i].m_oX = 0;

	//	shield[i].m_position.X = x;
	//	shield[i].m_position.Y = y;
	//}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::Refresh() const
{
	LockWindowUpdate(GetConsoleWindow());

	//Console::BackgroundColor = ConsoleColor::Yellow;
	Console::Clear();
	DrawFrame();
	ShowFlags();
	for (int i = 0; i < m_howManyStars; i++)
	{
		cells[i].Show(0, 0);
	}

	auto iter = Sprites.begin();

	if (IsCheatActive(1))
	{
		for (size_t i = 0; i < 8; i++)
		{
			shield[i].Show();
		}
	}
	for (; iter != Sprites.end(); iter++)
	{
		if (iter == Sprites.begin())
		{
			(*iter)->Show();
			Console::SetCursorPosition(45, 0);
			cout << "\tSeconds: " << m_secs;

		}
		else
			(*iter)->Show();
	}
	//shield->Show((*Sprites.begin())->GetLeft() + (*Sprites.begin())->GetWidth() / 2, (*Sprites.begin())->GetTop() + (*Sprites.begin())->GetHeight() / 2);


	if (m_stage <= 2)
	{
		for (unsigned int i = 0; i < LabsCells.size(); i++)
		{
			LabsCells[i].Show(aimX, aimY);
		}
	}
	LockWindowUpdate(NULL);

	System::Threading::Thread::Sleep(15);
}

void Game::Aim()
{
	LabsCells[4].m_bg = ConsoleColor::Black;

	if (GetAsyncKeyState(VK_NUMPAD4))  aimX += -3;
	if (GetAsyncKeyState(VK_NUMPAD5))  aimY += -1;
	if (GetAsyncKeyState(VK_NUMPAD6))  aimX += 3;
	if (GetAsyncKeyState(VK_NUMPAD2))  aimY += 1;
	if (GetAsyncKeyState(VK_SPACE))
	{
		LabsCells[4].m_bg = ConsoleColor::Gray;
		list<Sprite*>::iterator secIter = next(Sprites.begin());
		int count = 1;
		for (; count < m_numPlayers; secIter++, count++)
		{
			/* If missile collides with Computer */
			if ((*secIter)->Collides(aimX , aimY , 2, 2))
			{
				(*secIter)->SetAlive(false);
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::Input()
{

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		m_play = false;
	}

	int dx, dy;
	dx = dy = 0;

	if (GetAsyncKeyState('W'))  dy = -1;
	if (GetAsyncKeyState('S'))  dy = 1;
	if (GetAsyncKeyState('A'))  dx = -2;
	if (GetAsyncKeyState('D'))  dx = 2;

	if (dx || dy)
	{
		int newX = (*Sprites.begin())->GetLeft();
		int newY = (*Sprites.begin())->GetTop();

		newX += dx; //Updating the position
		newY += dy;

		//Out of bounds check
		bool collide = false, outofbounds = (*Sprites.begin())->OutOfBounds(newX, newY);
		auto enemyIter = Sprites.begin();
		for (int j = 1; j < m_numPlayers; j++)
		{
			enemyIter = next(Sprites.begin(), j);
			if (collide || outofbounds)
				break;
			if ((*enemyIter)->Collides(newX, newY, (*Sprites.begin())->GetWidth(), (*Sprites.begin())->GetHeight()))
			{
				collide = true;
			}
		}
		if (!(collide || outofbounds))
		{
			(*Sprites.begin())->SetLeft(newX);
			(*Sprites.begin())->SetTop(newY);
		}
	}

	if (!(m_frame % 2))
	{
		Missile * m = new Missile();
		m->SetLeft((*Sprites.begin())->GetLeft() + ((*Sprites.begin())->GetWidth() / 2));
		m->SetName("player");
		m->SetTop((*Sprites.begin())->GetTop() - 1);

		if (GetAsyncKeyState(VK_NUMPAD8) || (GetAsyncKeyState(VK_SPACE)))
		{
			m->SetDeltas(0, -1);
			Sprites.push_back(m);
			pShot->Play();
		}
		else if (GetAsyncKeyState(VK_NUMPAD7))
		{
			m->SetDeltas(-1, -1);
			Sprites.push_back(m);
			pShot->Play();
		}
		else if (GetAsyncKeyState(VK_NUMPAD9))
		{
			m->SetDeltas(1, -1);
			Sprites.push_back(m);
			pShot->Play();
		}
		else if (GetAsyncKeyState(VK_F1))
			ToggleCheat(0); //Stars Fast
		else if (GetAsyncKeyState(VK_F2))
		{
			ToggleCheat(1); //Ghost
			(*Sprites.begin())->SetForegroundColor(ConsoleColor::Cyan);
			if (!IsCheatActive(1))
				(*Sprites.begin())->SetForegroundColor(ConsoleColor::Magenta);
		}
		else if (GetAsyncKeyState(VK_F3))
			ToggleCheat(2); //Static Enemies
		else if (GetAsyncKeyState(VK_F4))
			ToggleCheat(3); //No Enemy Missiles
		else if (GetAsyncKeyState(VK_F5))
		{
	string ship = "    ^^\n"
				  "   /||\\\n"
				"<^</++\\>^>\n"
				"\\  \\||/  /\n"
				" \\__\\/__/\n";
			(*Sprites.begin())->SetText(ship.c_str());
			(*Sprites.begin())->SetForegroundColor(ConsoleColor::White);
		}
		else
			delete m;
	}

	//}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		m_play = false;
	}

	if ((m_frame))
	{
		switch (m_stage)
		{
		case 0:
			Stage0();
			break;
		case 1:
			Stage1();
			break;
		case 2:
			StageBoss();
			break;

		}
	}

	list<Sprite*>::iterator iter = next(Sprites.begin(), m_numPlayers);


	if (m_frame)
	{
		while (iter != Sprites.end())
		{
			Missile* m = dynamic_cast<Missile*>(*iter);
			if (m->IsAlive())
			{
				typedef signed char Delta;
				Delta _x, _y;
				m->GetDeltas(_x, _y);
				int newX = m->GetLeft() + _x, newY = m->GetTop() + _y;
				if (m->OutOfBounds(newX, newY))
					m->SetAlive(false);

				else
				{
					list<Sprite*>::iterator secIter = next(Sprites.begin());
					string missile = m->GetName();
					int count = 1;
					for (; count < m_numPlayers; secIter++, count++)
					{
						/* If missile collides with Computer */
						if (m->Collides((*secIter)->GetLeft(), (*secIter)->GetTop(), (*secIter)->GetWidth(), (*secIter)->GetHeight())
							&& missile != "ai" && (*secIter)->IsAlive())
						{
							m->SetAlive(false);

							if (!_strcmpi((*secIter)->GetName(), "Boss"))
							{
								(*secIter)->SetBossHealth(-1);
								if ((*secIter)->GetBossHealth() == 0)
								{
									(*secIter)->SetAlive(false);
									(*Sprites.begin())->SetScore(10);
									BossDead->Play();
									Music.Load("Victory.mp3");
									Music.Play(false);
									Sleep(3000);
								}
							}
							else
							{
								(*secIter)->SetAlive(false);
								(*Sprites.begin())->SetScore(1);
							}
						}
					}
					/* If Missile collides with player */
					if (!IsCheatActive(1))
					{
						if (m->Collides((*Sprites.begin())->GetLeft(), (*Sprites.begin())->GetTop(), (*Sprites.begin())->GetWidth(), (*Sprites.begin())->GetHeight())
							&& missile != "player")
						{
							m->SetAlive(false);

							Player * p = dynamic_cast<Player*> ((*Sprites.begin()));
							if (p->GetLives() == 0)
							{
								Util::FlushKeys();
								pdead->Play();
								Sleep(2000);
								Util::Pause();
								m_play = false;
								//Sprites[0]->SetAlive(false);
							}
							else
							{
								p->SetLives(-1);
								pdead->Play();
							}
						}
					}
					if (/*(m_frame % 2) &&*/  missile == "ai")
					{
						m->SetLeft(newX);
						m->SetTop(newY);
						m->SetForegroundColor(ConsoleColor::Green);
					}

					if (missile == "player")
					{
						m->SetLeft(newX);
						m->SetTop(newY);
					}
				}
			}
			++iter;
		}

		iter = Sprites.begin();

		//Checkin who is Alive and deleting 

		for (int count = 0; iter != Sprites.end(); iter++, count++)
		{
			if (!(*iter)->IsAlive())
			{
				if (count < m_numPlayers)
				{
					m_numPlayers--;
				}
				delete (*iter);
				iter = Sprites.erase(iter/*next(Sprites.begin() , count)*/);
				if (iter == Sprites.end())
					break;

			}
		}

		if (m_numPlayers == 1 && m_stage < 3) //increase stage
		{
			if (Sprites.size() == 1 || m_stage == 2)	//wait for all the missiles to be removed before adding more enemies. Add last stage
			{
				if (m_stage == 0)
				{
					m_stage++;
				}
				if (m_stage == 1)
				{


					switch (m_subStage)
					{
					case 0:
						ReadFromFile(0, 13);
						break;
					case 1:
						ReadFromFile(15, 15);
						break;
					case 2:
						ReadFromFile(25, 13);
						break;
					}
					m_subStage++;
				}
				if (m_stage >= 1 && m_subStage > 3)
				{
					m_stage++;
					//	Music.Play(true);
					//	Music.Unload();
					//MusicBoss.Play(true);
				}
			}
		}
		if (m_stage > 2 && m_numPlayers == 1)
			m_play = false;
		/*else if (m_stage > 2 && m_numPlayers == 1)
			m_play = false;*/
	}
}

void Game::EOLWrap(bool on)const
{
	// How are we now?
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(out, &mode);
	if (on)
		// Turn it on.
		mode |= ENABLE_WRAP_AT_EOL_OUTPUT;
	else
		// Turn it off.
		mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
	SetConsoleMode(out, mode);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::LookNice() const
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);
	GetConsoleScreenBufferInfoEx(consoleHandle, &info);
	COORD winSize;
	winSize.X = 105;
	winSize.Y = 48;
	info.dwSize.X = info.dwMaximumWindowSize.X = info.srWindow.Right = winSize.X;
	info.dwSize.Y = info.dwMaximumWindowSize.Y = info.srWindow.Bottom = winSize.Y;
	SetConsoleScreenBufferInfoEx(consoleHandle, &info);
	SetConsoleScreenBufferSize(consoleHandle, winSize);
	SetConsoleTitle(L"Programming 3. Turn-in 2"); // Unicode string!
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::DisplayTopScores()
{
	BinaryData bin;
	ifstream binRead;

	int binCount = 0;
	binRead.open("Highscores.bin", ios_base::binary);

	if (binRead.is_open())
	{
		while (true)
		{
			binRead.read((char*)&bin, sizeof(BinaryData));
			if (binRead.eof())
				break;
			binCount++;
		}
		binRead.close();
	}

	BinaryData * binArr = new BinaryData[binCount];

	binRead.open("Highscores.bin", ios_base::binary);

	if (binRead.is_open())
	{
		binRead.read((char*)&binArr[0], sizeof(BinaryData)* binCount);
		binRead.close();
	}

	vector <BinaryData> BinData;
	for (int i = 0; i < binCount; i++)
	{
		BinData.push_back(binArr[i]);
	}

	sort(BinData.begin(), BinData.end(), SortByScore);

	(*next(BinData.begin(), 5)).setbinname("Number Six");
	//iter.setbinname("Number Six");
	BinData.resize(25);
	binCount = 25;



	Console::Clear();
	DrawFrame();
	ScoresHeader();
	//for_each(BinData.begin(), BinData.end(), &WriteOutScores);
	for (BinaryData bin : BinData)
		WriteOutScores(bin);

	Console::SetCursorPosition(25, Console::CursorTop + 2);
	int max, min;
	max = (*BinData.begin()).b_score;
	min = (*BinData.rbegin()).b_score;
	float avg = ((float)max + (float)min) / 2.0f;
	cout << "Min score: " << min << "    Max score: " << max << "    ";
	cout << "Average score : " << avg;
	Console::CursorVisible = false;
	Sleep(200);
	Util::FlushKeys();

	while (true)
	{
		if (GetAsyncKeyState('1'))
		{
			Console::Clear();
			DrawFrame();
			ScoresHeader();
			sort(BinData.begin(), BinData.end(), SortByTime);
			for (BinaryData bin : BinData)
				WriteOutScores(bin);
			Console::SetCursorPosition(25, Console::CursorTop + 2);
			int max, min;
			max = (*BinData.begin()).b_secs;
			min = (*BinData.rbegin()).b_secs;
			float avg = ((float)max + (float)min) / 2.0f;
			cout << "Min time: " << min << "    Max time: " << max << "    ";
			cout << "Average Time : " << avg;
			Console::CursorVisible = false;
		}
		else if (GetAsyncKeyState('2'))
		{
			Console::Clear();
			DrawFrame();
			ScoresHeader();
			SortByList(BinData, binCount, 1);

		}
		else if (GetAsyncKeyState('3'))
		{
			Console::Clear();
			DrawFrame();
			ScoresHeader();
			SortByList(BinData, binCount, 2);
		}
		else if (GetAsyncKeyState('4'))
		{
			Console::Clear();
			DrawFrame();
			ScoresHeader();
			sort(BinData.begin(), BinData.end(), SortByScore);
			for (BinaryData bin : BinData)
				WriteOutScores(bin);
			Console::SetCursorPosition(25, Console::CursorTop + 2);
			int max, min;
			max = (*BinData.begin()).b_score;
			min = (*BinData.rbegin()).b_score;
			float avg = ((float)max + (float)min) / 2.0f;
			cout << "Min score: " << min << "    Max score: " << max << "    ";
			cout << "Average score : " << avg;
			Console::CursorVisible = false;
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}
		else if (GetAsyncKeyState(VK_RETURN))
		{
			Console::Clear();
			DrawFrame();
			Console::SetCursorPosition(5, 3);
			Console::ForegroundColor = ConsoleColor::Green;
			cout << "What name would you like to save the file as? ";
			string filename;
			Util::FlushKeys();
			getline(cin, filename);
			if (filename.size() < 1)
				filename = "User's Highscore";

			Sleep(200);
			Util::FlushKeys();
			int choice = -1;
			bool OctHex = false;
			do
			{
				Console::SetCursorPosition(5, 10);
				cout << "press 'Escape' to read input in oct and save player's score in hex. OtherWise Press Enter.";
				Util::FlushKeys();
				do
				{
					if (GetAsyncKeyState(VK_ESCAPE))
					{
						OctHex = true;
						break;
					}
					else if (GetAsyncKeyState(VK_RETURN))
					{
						break;
					}
				} while (true);
				Console::SetCursorPosition(5, 4);
				Console::ForegroundColor = ConsoleColor::Green;
				cout << "How many entries would you like to save?: \b";
				Console::SetCursorPosition(45, Console::CursorTop);

				if (OctHex)
					cin >> oct >> choice;
				else
					cin >> choice;

				if (choice > 0 && choice <= 25)
				{
					break;
				}
				cin.clear();
				cin.ignore(LLONG_MAX, '\n');
			} while (true);

			ofstream outStream;
			outStream.open(filename);

			if (outStream.is_open())
			{
				auto iter = BinData.begin();
				for (; iter != next(BinData.begin(), choice); iter++)
				{
					outStream << iter->b_name << '\t';
					if (OctHex)
						outStream << hex << iter->b_score << dec << '\t' << iter->b_secs << endl;

					else
						outStream << iter->b_score << '\t' << iter->b_secs << endl;
				}
				outStream.close();
			}
			break;
		}
	}
	delete binArr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::SortByList(vector<BinaryData>& binArr, const int binCount, int option)
{
	/*list<BinaryData> BinData;
	for (int i = 0; i < binCount; i++)
	{
	BinData.push_back(binArr[i]);
	}

	if (option == 1)
	BinData.sort();
	else if (option == 2)
	BinData.sort(greater<BinaryData>());
	else
	cout << "error in code";*/

	if (option == 1)
		sort(binArr.begin(), binArr.end());
	else if (option == 2)
		sort(binArr.begin(), binArr.end(), greater<BinaryData>());
	else
		cout << "error in code";


	for (BinaryData a : binArr)
		WriteOutScores(a);

}
void Game::UpdateHighScoresList()
{
	ifstream binRead;
	BinaryData bin;
	strcpy_s(bin.b_name, 32, (*Sprites.begin())->GetName());
	bin.b_score = (*Sprites.begin())->GetScore();
	bin.b_secs = m_secs;

	int binCount = 25;

	BinaryData * binArr = new BinaryData[binCount];

	binRead.open("Highscores.bin", ios_base::binary);

	if (binRead.is_open())
	{
		binRead.read((char*)&binArr[0], sizeof(BinaryData)* binCount);
		binRead.close();
	}

	vector <BinaryData> BinData;
	for (int i = 0; i < binCount; i++)
	{
		BinData.push_back(binArr[i]);
	}

	BinData.push_back(bin);
	sort(BinData.begin(), BinData.end(), SortByScore);

	BinData.resize(binCount);

	ofstream outfile;
	outfile.open("Highscores.txt");
	if (outfile.is_open())
	{
		auto iter = BinData.begin();
		while (iter != BinData.end())
		{
			outfile << iter->b_name << '\t' << iter->b_score << '\t' << iter->b_secs << '\n';
			iter++;
		}
		outfile.close();
	}

	outfile.open("Highscores.bin", ios_base::out, ios_base::binary);
	if (outfile.is_open())
	{
		outfile.write((char*)BinData.data(), sizeof(BinaryData)* 25);
		outfile.close();
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::UpdateStars()
{
	short dy;
	if (IsCheatActive(0))
		dy = 3;
	else
		dy = 1;
	for (int i = 0; i < m_howManyStars; i++)
	{
		if (cells[i].m_fg == ConsoleColor::White)
		{
			cells[i].SetCords(0, dy);
		}
		else if (!(m_frame % 8))
			cells[i].SetCords(0, dy);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Game::Stage0()
{
	int dx, dy;
	dx = dy = 0;
	int avoidX, avoidY;

	auto iter = next(Sprites.begin());
	for (int count = 1; count < m_numPlayers; count++, iter++)
	{
		if ((*iter)->IsAlive())
		{
			int newX = (*iter)->GetLeft();
			int newY = (*iter)->GetTop();

			(*iter)->GetAvoidMove(avoidX, avoidY);

			if (avoidX == 0 || avoidX == -1)
			{
				dx = 1; //Move right
				dy = 0;
			}
			else if (avoidX == 1)
			{
				dx = -1; //Move Left
				dy = 0;
			}

			if ((*iter)->OutOfBounds(newX + dx, newY + dy)) //Change directions when out of bounds.
			{
				if (newX + dx > (Console::WindowWidth - 1) - (*iter)->GetWidth())
				{
					dx = 0;
					dy = 2;
					avoidX = 1;
				}
				if (newX + dx < 1)
				{
					dx = 0;
					dy = 2;
					avoidX = -1;
				}
			}
			newX += dx;
			newY += dy;

			//Out of bounds check

			bool collide = false, outofbounds = (*iter)->OutOfBounds(newX, newY);
			auto j = Sprites.begin();
			for (int count2 = 0; count2 < m_numPlayers; j++, count2++)
			{
				if (j != iter) //Collision
				{
					if ((*j)->Collides(newX, newY, (*iter)->GetWidth(), (*iter)->GetHeight()))
					{
						collide = true;
					}
				}
			}
			if (!(collide || outofbounds)) //Make the actual changes
			{
				if (!IsCheatActive(2))
				{
					(*iter)->SetLeft(newX);
					(*iter)->SetTop(newY);

					if (dy == -1)
						avoidY = 1;
					else if (dy == 1)
						avoidY = -1;
					else
						avoidY = 0;
					(*iter)->SetAvoidMove(avoidX, avoidY);
				}
				if ((!(m_frame % 40) && count % 2 == 0) || ((!(m_frame % 20)) && count % 2 == 1))
				{
					if (!IsCheatActive(3))
					{
						int dir = rand() % 3;
						Missile * m = new Missile();
						m->SetLeft((*iter)->GetLeft() + ((*iter)->GetWidth() / 2));
						m->SetTop((*iter)->GetTop() + (*iter)->GetHeight() + 1);
						m->SetName("ai");
						switch (dir)
						{

						case 0:
							m->SetDeltas(1, 1);
							break;
						case 1:
							m->SetDeltas(0, 1);
							break;
						case 2:
							m->SetDeltas(-1, 1);
							break;
						}
						Sprites.push_back(m);
						if (m_numPlayers > 11)
							oppShot->Play();
						else if (m_numPlayers > 6)
							oppShot2->Play();
						else if (m_numPlayers > 1)
							pShot->Play();
					}

				}

			}
		}
	}
}
void Game::Stage1()
{

	if (!(m_frame % 2))
	{
		int dx, dy;
		dx = dy = 0;
		int avoidX, avoidY;
		auto iter = next(Sprites.begin());
		for (int count = 1; count < m_numPlayers; count++, iter++)
		{
			if ((*iter)->IsAlive())
			{
				int newX = (*iter)->GetLeft();
				int newY = (*iter)->GetTop();

				(*iter)->GetAvoidMove(avoidX, avoidY);

				if (m_subStage < 3)
				{
					do
					{
						dx = rand() % 3 - 1;
						dy = rand() % 2;
					} while (dx == avoidX && dy == avoidY);

					/*if ((*iter)->GetTop() < 5)
					{
					dy = 1;
					avoidY = -1;
					}*/
					if ((*iter)->GetTop() > (Console::WindowHeight / 2) + 4)
					{
						dy = rand() % 2 - 1;
						avoidY = 1;
					}
				}
				else if (m_subStage == 3)
				{
					do
					{
						dx = rand() % 5 - 2;
						dy = rand() % 5 - 2;
					} while (dx == avoidX && dy == avoidY);

					if ((*iter)->GetTop() < 5)
					{
						dy = 2;
						avoidY = -2;
					}
					if ((*iter)->GetTop() > (Console::WindowHeight / 3) + (Console::WindowHeight / 3))
					{
						dy = -2;
						avoidY = 2;
					}
				}
				if ((*iter)->OutOfBounds(newX + dx, newY + dy)) //Change directions when out of bounds.
				{
					if (newX + dx > (Console::WindowWidth - 1) - (*iter)->GetWidth())
					{
						dx = -2;
						dy = 2;
					}
				}
				//Out of bounds check
				newX += dx;
				newY += dy;
				bool collide = false, outofbounds = (*iter)->OutOfBounds(newX, newY);
				auto j = Sprites.begin();
				for (int count2 = 0; count2 < m_numPlayers; j++, count2++)
				{
					if (j != iter) //Collision
					{
						if ((*j)->Collides(newX, newY, (*iter)->GetWidth(), (*iter)->GetHeight()))
						{
							collide = true;
						}
					}
				}
				if (!(collide || outofbounds)) //Make the actual changes
				{
					if (!IsCheatActive(2))
					{
						(*iter)->SetLeft(newX);
						(*iter)->SetTop(newY);
						if (dx == -1)
							avoidX = 1;
						else if (dx == 1)
							avoidX = -1;
						else
							avoidX = 0;
						(*iter)->SetAvoidMove(avoidX, avoidY);
					}
					if ((!(m_frame % 40) && count % 2 == 0) || ((!(m_frame % 20)) && count % 2 == 1))
					{

						if (!IsCheatActive(3))
						{
							int dir = rand() % 3;
							Missile * m = new Missile();
							m->SetLeft((*iter)->GetLeft() + ((*iter)->GetWidth() / 2));
							m->SetTop((*iter)->GetTop() + (*iter)->GetHeight() + 1);
							m->SetName("ai");
							switch (dir)
							{

							case 0:
								m->SetDeltas(1, 1);
								break;
							case 1:
								m->SetDeltas(0, 1);
								break;
							case 2:
								m->SetDeltas(-1, 1);
								break;
							}
							Sprites.push_back(m);
							oppShot->Play();
						}
					}
				}
				else
				{
					/*count--;
					iter--;*/
					continue;

				}
			}
		}
	}
}
void Game::StageBoss()
{
	if (m_numPlayers == 1)
	{
		string boss = "    00[[-^^-]]00\n"
			"   00//     \\\\00\n"
			"[====\\\\ 0  0 //====]\n"
			"      \\\\    //\n"
			"       \\\\--//\n"
			"        \\\\//\n"
			"        ====\n"
			"         \\/";

		Sprite * Boss = new Sprite(ConsoleColor::DarkYellow, ConsoleColor::Black, boss.c_str(), "Boss", 15, 3);

		Sprites.push_back(Boss);
		m_numPlayers++;

		if (!IsCheatActive(0))
		{
			ToggleCheat(0);
		}

	}

	auto iter = next(Sprites.begin());
	int newX, newY, dx = 0, dy = 0;
	do
	{
		newX = (*iter)->GetLeft(), newY = (*iter)->GetTop();
		if (!(m_frame % 20))
		{
			dx = rand() % 20 - 10;
			dy = rand() % 8 - 4;
		}
		if (!(m_frame % 70))
		{
			if ((*iter)->GetLeft() < Console::WindowWidth / 2)
			{
				newX = rand() % 45 + 50;
				newY = rand() % 12 - 6;
			}
			else if ((*iter)->GetLeft() > Console::WindowWidth / 2)
			{
				newX = rand() % 50;
				newY = rand() % 12 - 6;
			}
		}
		else
		{
			dx = rand() % 3 - 1;
			dy = rand() % 3 - 1;
		}
		newX += dx;
		newY += dy;

	} while ((*iter)->OutOfBounds(newX, newY));

	if (!IsCheatActive(2))
	{
		(*next(Sprites.begin()))->SetLeft(newX);
		(*next(Sprites.begin()))->SetTop(newY);
	}
	if (!IsCheatActive(3)) //if Missiles are activated;
	{
		if (!(m_frame % 5))
		{
			Missile * m = new Missile();
			Missile * m2 = new Missile();
			Missile * m3 = new Missile();
			m->SetLeft((*iter)->GetLeft() + ((*iter)->GetWidth() / 2));
			m->SetTop((*iter)->GetTop() + (*iter)->GetHeight());
			m->SetName("ai");
			m2->SetLeft((*next(Sprites.begin()))->GetLeft() + ((*iter)->GetWidth()));
			m2->SetTop((*next(Sprites.begin()))->GetTop() + (*iter)->GetHeight() / 2 - 1);
			m2->SetName("ai");
			m3->SetLeft((*iter)->GetLeft());
			m3->SetTop((*iter)->GetTop() + (*iter)->GetHeight() / 2 - 1);
			m3->SetName("ai");

			int diffX;
			iter = Sprites.begin();

			if ((*iter)->GetLeft() > (*next(iter))->GetLeft())
			{
				diffX = 1;
			}
			if ((*iter)->GetLeft() < (*next(iter))->GetLeft())
			{
				diffX = -1;
			}


			m->SetDeltas(0, 1);


			Sprites.push_back(m);
			m2->SetDeltas(diffX, 1);
			m3->SetDeltas(diffX, 1);


			if (m_frame % 2)
				Sprites.push_back(m2);

			else
				Sprites.push_back(m3);
		}
	}
}

void Game::ReadFromFile(int _minIndex, int _quantity)
{
	ifstream ReadIn;
	ReadIn.open("SpritesList.txt");

	if (ReadIn.is_open())
	{
		int  fgColor, bgColor, left, top;
		for (int i = 0; i < (_minIndex + _quantity); i++)
		{
			string text;
			string name;
			getline(ReadIn, text, '\t');
			if (ReadIn.eof())
				break;
			ReadIn >> fgColor;
			ReadIn >> bgColor;
			ConsoleColor fg = (ConsoleColor)fgColor;
			ConsoleColor bg = (ConsoleColor)bgColor;
			ReadIn.ignore(LLONG_MAX, '\t');
			getline(ReadIn, name, '\t');
			ReadIn >> left;
			ReadIn >> top;
			if ((int)i <= _minIndex)
				continue;

			Sprite * o = new Sprite(fg, bg, text.c_str(), name.c_str(), left, top);
			o->SetID("Opponent");
			Sprites.push_back(o);

			m_numPlayers++;
			ReadIn.ignore(LLONG_MAX, '\n');
		}
		ReadIn.close();
	}
}
void Game::WriteOutScores(BinaryData a)
{
	if (!_strcmpi(a.b_name, (*Sprites.begin())->GetName()))
		Console::BackgroundColor = ConsoleColor::DarkGray;
	else
		Console::BackgroundColor = ConsoleColor::Black;
	Console::SetCursorPosition(35, Console::CursorTop);
	Console::ForegroundColor = ConsoleColor::Red;
	cout << a.b_name;
	Console::SetCursorPosition(56, Console::CursorTop);
	if (!_strcmpi(a.b_name, (*Sprites.begin())->GetName()))
		Console::ForegroundColor = ConsoleColor::Cyan;
	else
		Console::ForegroundColor = ConsoleColor::DarkCyan;
	cout << a.b_score;
	Console::ForegroundColor = ConsoleColor::Yellow;
	Console::SetCursorPosition(67, Console::CursorTop);
	cout << a.b_secs << endl;
	Console::ResetColor();
}

void DrawFrame()
{
	int width = Console::WindowWidth;
	int height = Console::WindowHeight;
	Console::SetCursorPosition(0, 1);
	Console::ForegroundColor = ConsoleColor::Blue;
	for (int i = 0; i < width - 1; i++)
	{
		cout << '\xCD';
	}
	Console::SetCursorPosition(0, height - 2);
	for (int i = 0; i < width - 1; i++)
	{
		cout << '\xCD';
	}
	for (int i = 1; i < height - 1; i++)
	{
		Console::SetCursorPosition(0, i);
		if (i == 1)
			cout << '\xC9';
		else if (i == height - 2)
			cout << '\xC8';
		else
			cout << '\xBA';
		Console::SetCursorPosition(width - 1, i);
		if (i == 1)
			cout << '\xBB';
		else if (i == height - 2)
			cout << '\xBC';
		else
			cout << '\xBA';
	}
	Console::SetCursorPosition(2, 1);
}