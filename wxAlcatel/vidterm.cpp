#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "vidterm.h"

#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(ArrayOfTermLines);

enum {IRM_INSERT=0,
IRM_REPLACE};

enum {STATE_STREAM=0,
STATE_ESCAPE,
STATE_ESCAPE_LB,
STATE_XTERM_HACK,
STATE_MODE,
STATE_CHARSET_G0,
STATE_CHARSET_G1};

enum {TIMER_ID=1234};

///////////////////////////////////////////////////////////////////////////////
// Events

BEGIN_EVENT_TABLE(wxVideoTerminal, wxWindow)
	EVT_PAINT(wxVideoTerminal::OnPaint)
	EVT_CHAR(wxVideoTerminal::OnChar)
	EVT_SIZE(wxVideoTerminal::OnSize)
	EVT_SCROLLWIN(wxVideoTerminal::OnScrollBar)
	EVT_TIMER(TIMER_ID, wxVideoTerminal::OnTimer)
	EVT_SET_FOCUS(wxVideoTerminal::OnSetFocus)
	EVT_KILL_FOCUS(wxVideoTerminal::OnKillFocus)
	EVT_ERASE_BACKGROUND(wxVideoTerminal::OnEraseBackground)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////

static const wxColour colors[] = {wxColour(0, 0, 0),	// black
	wxColour(187, 0, 0),	// dark-red
	wxColour(0, 187, 0),	// dark-green
	wxColour(187, 187, 0),	// brown/dark-yellow
	wxColour(0, 0, 187),	// dark-blue
	wxColour(187, 0, 187),	// dark-magenta
	wxColour(0, 187, 187),	// dark-cyan
	wxColour(187, 187, 187),	// gray

	wxColour(85, 85, 85),	// dark-gray
	wxColour(255, 85, 85),	// red
	wxColour(85, 255, 85),	// green
	wxColour(255, 255, 85),	// yellow
	wxColour(85, 85, 255),	// blue
	wxColour(255, 85, 255),	// magent
	wxColour(85, 255, 255),	// cyan
	wxColour(255, 255, 255)	// white
};

static const wxBrush brushes[] = {wxBrush(colors[0]),
	wxBrush(colors[1]),
	wxBrush(colors[2]),
	wxBrush(colors[3]),
	wxBrush(colors[4]),
	wxBrush(colors[5]),
	wxBrush(colors[6]),
	wxBrush(colors[7]),

	wxBrush(colors[8]),
	wxBrush(colors[9]),
	wxBrush(colors[10]),
	wxBrush(colors[11]),
	wxBrush(colors[12]),
	wxBrush(colors[13]),
	wxBrush(colors[14]),
	wxBrush(colors[15])
};

static const wxPen pens[] = {wxPen(colors[0]),
	wxPen(colors[1]),
	wxPen(colors[2]),
	wxPen(colors[3]),
	wxPen(colors[4]),
	wxPen(colors[5]),
	wxPen(colors[6]),
	wxPen(colors[7]),

	wxPen(colors[8]),
	wxPen(colors[9]),
	wxPen(colors[10]),
	wxPen(colors[11]),
	wxPen(colors[12]),
	wxPen(colors[13]),
	wxPen(colors[14]),
	wxPen(colors[15])
};

///////////////////////////////////////////////////////////////////////////////

wxTermLine::wxTermLine(int def_cols): width(def_cols)
{
	chars.assign(width, ' ');
	attrs.assign(width, 0);
	has_blink = false;
	dirty = true;
};

void wxTermLine::Zap(int start, int stop)
{
	int i;
	for (i=start; i<stop && i<width; ++i)
	{
		chars[i] = ' ';
		attrs[i] = 0;
	}
	dirty = true;
};

