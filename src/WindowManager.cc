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

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <signal.h>

#include "HorzTerminalNodeShim.hh"
#include "VertTerminalNodeShim.hh"
#include "NcursesTerminal.hh"
#include "WindowManager.hh"
#include "Rokkaku.hh"

const char       * login_shell;
NcursesTerminal  * focusedTerminal;

void rokkaku_handle_signal( int signo ) {
	switch ( signo ) {
		case SIGWINCH:
			uninit_screen();
			init_screen();
			update_screen();
			break;
		case SIGTERM:
			uninit_screen();
			exit(0);
			break;
		case SIGINT:
			// send sigint to focused terminal
			break;
		default:
			break;
	}
}

void start_window_management() {
	init_window_management();
	window_management_loop();
}

void focus_on_next_terminal() {
}

void do_key( char ch ) {
	if ( ch < 0x80 ) { /* If it's type-able */
		// type at terminal
		if ( focusedTerminal )
			focusedTerminal->type( ch );
	}
}

void window_management_loop() {
	while ( rokkaku_manage_windows ) {
		char ch = getch();
		if ( ch != ERR ) {
			do_key( ch );
		}
		
		rokkaku_terminal_tree.pokeTree();
		rokkaku_terminal_tree.pruneTree();
		rokkaku_terminal_tree.renderTree();
		
		if ( focusedTerminal )
			focusedTerminal->set_cursor();
		
		update_screen();
		usleep(5000);
	}
}

void init_window_management() {
	rokkaku_manage_windows = true;
	login_shell = getenv("SHELL");
	login_shell = ( login_shell ) ? login_shell : LOGIN_SHELL;
	
	NcursesTerminal * initialTerminal  = new NcursesTerminal();
	NcursesTerminal * initialTerminal1 = new NcursesTerminal();
	VertTerminalNodeShim * shim = new VertTerminalNodeShim( initialTerminal,
		initialTerminal1 );
	
	initialTerminal->fork(login_shell);
	initialTerminal1->fork(login_shell);
	
	focusedTerminal = initialTerminal;
	rokkaku_terminal_tree.setRootNode(shim);
}

TerminalTree rokkaku_terminal_tree;
bool         rokkaku_manage_windows;
