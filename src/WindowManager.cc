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
int                menu_key;

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
			focusedTerminal->sigint();
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
	std::vector<NcursesTerminal *>::iterator i =
		std::find(
			ncurses_terminal_peers.begin(),
			ncurses_terminal_peers.end(),
			focusedTerminal
		);
	if ( i == ncurses_terminal_peers.end() ) {
		/* somehow, not found. abrt */
		std::clog << "n-term not found" << std::endl;
		focusedTerminal = *(ncurses_terminal_peers.begin());
	} else {
		++i;
		if ( i == ncurses_terminal_peers.end() ) {
			/* we had the last node */
			std::clog << "n-term wrap" << std::endl;
			focusedTerminal = *(ncurses_terminal_peers.begin());
		} else {
			/* set the iterator to current node */
			std::clog << "n-term next" << std::endl;
			focusedTerminal = *(i);
		}
	}
}
void focus_on_prev_terminal() {
	std::vector<NcursesTerminal *>::iterator i =
		std::find(
			ncurses_terminal_peers.begin(),
			ncurses_terminal_peers.end(),
			focusedTerminal
		);
	if ( i == ncurses_terminal_peers.end() ) {
		/* somehow, not found. abrt */
		std::clog << "n-term not found" << std::endl;
		focusedTerminal = *(ncurses_terminal_peers.begin());
	} else {
		--i;
		if ( i == ncurses_terminal_peers.end() ) {
			/* we had the last node */
			std::clog << "n-term wrap" << std::endl;
			focusedTerminal = *(ncurses_terminal_peers.begin());
		} else {
			/* set the iterator to current node */
			std::clog << "n-term next" << std::endl;
			focusedTerminal = *(i);
		}
	}
}

void do_wm_menu() {
	bool wm_menu_iface_active = true;
	/* freeze. Let's lock and load. */
	
	int width, height;
	getmaxyx(stdscr, height, width);
	
	int paneWidth  = 80;
	int paneHeight = 25; 
	
	Pane p( paneWidth, paneHeight,
		((width  / 2) - (paneWidth  / 2)),
		((height / 2) - (paneHeight / 2))
	);
	p.setTitle("Rokkaku Menu");
	p.render_frame();
	
	/* populate keys */
	mvwprintw(p.getWindow(), 3, 2, "q -- quit rokkaku" );
	mvwprintw(p.getWindow(), 4, 2, "w -- exit menu" );
	mvwprintw(p.getWindow(), 5, 2, "n -- next window" );
	mvwprintw(p.getWindow(), 6, 2, "m -- previous window" );
	mvwprintw(p.getWindow(), 7, 2, "h -- U new term, split horz" );
	mvwprintw(p.getWindow(), 8, 2, "v -- U new term, split vert" );
	mvwprintw(p.getWindow(), 9, 2, "l -- rebuild screen" );
	
	update_screen();
	
	while ( wm_menu_iface_active ) {
		char ch = getch();
		if ( ch != ERR ) {
			switch ( ch ) {
				case 'q':
					rokkaku_manage_windows = false;
					wm_menu_iface_active   = false;
					break;
				case 'w':
					wm_menu_iface_active   = false;
					break;
				case 'l':
					rokkaku_handle_signal(SIGWINCH);
					wm_menu_iface_active   = false;
					break;
				case 'n':
					focus_on_next_terminal();
					focusedTerminal->set_cursor();
					wm_menu_iface_active   = false;
					/* output status */
					break;
				case 'm':
					focus_on_prev_terminal();
					focusedTerminal->set_cursor();
					wm_menu_iface_active   = false;
					/* output status */
					break;
				default:
					break;
			}
		} else {
			usleep(4000);
		}
	}
}

void do_key( char ch ) {
	if ( ch < 0x80 ) { /* If it's type-able */
		// type at terminal
		
		if ( ch == menu_key ) {
			do_wm_menu();
			return;
		}
		
		if ( focusedTerminal )
			focusedTerminal->type( ch );
	}
}

bool is_active( NcursesTerminal * n ) {
	return ( std::find(
			ncurses_terminal_peers.begin(),
			ncurses_terminal_peers.end(),
			n
		) != ncurses_terminal_peers.end());
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
		
		if ( focusedTerminal ) {
			if ( is_active( focusedTerminal ) ) {
				focusedTerminal->set_cursor();
			} else {
				focusedTerminal = *(ncurses_terminal_peers.begin());
			}
		}
		
		update_screen();
		usleep(5000);
	}
}

void init_window_management() {
	
	menu_key        = 0x05; // CTRL+e for most systems.
	focusedTerminal = NULL;
	rokkaku_manage_windows = true;
	
	login_shell = getenv("SHELL");
	login_shell = ( login_shell ) ? login_shell : LOGIN_SHELL;
	
	/* clean this up */
	
	NcursesTerminal * initialTerminal  = new NcursesTerminal();
	NcursesTerminal * initialTerminal1 = new NcursesTerminal();
	NcursesTerminal * initialTerminal2 = new NcursesTerminal();
	
	VertTerminalNodeShim * shim =
		new VertTerminalNodeShim( initialTerminal, initialTerminal1 );
	
	HorzTerminalNodeShim * vshim =
		new HorzTerminalNodeShim( initialTerminal2, shim );
	
	 initialTerminal->fork(login_shell);
	initialTerminal1->fork(login_shell);
	initialTerminal2->fork(login_shell);
	
	focusedTerminal = initialTerminal;
	/* end cleanup */
	
	rokkaku_terminal_tree.setRootNode(vshim);
}

TerminalTree rokkaku_terminal_tree;
bool         rokkaku_manage_windows;