wxVideoTerminal::wxVideoTerminal(wxWindow *parent, wxWindowID id):
	wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL),
	m_timer(this, TIMER_ID),
	m_console_font("0;-24;16;0;0;400;0;0;0;255;0;2;5;1;Courier New"),//Lucida Console
	m_font_pixelsize(8, 12),
	m_font_size(8, 15),
	mp_bitmap(NULL)
{
	m_scrollbar_width = wxSystemSettings::GetMetric(wxSYS_VSCROLL_ARROW_X);

	m_screen_size.SetWidth(80);
	m_screen_size.SetHeight(24);
	
	m_screen_irm = IRM_INSERT;
	
	m_cursor_blink = false;
	m_counter_blink = 0;

	m_screen_cur.x = 0;
	m_screen_cur.y = 0;

	m_lines.Add( wxTermLine(m_screen_size.GetWidth()), 8*m_screen_size.GetHeight() );
	
	m_topscreen_row = m_lines.GetCount()-m_screen_size.GetHeight();
	m_botscreen_row = m_lines.GetCount();
	m_firstline_row = m_topscreen_row;
	m_scrollpos_row = m_firstline_row;

	m_topmargin_y = 0;
	m_botmargin_y = m_screen_size.GetHeight()-1;

	m_scrolled_follow = true;
	
	m_viewport = m_screen_size;

	m_cursor_attr.value = 0;
	m_cursor_attr.val.fgcolor = 7;

	m_stream_state = STATE_STREAM;
	m_console_font.SetEncoding( wxFONTENCODING_CP437 ); //wxFONTENCODING_CP437  ); wxFONTENCODING_UTF8
	m_console_font.SetPixelSize( m_font_pixelsize );

	m_redraw_bitmap = true;
}

wxVideoTerminal::~wxVideoTerminal()
{
	if (mp_bitmap) delete mp_bitmap;
}

void wxVideoTerminal::DrawTexts()
{
	wxSize bmp_sz;
	if (mp_bitmap) bmp_sz = mp_bitmap->GetSize();
	
	wxSize new_bmp_sz(m_screen_size.GetWidth() * m_font_size.GetWidth(), 
		wxMax(m_screen_size.GetHeight(),m_viewport.GetHeight()) *  m_font_size.GetHeight() );
	
	if (bmp_sz != new_bmp_sz)
	{
		if (mp_bitmap) delete mp_bitmap;
		mp_bitmap = new wxBitmap(new_bmp_sz.GetWidth(),  new_bmp_sz.GetHeight(), 24);
		if ( mp_bitmap )
		{
			m_redraw_bitmap = true;
			bmp_sz = new_bmp_sz;

			wxMemoryDC dc(*mp_bitmap);
			dc.SetBackground(brushes[0]);
			dc.Clear();
		}
	}
	if (!mp_bitmap) return;

	wxMemoryDC dc(*mp_bitmap);
	dc.SetFont( m_console_font );

	TCharAttr old_attr, attr;
	
	int j = m_scrollpos_row;
	int virtual_y = -1;
	{
		int scroll_gap = m_topscreen_row - m_scrollpos_row;
		virtual_y = m_screen_cur.y+scroll_gap;
	}
	int y;
	for (y=0; y < m_viewport.GetHeight() && j < m_botscreen_row; ++y, ++j)
	{
		wxTermLine &term_line = m_lines[j];
		if (y == virtual_y || m_redraw_bitmap) term_line.dirty = true;
		if (!term_line.dirty) continue;

		term_line.dirty = false;

		old_attr.value = 0;
		dc.SetBrush( brushes[0] );
		dc.SetPen( pens[0] );
		
		dc.DrawRectangle(0,y*m_font_size.GetHeight(), 
			bmp_sz.GetWidth(), m_font_size.GetHeight() );

		const int w = m_font_size.GetWidth();
		const int h = m_font_size.GetHeight();

		int x;
		for (x=0; x < m_screen_size.GetWidth() && x < term_line.width; ++x)
		{
			attr.value = term_line.attrs[x];
			wxUniChar ch = term_line.chars[x];

			if (attr.value == 0) continue;

			if (attr.value != old_attr.value)
			{
				if (attr.val.dim)
				{

				} else
				if (attr.val.bold)
					attr.val.fgcolor = 8+(attr.val.fgcolor%8);

				if (attr.val.reverse)
				{
					unsigned char tmp = attr.val.fgcolor;
					attr.val.fgcolor = attr.val.bgcolor;
					attr.val.bgcolor = tmp;
				}
				
				dc.SetTextForeground( colors[attr.val.fgcolor] );

				if (old_attr.val.bgcolor != attr.val.bgcolor)
				{
					dc.SetBrush( brushes[attr.val.bgcolor] );
					dc.SetPen( pens[attr.val.bgcolor] );
				}
				
#if 0
				if (old_attr.val.bold != attr.val.bold)
				{
					if (attr.val.bold)
						dc.SetFont( m_console_font.Bold() );
					else
						dc.SetFont( m_console_font );
				}
#endif
				old_attr = attr;
			}
			if (attr.val.bgcolor > 0)
			{
				dc.DrawRectangle(
					x*w,
					y*h,
					w,
					h);
			}

			dc.DrawText(ch, x*w,y*h);
			if (attr.val.underline)
			{
				dc.SetPen( pens[attr.val.fgcolor] );
				dc.DrawLine( x*w, y*h+h-1, x*w+w,y*h+h-1);
			}
		}
	}

	m_redraw_bitmap = false;
	if (m_cursor_blink)
	{
		dc.SetBrush( brushes[7] );
		dc.SetPen( pens[7] );

		if (virtual_y >= 0 && virtual_y < m_viewport.y)
		{
			dc.DrawRectangle(m_screen_cur.x*m_font_size.GetWidth(),
				virtual_y * m_font_size.GetHeight(), 
				m_font_size.GetWidth(), m_font_size.GetHeight());
		}
	}
}

