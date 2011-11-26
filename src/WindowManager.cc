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

#include "HorzSplitRenderShim.hh"
#include "VertSplitRenderShim.hh"
#include "TerminalNodeLeaf.hh"
#include "NcursesTerminal.hh"
#include "WindowManager.hh"
#include "Rokkaku.hh"

const char       * login_shell;
TerminalNodeLeaf * focusedTerminal;

void start_window_management() {
	init_window_management();
	window_management_loop();
}

void focus_on_next_terminal() {
	std::vector<TerminalNodeLeaf *>::iterator fTerm
		= rokkaku_terminal_leafs.begin();
	
	fTerm = std::find(
		rokkaku_terminal_leafs.begin(),
		rokkaku_terminal_leafs.end(),
		focusedTerminal
	);
	
	if ( fTerm == rokkaku_terminal_leafs.end() ) {
		fTerm = rokkaku_terminal_leafs.begin(); /* somehow we lost the item? */
	} else {
		fTerm += 1;
		if ( fTerm == rokkaku_terminal_leafs.end() ) {
			/* We were focused on the last item. let's wrap over. */
			fTerm = rokkaku_terminal_leafs.begin();
		}
	}
	
	focusedTerminal = *fTerm;
}

void do_key( char ch ) {
	if ( ch == 'G' ) {
		focus_on_next_terminal();
		return;
	}
	
	if ( ch < 128 ) /* If it's type-able */
		if ( focusedTerminal ) /* and we're not null */
			focusedTerminal->type(ch); /* eat it :) */
}

void window_management_loop() {
	while ( rokkaku_manage_windows ) {
		char ch = getch();
		if ( ch != ERR )
			do_key( ch );
		
		rokkaku_terminal_tree.pokeTree();
		if ( rokkaku_terminal_tree.renderTree() ) {
			focusedTerminal->set_cursor();
			update_screen();
		} else {
			usleep(5000);
		}
	}
}

TerminalNodeLeaf * newLeaf() {
	NcursesTerminal * nt = new NcursesTerminal(80, 25, 0, 0);
	nt->fork(login_shell);
	TerminalNodeLeaf * newNode = new TerminalNodeLeaf();
	newNode->setChild( nt );
	return newNode;
}

void init_window_management() {
	rokkaku_manage_windows = true;
	
	login_shell = getenv("SHELL");
	login_shell = ( login_shell ) ? login_shell : LOGIN_SHELL;
	
	TerminalNodeLeaf * newTopNode    = newLeaf();
	TerminalNodeLeaf * newBottomNode = newLeaf();
	TerminalNodeLeaf * bigNode       = newLeaf();
	
	VertSplitRenderShim * shimV = new VertSplitRenderShim(
		newTopNode, newBottomNode );
	
	HorzSplitRenderShim * shimH = new HorzSplitRenderShim( bigNode, shimV );
	
	rokkaku_terminal_tree.setRootNode( shimH );
	focusedTerminal = bigNode;
}

TerminalTree rokkaku_terminal_tree;
bool         rokkaku_manage_windows;
