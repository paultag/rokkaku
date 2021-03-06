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

#include "Rokkaku.hh"

#include <Shibuya.hh>

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <panel.h>
#include <string>

void init_screen() {
	initscr();
	cbreak();
	noecho();
	start_color();
	
	for ( int j = 0; j < 8; j++ ) {
		for ( int i = 0; i < 8; i++ ) {
			int offset = SHIBUYA_GET_COLOR_PAIR(i, j);
			init_pair(offset, i, j);
		}
	}
}

void uninit_screen() {
	endwin();
}

void update_screen() {
	refresh();
	update_panels();
	doupdate();
}

void set_clog() {
	String debug_log;
	debug_log = "rokkaku.debug.log";
	std::ofstream * ofs = new std::ofstream(debug_log.c_str());
	std::clog.rdbuf(ofs->rdbuf());
}