void wxVideoTerminal::CalcScrollRange(bool calc_range)
{
	if (calc_range)
	{
		int range = (m_botscreen_row - m_viewport.y) - m_firstline_row;
		SetScrollbar(wxVERTICAL, m_scrollpos_row-m_firstline_row, true, range+1);
	} else
	{
		SetScrollPos(wxVERTICAL, m_scrollpos_row-m_firstline_row, true);
	}
}

void wxVideoTerminal::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxPaintDC dc(this);

	DrawTexts();

	if (mp_bitmap)
	{
		wxMemoryDC dc_mem(*mp_bitmap);
		dc.Blit( wxPoint(0, 0), dc_mem.GetSize(), &dc_mem, wxPoint(0, 0) );
	}

#if 0
	if (m_cursor_blink)
	{
		dc.SetBrush( brushes[7] );
		dc.SetPen( pens[7] );

		int scroll_gap = m_topscreen_row - m_scrollpos_row;
		int virtual_y = m_screen_cur.y+scroll_gap;
		if (virtual_y >= 0 && virtual_y < m_viewport.y)
		{
			dc.DrawRectangle(m_screen_cur.x*m_font_size.GetWidth(),
				virtual_y * m_font_size.GetHeight(), 
				m_font_size.GetWidth(), m_font_size.GetHeight());
		}
	}
#endif
}

void wxVideoTerminal::OnEraseBackground(wxEraseEvent &evt)
{
	wxDC *p_dc = evt.GetDC();
	
	wxSize sz = p_dc->GetSize();
	p_dc->SetBrush( brushes[0] );

	if (mp_bitmap)
	{
		wxSize sz_bmp = mp_bitmap->GetSize();
		if (sz_bmp.x < sz.x)
			p_dc->DrawRectangle(wxPoint(sz_bmp.x,0), wxSize(sz.x-sz_bmp.x,sz.y));

		if (sz_bmp.y < sz.y)
			p_dc->DrawRectangle(wxPoint(0,sz_bmp.y), wxSize(sz.x, sz.y-sz_bmp.y));
	} else {
		p_dc->DrawRectangle(wxPoint(0,0), sz);
	}
}

void wxVideoTerminal::DoSendChar(wxChar chr, int code)
{
	if (chr < 32 || chr > 126)
	{
		switch (code) {
		case WXK_BACK:
			Process("\x8\x1b[\x50");
			break;
		case WXK_TAB:
			//Process("\x9");
			break;
		case WXK_UP:
			//Process("\x1b[\x41");
			break;
		case WXK_DOWN:
			//Process("\x1b[\x42");
			break;
		case WXK_RIGHT:
			//Process("\x1b[\x43");
			break;
		case WXK_LEFT:
			//Process("\x1b[\x44");
			break;		
		case WXK_RETURN:
			Process("\r\n");
			break;
		}
	}
	else
		Process(chr);
}

void wxVideoTerminal::OnChar(wxKeyEvent &evt)
{
	wxChar chr = evt.GetUnicodeKey();
	int code = evt.GetKeyCode();
	DoSendChar(chr, code);
}

void wxVideoTerminal::OnSize(wxSizeEvent &evt)
{
	wxSize sz = evt.GetSize();
	m_viewport.y = sz.GetHeight() / m_font_size.GetHeight();
	m_viewport.x = sz.GetWidth() / m_font_size.GetWidth();

	CalcScrollRange();
	Refresh(true);
}

bool wxVideoTerminal::UpdateScrollPos(int scroll_pos)
{
	int max_scrolled_pos = m_botscreen_row - m_viewport.y;
	if (scroll_pos < m_firstline_row) scroll_pos = m_firstline_row;
	if (scroll_pos > max_scrolled_pos) scroll_pos = max_scrolled_pos;

	if (scroll_pos != m_scrollpos_row) {
		m_scrollpos_row = scroll_pos;
		m_redraw_bitmap = true;
		CalcScrollRange(false);
		Refresh(false);
		return true;
	}

	return false;
}

