#pragma once




template<typename Offset = signed char, typename Symbol = wchar_t>
struct Cell
{

	Offset m_oX, m_oY;
	ConsoleColor m_fg, m_bg;
	Symbol m_sym;
	COORD m_position;

public:
	Cell();
	~Cell();
	Cell(Offset oX, Offset oY, ConsoleColor fg, ConsoleColor bg, Symbol sym);

	bool Show(int left, int top) const;
	bool Show() const;
	void SetBg(ConsoleColor _bg);

	void SetCords(int x, int y);
	Offset& operator[](int index);
	const Offset& operator[](int index) const;


};

template<typename Offset, typename Symbol>
void Cell<Offset, Symbol>::SetBg(ConsoleColor _bg)
{
	m_bg = _bg;
}

template<typename Offset, typename Symbol>
Offset& Cell<Offset, Symbol>::operator[](int index)
{
	if (index == 0)
		return m_oX;
	if (index == 1)
		return m_oY;
	else
	{
		clog << L"Get outaa here";
		exit(1);
	}

}

template<typename Offset, typename Symbol>
const Offset& Cell<Offset, Symbol>::operator[](int index) const
{
	if (index == 0)
		return m_oX;
	if (index == 1)
		return m_oY;
	else
	{
		cerr << "Get outaa here";
		exit(1);
	}
}

template<typename Offset, typename Symbol>
Cell<Offset, Symbol>::Cell()
{
}

template<typename Offset, typename Symbol>
Cell<Offset, Symbol>::~Cell()
{
}

template<typename Offset, typename Symbol>
Cell<Offset, Symbol>::Cell(Offset _oX, Offset _oY, ConsoleColor _fg, ConsoleColor _bg, Symbol _sym)
{
	m_oX = _oX;
	m_oY = _oY;
	m_fg = _fg;
	m_bg = _bg;
	m_sym = _sym;
}

template<typename Offset, typename Symbol>
bool Cell<Offset, Symbol>::Show(int left, int top) const
{
	int newX = left + m_oX, newY = top + m_oY;
	Console::ForegroundColor = m_fg;
	Console::BackgroundColor = m_bg;

	if (newX >= 1 && newX < Console::WindowWidth - 1)
	{
		if (newY >= 2 && newY <= Console::WindowHeight-3)
		{
			Console::SetCursorPosition(newX, newY);
			Console::Write(m_sym);
			Console::ResetColor();
			return true;
		}
	}

	return false;
}

template<typename Offset, typename Symbol>
bool Cell<Offset, Symbol>::Show() const
{
	Console::ForegroundColor = m_fg;
	Console::BackgroundColor = m_bg;
	if (m_position.X >= 1 && m_position.X < Console::WindowWidth - 1)
	{
		if (m_position.Y >= 2 && m_position.Y <= Console::WindowHeight - 3)
		{
			Console::SetCursorPosition(m_position.X, m_position.Y);
			Console::Write(m_sym);
			Console::ResetColor();
			return true;
		}
	}

	return false;
}

template<typename Offset, typename Symbol>
void Cell<Offset, Symbol>::SetCords(int x, int y)
{
	if (m_oY + y >= Console::WindowHeight - 2)
	{
		m_oY = rand() % 4 + 2;
		m_oX = rand() % 104 + 1;
	}
	else if (m_oX + x > 0 && m_oX + x < Console::WindowWidth-1)
	{
		if (m_oY + y >= 0 && m_oY + y < Console::WindowHeight-2)
		{
			m_oX += x; m_oY += y;
		}
	}
}