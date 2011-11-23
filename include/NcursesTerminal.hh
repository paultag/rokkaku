/*
 * Copyright (C) 2011, Paul Tagliamonte <tag@pault.ag>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _NCURSESTERMINAL_HH_
#define _NCURSESTERMINAL_HH_ CAN_HAS

#include "ANSITerminal.hh"
#include "Pane.hh"

#include <ansiescape.hh>
#include <ncurses.h>

typedef struct _TerminalSize {
        unsigned short  ws_row;         /* rows, in characters */
        unsigned short  ws_col;         /* columns, in characters */
        unsigned short  ws_xpixel;      /* horizontal size, pixels */
        unsigned short  ws_ypixel;      /* vertical size, pixels */
} TerminalSize;

class NcursesTerminal : public ANSITerminal {
	protected:
		Pane * pane;
		void _init_NcursesTerminal( int width, int height, int x, int y );

		bool tainted;

	public:
		NcursesTerminal();
		~NcursesTerminal();
		NcursesTerminal( int width, int height );
		NcursesTerminal( int width, int height, int x, int y );
		
		bool render();
		bool render(WINDOW * window);
		
		void resize( int x, int y );
		void move_to( int x, int y );
		
		void sigwinch();
		
		void insert( unsigned char c );
};

#endif