bool wxVideoTerminal::ScrollLines(int lines)
{
	int scroll_pos = m_scrollpos_row + lines;
	return UpdateScrollPos(scroll_pos);
};

bool wxVideoTerminal::ScrollPages(int pages)
{
	int lines = pages * m_screen_size.GetHeight();
	return ScrollLines(lines);
}

void wxVideoTerminal::OnScrollBar(wxScrollWinEvent &evt)
{
	if (evt.GetOrientation() == wxVERTICAL)
	{
		if (evt.GetEventType() == wxEVT_SCROLLWIN_BOTTOM)
			wxLogDebug("You scrollwin Bottom: %d", evt.GetPosition());

		else if (evt.GetEventType() == wxEVT_SCROLLWIN_TOP)
			wxLogDebug("You scrollwin Top: %d", evt.GetPosition());

		else if (evt.GetEventType() == wxEVT_SCROLLWIN_LINEDOWN)
			LineDown();

		else if (evt.GetEventType() == wxEVT_SCROLLWIN_LINEUP)
			LineUp();

		else if (evt.GetEventType() == wxEVT_SCROLLWIN_PAGEUP)
			PageUp();

		else if (evt.GetEventType() == wxEVT_SCROLLWIN_PAGEDOWN)
			PageDown();

		else if (evt.GetEventType() == wxEVT_SCROLLWIN_THUMBTRACK || 
			evt.GetEventType() == wxEVT_SCROLLWIN_THUMBRELEASE)
		{
			int scroll_pos = m_firstline_row + evt.GetPosition();
			UpdateScrollPos(scroll_pos);
		}
	}
}

void wxVideoTerminal::OnTimer(wxTimerEvent &evt)
{
	m_counter_blink = (m_counter_blink+1)% 10;
	if (m_counter_blink == 0)
	{
		m_cursor_blink = !m_cursor_blink;
		Refresh(false);
	}
	//wxLogDebug("%d", m_counter_test++);
	evt.Skip();
}

void wxVideoTerminal::OnSetFocus(wxFocusEvent& WXUNUSED(evt))
{
	m_counter_blink = 0;
	m_cursor_blink = true;
	m_timer.Start(33);
	Refresh(false);
}

void wxVideoTerminal::OnKillFocus(wxFocusEvent& WXUNUSED(evt))
{
	m_timer.Stop();
	m_cursor_blink = false;
	Refresh(false);
}




///////////////////////////////////////////////////////////////////////////////
void wxVideoTerminal::TryStream(wxChar chr)
{
	switch (chr) 
	{
	case 0x07: // BELL
		DoBell();
		break;
	case 0x08: // BS
		DoBackspace();
		break;
	case 0x09: // HT 
		DoTab();
		break;
	case 0x0a: // LF
		DoFormfeed();
		break;
	case 0x0b: // VT
		DoVerticalTab();
		break;
	case 0x0c: // FF
		DoLinefeed();
		break;
	case 0x0d: // CR
		DoCarriageReturn();
		break;
	case 0x0f: // SI
		DoShiftIn();
		break;
	case 0x0e: // SO
		DoShiftOut();
		break;
	case 0x1b: // ESC
		m_stream_state = STATE_ESCAPE;
		break;
	case 0x00: // NUL
		break;
	default:
		DoPrint(chr);
	}
}

void wxVideoTerminal::TryEscapeSequence(wxChar chr)
{
	switch (chr) {
	case '[':
		m_stream_state = STATE_ESCAPE_LB;
		return;
	case ']':
		m_stream_state = STATE_XTERM_HACK;
		return;
	case '(':
		m_stream_state = STATE_CHARSET_G0;
		return;
	case ')':
		m_stream_state = STATE_CHARSET_G1;
		return;

	case 'D': // IND
		DoIndex();
		break;
	case 'M': // RI
		DoReverseIndex();
		break;
	case 'E': // NEL
		DoLinefeed();
		break;
	case '7':// DECSC
		DoStoreCursor();
		break;
	case '8': // DECRC
		DoRestoreCursor();
		break;
	case 'I': // RLF
		DoReverseLinefeed();
		break;
	}
	m_stream_state = STATE_STREAM;
}

void wxVideoTerminal::Consume(wxChar chr)
{
	switch (m_stream_state)
	{
	case STATE_STREAM:
		TryStream(chr);
		break;
	case STATE_ESCAPE:
		TryEscapeSequence(chr);
		break;
	case STATE_ESCAPE_LB:
	case STATE_XTERM_HACK:
		TryEscapeParameters(chr);
		break;
	case STATE_MODE:
		TryMode(chr);
		break;
	case STATE_CHARSET_G0:
		TryCharsetG0(chr);
		break;
	case STATE_CHARSET_G1:
		TryCharsetG1(chr);
		break;
	}
}

