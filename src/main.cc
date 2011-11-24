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

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <menu.h>

#include "NcursesTerminal.hh"
#include "TerminalEndNode.hh"
#include "TerminalTree.hh"
#include "RokkakuMenu.hh"
#include "Rokkaku.hh"
#include "Pane.hh"

#define LOGIN_SHELL "/bin/sh"

using namespace std;

TerminalTree tt;
NcursesTerminal * focusedTerminal;

int rX1, rX2, rY1, rY2;

void do_wm_ing() {
	char ch;
	while ( true ) {
		ch = getch();
		if ( ch != ERR && ch < 128 ) {
			if ( focusedTerminal )
				focusedTerminal->type( ch );
		}
		tt.render( rX1, rY1, rX2, rY2 );
		update_screen();
		usleep(20);
	}
}

const char * login_shell;

int main ( int argc, char ** argv ) {
	set_clog(); /* dump to the logging fd (note: both libansiescape
	               and libshibuya, when compiled with DEBUG=true will
	               write to this log as well. */
	init_screen();
	/* let's setup the shell stuff and rock' */
	login_shell = getenv("SHELL");
	login_shell = ( login_shell ) ? login_shell : LOGIN_SHELL;
	
	std::clog << login_shell << std::endl;
	
	/* initial tty */
	NcursesTerminal * nt = new NcursesTerminal( 80, 25, 0, 0 );
	focusedTerminal = nt;
	
	nt->fork( login_shell );
	
	TerminalEndNode * ten = new TerminalEndNode();
	ten->setChildTerminal( nt );
	tt.setRootNode( ten );
	
	/* to start, we'll just render the whole screen. */
	rX1 = 0;
	rX2 = 0;
	getmaxyx(stdscr, rY2, rX2);
	/* mkay, let's do this! */
	update_screen();
	timeout(0);
	do_wm_ing();
	uninit_screen();
}
