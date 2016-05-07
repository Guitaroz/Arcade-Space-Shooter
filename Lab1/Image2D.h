#pragma once

class Image2D
{

	ConsoleColor m_fg, m_bg; // ForeGround and BackGround
	string m_text;
	unsigned short m_width, m_height;
	string ID;




public:

	/*CTOR */
	Image2D();
	Image2D(ConsoleColor fg, ConsoleColor bg, char const * const text);
	/*Trilogy*/
	Image2D(Image2D const & _cpy);
	Image2D& operator=(const Image2D& _assign);
	virtual ~Image2D();

	/* F's */
	virtual void Show() const;


	/* A's */
	const ConsoleColor GetForegroundColor()const  { return m_fg; }
	const ConsoleColor GetBackgroundColor()const  { return m_bg; }
	const char * GetText() const { return m_text.c_str(); }
	const unsigned short GetWidth() const { return m_width; }
	const  short GetHeight()const { return m_height; }
	const string GetID()const { return ID; }


	/* M's */
	void SetForegroundColor(const ConsoleColor _fg) { m_fg = _fg; }
	void SetBackgroundColor(const ConsoleColor _bg) { m_bg = _bg; }
	void SetText(const char * const _text);
	void SetID(const char* const _ID) { ID = _ID; }

};