void wxVideoTerminal::TryEscapeParameters(wxChar chr)
{
	if (chr == ';')
	{
		//long cur_param = 0;
		//m_current_param.ToLong( &cur_param);
		//m_stream_params.Add( cur_param );
		m_stream_params.Add( m_current_param );
		m_current_param = "";
		return;
	}
	
	if (chr == '?')
	{
		m_stream_state = STATE_MODE;
		return;
	}
	
	if (chr >= '0' && chr <= '9')
	{
		m_current_param += chr;
		return;
	}
	
	if (m_stream_state == STATE_XTERM_HACK){
		if (chr == 0x07)	// ^G
		{
			if (m_current_param.Len())
				m_stream_params.Add( m_current_param );
			DoEndXtermHack();
		} else
		{
			m_current_param += chr;
		}
	}
	else{
		if (m_current_param.Len() > 0)
		{
			//long cur_param = 0;
			//m_current_param.ToLong( &cur_param);
			//m_stream_params.Add( cur_param );
			m_stream_params.Add( m_current_param );
		}

		DoEndEscapeSequence(chr);
	}
}

void wxVideoTerminal::TryMode(wxChar chr)
{
	if (chr == 'l' || chr == 'h')
		m_stream_state = STATE_STREAM;
}

void wxVideoTerminal::TryCharsetG0(wxChar chr)
{
	wxLogDebug("TODO: charseg-g0 0x%x", chr);
	m_stream_state = STATE_STREAM;
}

void wxVideoTerminal::TryCharsetG1(wxChar chr)
{
	wxLogDebug("TODO: charseg-g1 0x%x", chr);
	m_stream_state = STATE_STREAM;
}


void wxVideoTerminal::Process(wxString str)
{
	unsigned int i;
	for (i=0; i<str.Len(); ++i)
		Consume(str.GetChar(i));

	m_counter_blink = -1;
	m_cursor_blink = false;
	Refresh(false);
}

void wxVideoTerminal::DoBell()
{
	wxBell();
}

void wxVideoTerminal::DoBackspace()
{
	m_screen_cur.x = wxMax(0, m_screen_cur.x-1);
}

void wxVideoTerminal::DoTab()
{
	// TODO: perform tabstops
	static const int fixed_tab_space = 4;
	if (m_screen_cur.x + fixed_tab_space < m_screen_size.GetWidth())
	{
		int next_tab_stop = m_screen_cur.x + fixed_tab_space;
		m_screen_cur.x = next_tab_stop - (next_tab_stop % fixed_tab_space);
	}
}

void wxVideoTerminal::DoLinefeed()
{
	DoIndex();
	m_screen_cur.x = 0;
}

void wxVideoTerminal::DoFormfeed()
{
	DoLinefeed();
}

void wxVideoTerminal::DoVerticalTab()
{
	DoLinefeed();
}

void wxVideoTerminal::DoReverseLinefeed()
{
	DoReverseIndex();
    m_screen_cur.x = 0;
}

void wxVideoTerminal::DoCarriageReturn()
{
	m_screen_cur.x = 0;
}

void wxVideoTerminal::DoShiftIn()
{
	wxLogDebug("TODO: DoShiftIn");
}

void wxVideoTerminal::DoShiftOut()
{
	wxLogDebug("TODO: DoShiftOut");
}

void wxVideoTerminal::DoPrint(wxChar chr)
{
	// TODO: Charset checking
	int j = m_topscreen_row+m_screen_cur.y;
	int i = m_screen_cur.x;
	wxTermLine &line = m_lines[j];
	if (i < line.width)
	{
		line.chars[i] = chr;
		line.attrs[i] = m_cursor_attr.value;
		line.dirty = true;
	}

	m_screen_cur.x++;
	if (m_screen_cur.x >= m_screen_size.GetWidth())
		DoLinefeed();
}

