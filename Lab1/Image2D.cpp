#include "stdafx.h"
#include "Image2D.h"


Image2D::Image2D(ConsoleColor _fg, ConsoleColor _bg, char const * const _text)
{

	m_fg = _fg;
	m_bg = _bg;
	
	SetText(_text);
}
Image2D::Image2D()
{
	m_fg = (ConsoleColor)0;
	m_bg = (ConsoleColor)0;


}

Image2D::~Image2D()
{

}

void Image2D::SetText(const char * const _text)
{
	m_text = _text;
	m_height = 1;
	m_width = 0;

	int length = strlen(_text);
	int tempwidth = 0;

	for (int i = 0; i < length; i++)
	{
		if (_text[i] == '\n')
		{
			m_height++;
			if (tempwidth > m_width)
				m_width = tempwidth;
			tempwidth = 0;
			continue;
		}

		tempwidth++;
	}

	if (m_width == 0)
		m_width = tempwidth;
}
Image2D::Image2D(Image2D const& _cpy)
{

	SetText(_cpy.GetText());
	m_fg = _cpy.GetForegroundColor();
	m_bg = _cpy.GetBackgroundColor();
	m_width = _cpy.GetWidth();
	m_height = _cpy.GetHeight();
}
Image2D& Image2D::operator=(const Image2D& _assign)
{
	if (this != &_assign)
	{
		SetText(_assign.GetText());
		m_fg = _assign.GetForegroundColor();
		m_bg = _assign.GetBackgroundColor();
		m_width = _assign.GetWidth();
		m_height = _assign.GetHeight();
	}
	return *this;
}
void Image2D::Show() const
{
	Console::ForegroundColor = m_fg;
	Console::BackgroundColor = m_bg;

	cout << m_text.c_str();
	Console::ResetColor();
}