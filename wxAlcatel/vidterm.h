#pragma once

#include <wx/dynarray.h>

typedef union {
	struct {
		unsigned char fgcolor:4;
		unsigned char bgcolor:4;
		bool bold:1;
		bool dim:1;
		bool underline:1;
		bool blink:1;
		bool reverse:1;
	} val;
	unsigned int value:32;
} TCharAttr;

class wxTermLine
{
public:
	wxTermLine(int def_cols);
	void Zap(int start, int stop);

	int width;
	wxString chars;
	wxArrayInt attrs;

	bool dirty;
	bool has_blink;	
};

WX_DECLARE_OBJARRAY(wxTermLine, ArrayOfTermLines);

class wxVideoTerminal: public wxWindow
{
public:
	wxVideoTerminal(wxWindow *parent, wxWindowID id);
	~wxVideoTerminal();
	
	// Event Handler
	void OnPaint(wxPaintEvent &evt);
	void OnEraseBackground(wxEraseEvent &evt);
	void OnChar(wxKeyEvent &evt);
	void OnSize(wxSizeEvent &evt);
	void OnScrollBar(wxScrollWinEvent &evt);
	void OnTimer(wxTimerEvent &evt);
	void OnSetFocus(wxFocusEvent &evt);
	void OnKillFocus(wxFocusEvent &evt);

	virtual void DoSendChar(wxChar chr, int code);

	void Consume(wxChar chr);
	void Process(wxString str);
private:
	void TryStream(wxChar chr);
	void TryEscapeSequence(wxChar chr);
	void TryEscapeParameters(wxChar chr);
	void TryMode(wxChar chr);
	void TryCharsetG0(wxChar chr);
	void TryCharsetG1(wxChar chr);

	void DoEndEscapeSequence(wxChar chr);
	void DoEndXtermHack();

	void DoBackspace();
	void DoTab();
	void DoLinefeed();
	void DoCarriageReturn();
	void DoShiftIn();
	void DoShiftOut();
	void DoPrint(wxChar chr);

	void DoIndex();
	void DoReverseIndex();
	void DoStoreCursor();
	void DoRestoreCursor();
	void DoReverseLinefeed();

	void DoCursorUp(int count=1);
	void DoCursorDown(int count=1);
	void DoCursorBack(int count=1);
	void DoCursorForward(int count=1);
	void DoCursorPosition(int row=0, int col=0);

	void DoEraseInLine(int type_of=0);
	void DoEraseInDisplay(int type_of=0);
	void DoDeleteCharacter(int count=1);
	void DoInsertLine(int count=1);
	void DoDeleteLine(int count=1);
	void DoSetInsertMode();
	void DoSetReplaceMode();
	void DoSelectGraphicRendition(wxArrayInt &attrs);
	
	void DoBell();
	void DoFormfeed();
	void DoVerticalTab();
	void DoSetScrollRegion(int top, int bot);

	void DrawTexts();

	void CalcScrollRange(bool calc_range=true);
	bool UpdateScrollPos(int scroll_pos);
	virtual bool ScrollLines(int lines);
	virtual bool ScrollPages(int pages);

private:
	//int m_counter_test;
	int m_scrollbar_width;
	bool m_cursor_blink;
	bool m_redraw_bitmap;
	int m_counter_blink;

	wxSize m_screen_size;	// Actual Screen-Size
	wxSize m_viewport;	// Currently viewing
	
	int m_screen_irm;	// Insert or Replace mode

	ArrayOfTermLines m_lines;
	int m_firstline_row;
	int m_topscreen_row;
	int m_botscreen_row;
	int m_scrollpos_row;
	int m_topmargin_y;
	int m_botmargin_y;
	wxPoint m_screen_cur;

	TCharAttr m_cursor_attr;	
	bool m_scrolled_follow;
	
	int m_stream_state;
	//wxArrayInt m_stream_params;
	wxArrayString m_stream_params;
	wxString m_current_param;

	wxTimer m_timer;
	wxFont m_console_font;
	wxSize m_font_pixelsize;
	wxSize m_font_size;

	wxBitmap *mp_bitmap;	

	DECLARE_EVENT_TABLE()
};