void wxVideoTerminal::DoIndex()
{
	wxLogDebug("FIXME: DoIndex");

	if (m_screen_cur.y+1 > m_botmargin_y ) // m_screen_size.GetHeight()
	{
		if (m_topmargin_y == 0)
		{
			if (m_firstline_row > 0)
			{
				m_firstline_row--;
				CalcScrollRange();
			}

			if (m_scrolled_follow)
			{
				int scroll_pos = m_botscreen_row - m_viewport.y;
				if (scroll_pos < m_firstline_row) scroll_pos = m_firstline_row;
				if (m_scrollpos_row != scroll_pos)
				{
					m_scrollpos_row = scroll_pos;
					CalcScrollRange(false);
				}
			}

			m_lines.RemoveAt(0);
			
			m_lines.Insert( wxTermLine(m_screen_size.GetWidth()), m_topscreen_row+m_botmargin_y );
		} else {

			m_lines.RemoveAt( m_topscreen_row+m_topmargin_y );
			m_lines.Insert( wxTermLine(m_screen_size.GetWidth()), m_topscreen_row+m_botmargin_y );
		}
	} else
		m_screen_cur.y++;

	m_redraw_bitmap = true;
}

void wxVideoTerminal::DoReverseIndex()
{
	wxLogDebug("FIXME: DoReverseIndex");

	if (m_screen_cur.y <= m_topmargin_y)
	{
		if (m_topmargin_y == 0)
		{
			m_lines.Insert( wxTermLine( m_screen_size.GetWidth() ), m_firstline_row);
			m_lines.RemoveAt( m_topscreen_row+m_botmargin_y);

			if (m_firstline_row < m_topscreen_row)
			{
				m_firstline_row++;
				CalcScrollRange();
				if (m_scrollpos_row < m_firstline_row)
				{
					m_scrollpos_row = m_firstline_row;
					CalcScrollRange(false);
				}
			}
		} else 
		{
			m_lines.Insert( wxTermLine( m_screen_size.GetWidth() ), m_topscreen_row+m_topmargin_y);
			m_lines.RemoveAt( m_topscreen_row+m_botmargin_y);
		}
	} else
		m_screen_cur.y--;

	m_redraw_bitmap = true;
}

void wxVideoTerminal::DoStoreCursor()
{
	wxLogDebug("TODO: DoStoreCursor");
}

void wxVideoTerminal::DoRestoreCursor()
{
	wxLogDebug("TODO: DoRestoreCursor");
}

void wxVideoTerminal::DoEndEscapeSequence(wxChar chr)
{
	bool do_esc = true;
	int count=1;
	int type_of=0;
	int row=0;
	int col=0;
	int top=0,bot=m_screen_size.y-1;
	unsigned int i;

	wxArrayInt int_params;
	for (i=0; i<m_stream_params.GetCount();++i)
	{
		long cur_param = 0;
		m_stream_params[i].ToLong( &cur_param);
		int_params.Add( cur_param );
	}

	switch (chr)
	{
	case 'A':	// CUU
		if (int_params.GetCount() >= 1) count = int_params.Item(0);
		DoCursorUp(count);
		break;
	case 'B':	// CUD
		if (int_params.GetCount() >= 1) count = int_params.Item(0);
		DoCursorDown(count);
		break;
	case 'C': // CUF
		if (int_params.GetCount() >= 1) count = int_params.Item(0);
		DoCursorForward(count);
		break;
    case 'D': // CUB
		if (int_params.GetCount() >= 1) count = int_params.Item(0);
		DoCursorBack(count);
		break;
    case 'H': // CUP
		if (int_params.GetCount() >= 1) row = int_params.Item(0);
		if (int_params.GetCount() >= 2) col = int_params.Item(1);
		DoCursorPosition(row, col);
		break;
    case 'f': // HVP
		if (int_params.GetCount() >= 1) row = int_params.Item(0);
		if (int_params.GetCount() >= 2) col = int_params.Item(1);
		wxLogDebug("REDIRECT: DoHVP");
		DoCursorPosition(row, col);
		break;
    case 'K': // EL
		if (int_params.GetCount() >= 1) type_of = int_params.Item(0);
		DoEraseInLine(type_of);
		break;
    case 'J': //  ED
		if (int_params.GetCount() >= 1) type_of = int_params.Item(0);
		DoEraseInDisplay(type_of);
		break;
    case 'P': //  DCH
		if (int_params.GetCount() >= 1) count = int_params.Item(0);
		DoDeleteCharacter(count);
		break;
    case 'L': //  IL
		if (int_params.GetCount() >= 1) count = int_params.Item(0);
		DoInsertLine(count);
		break;
    case 'M': //  DL
		if (int_params.GetCount() >= 1) count = int_params.Item(0);
		DoDeleteLine(count);
		break;
    case 'm':
		DoSelectGraphicRendition(int_params);
		break;
    case 'r': //  DECSTBM: "set-margins",
		if (int_params.GetCount() >= 1) top = int_params.Item(0);
		if (int_params.GetCount() >= 2) bot = int_params.Item(1);
		DoSetScrollRegion(top, bot);
		break;
    case 'h': //  IRMI
		DoSetInsertMode();
		break;
	case 'l': // IRMR
		DoSetReplaceMode();
		break;
	default:
		do_esc = false;
	}

	if (do_esc)
	{
		m_stream_state = STATE_STREAM;
		m_current_param = "";
		m_stream_params.Clear();
	}
}

void wxVideoTerminal::DoEndXtermHack()
{
	wxLogDebug("DoEndXtermHack");

	m_stream_state = STATE_STREAM;
	m_current_param = "";
	m_stream_params.Clear();
}

void wxVideoTerminal::DoCursorUp(int count)
{
	wxLogDebug("DoCursorUp(count=%d)", count);
	m_lines[m_topscreen_row+m_screen_cur.y].dirty = true;

	m_screen_cur.y = wxMax(0, m_screen_cur.y-count);
}

void wxVideoTerminal::DoCursorDown(int count)
{
	wxLogDebug("DoCursorDown(count=%d)", count);
	m_lines[m_topscreen_row+m_screen_cur.y].dirty = true;

	m_screen_cur.y = wxMin(m_screen_size.GetHeight()-1, m_screen_cur.y+count);
}

void wxVideoTerminal::DoCursorBack(int count)
{
	wxLogDebug("DoCursorBack(count=%d)", count);
	m_lines[m_topscreen_row+m_screen_cur.y].dirty = true;

	m_screen_cur.x = wxMax(0, m_screen_cur.x-count);
}

void wxVideoTerminal::DoCursorForward(int count)
{
	wxLogDebug("DoCursorForward(count=%d)", count);
	m_lines[m_topscreen_row+m_screen_cur.y].dirty = true;

	m_screen_cur.x = wxMin(m_screen_size.GetWidth()-1, m_screen_cur.x+count);
}

void wxVideoTerminal::DoCursorPosition(int row, int col)
{
	wxLogDebug("DoCursorPosition(row=%d, col=%d)", row, col);
	m_lines[m_topscreen_row+m_screen_cur.y].dirty = true;

	if (row  == 0) row = 1;
	if (col == 0) col = 1;

	m_screen_cur.y = wxMin(row - 1, m_screen_size.GetHeight()-1);
	m_screen_cur.x = wxMin(col - 1, m_screen_size.GetWidth()-1);
}

void wxVideoTerminal::DoEraseInLine(int type_of)
{
	wxLogDebug("DoEraseInLine(type_of=%d)", type_of);

	// inclussive
	int begin = 0;
	int end = m_screen_size.GetWidth()-1;
	int cursor = m_screen_cur.x;

	switch (type_of)
	{
	case 0:
		begin = cursor;
		break;
	case 1:
		end = cursor;
		break;
	case 2:
		break;
	default:
		return;
	};

	m_lines[m_topscreen_row + m_screen_cur.y].Zap(begin, end+1);
}

void wxVideoTerminal::DoEraseInDisplay(int type_of)
{
	wxLogDebug("DoEraseInDisplay(type_of=%d)", type_of);

	// inclussive
	wxPoint begin(0, m_topscreen_row);
	wxPoint end(m_screen_size.GetWidth()-1, m_botscreen_row-1);
	wxPoint cursor(m_screen_cur.x, m_topscreen_row+m_screen_cur.y);
	int i;
	int old_screen_y;

	switch (type_of)
	{
	case 0:
		if (cursor != begin)
		{
			begin = cursor;
			break;
		}
	case 2:
		old_screen_y = m_screen_cur.y;
		m_screen_cur.y = m_screen_size.GetHeight()-1;
		for (i=0;i<m_screen_size.GetHeight();++i)
			DoIndex();
		m_screen_cur.y = old_screen_y;
		return;

	case 1:
		end = cursor;
		break;
	default:
		
		return;
	};

	int j;
	for (j=begin.y+1;j<end.y; ++j)
		m_lines[j].Zap(0, m_screen_size.GetWidth());

	if (begin.y == end.y)
		m_lines[begin.y].Zap(begin.x, end.x+1);
	else
	{
		m_lines[begin.y].Zap(begin.x, m_screen_size.GetWidth() );
		m_lines[end.y].Zap(0, end.x+1);
	}
}

void wxVideoTerminal::DoDeleteCharacter(int count)
{
	wxLogDebug("DoDeleteCharacter(count=%d)", count);

	count = wxMin(count, m_screen_size.GetWidth() - m_screen_cur.x);

	int begin = m_screen_cur.x;
	int end = m_screen_size.GetWidth()-1;
	int i;
	wxTermLine &line = m_lines[m_topscreen_row + m_screen_cur.y];
	line.dirty = true;
	// moving back
	for (i=begin+count;i<=end;++i)
	{
		line.chars[i-count] = line.chars[i];
		line.attrs[i-count] = line.attrs[i];
	}
	begin = i-count;
	for (i=begin;i<=end;++i)
	{
		line.chars[i] = ' ';
		line.attrs[i] = 0;
	}
}

void wxVideoTerminal::DoInsertLine(int count)
{
	wxLogDebug("FIXME: DoInsertLine(count=%d)", count);
}

void wxVideoTerminal::DoDeleteLine(int count)
{
	wxLogDebug("FIXME: DoDeleteLine(count=%d)", count);
}

void wxVideoTerminal::DoSetInsertMode()
{
	m_screen_irm = IRM_INSERT;
}

void wxVideoTerminal::DoSetReplaceMode()
{
	m_screen_irm = IRM_REPLACE;
}

void wxVideoTerminal::DoSelectGraphicRendition(wxArrayInt &attrs)
{
	if (attrs.GetCount() == 0)
		attrs.Add( 0 );

	unsigned int i;
	for (i=0; i<attrs.GetCount();++i)
	{
		int cur_param = attrs[i];

		wxLogDebug("DoSelectGraphicRendition(%d=%d)", i, cur_param);

		switch (cur_param)
		{
		case 0:  // reset
			m_cursor_attr.value = 0;
			m_cursor_attr.val.fgcolor = 7;
			break;
		case 21:
			m_cursor_attr.val.bold = false;
			break;
		case 22:
			m_cursor_attr.val.dim = false;
			break;
		case 24: // underline-off
			m_cursor_attr.val.underline = false;
			break;
		case 25: // blink-off
			m_cursor_attr.val.blink = false;
			break;
		case 27: // reverse-off
			m_cursor_attr.val.reverse = false;
			break;
		case 1:	 // bold
			m_cursor_attr.val.bold = true;
			break;
		case 2:  // dim
			m_cursor_attr.val.dim = true;
			break;
		case 4:  // underline
			m_cursor_attr.val.underline = true;
			break;
		case 5:  // blink
			m_cursor_attr.val.blink = true;
			break;
		case 7:  // reverse
			m_cursor_attr.val.reverse = true;
			break;

		case 39: // default w/ underscore fgcolor
			m_cursor_attr.val.fgcolor = 7;
			break;
		case 38: // default fgcolor
			m_cursor_attr.val.fgcolor = 7;
			break;
		case 30: // black
			m_cursor_attr.val.fgcolor = 0;
			break;
		case 31: // red
			m_cursor_attr.val.fgcolor = 1;
			break;
		case 32: // green
			m_cursor_attr.val.fgcolor = 2;
			break;
		case 33: // brown
			m_cursor_attr.val.fgcolor = 3;
			break;
		case 34: // blue
			m_cursor_attr.val.fgcolor = 4;
			break;
		case 35: // magenta
			m_cursor_attr.val.fgcolor = 5;
			break;
		case 36: // cyan
			m_cursor_attr.val.fgcolor = 6;
			break;
		case 37: // white
			m_cursor_attr.val.fgcolor = 7;
			break;

		case 49: // default bgcolor
			m_cursor_attr.val.bgcolor = 0;
			break;
		case 40: // black
			m_cursor_attr.val.bgcolor = 0;
			break;
		case 41: // red
			m_cursor_attr.val.bgcolor = 1; 
			break;
		case 42: // green
			m_cursor_attr.val.bgcolor = 2;
			break;
		case 43: // brown
			m_cursor_attr.val.bgcolor = 3;
			break;
		case 44: // blue
			m_cursor_attr.val.bgcolor = 4;
			break;
		case 45: // magenta
			m_cursor_attr.val.bgcolor = 5;
			break;
		case 46: // cyan
			m_cursor_attr.val.bgcolor = 6;
			break;
		case 47: // white
			m_cursor_attr.val.bgcolor = 7;
			break;
		}
	}
}

void wxVideoTerminal::DoSetScrollRegion(int top, int bot)
{
	wxLogDebug("DoSetScrollRegion(top=%d, bot=%d)", top, bot);

	m_topmargin_y = wxMax(top, 1)-1;
	m_botmargin_y = wxMin(bot, m_screen_size.GetHeight())-1;
}