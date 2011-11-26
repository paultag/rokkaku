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

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include <Exceptions.hh>

#include "TerminalNodeLeaf.hh"

std::vector<TerminalNodeLeaf *> rokkaku_terminal_leafs;

TerminalNodeLeaf::TerminalNodeLeaf() {
	this->dead = false;
	rokkaku_terminal_leafs.push_back( this );
}

TerminalNodeLeaf::~TerminalNodeLeaf() {
	std::vector<TerminalNodeLeaf *>::iterator fTerm =
		std::find(
			rokkaku_terminal_leafs.begin(),
			rokkaku_terminal_leafs.end(),
			this
		);
		rokkaku_terminal_leafs.erase( fTerm );
}

bool TerminalNodeLeaf::render ( int rX1, int rY1, int rX2, int rY2 ) {
	if ( ! this->child )
		return false;
	
	std::clog << "Render xy wh: " << rX1 << ", " << rY1
		<< ", " << rX2 << ", " << rY2 << std::endl;
	
	int width  = (( rX2 - rX1 ) - 2);
	int height = (( rY2 - rY1 ) - 2);
	
	/* OK. Let's account for padding and such */
	this->child->move_to( rX1, rY1 );
	this->child->resize( width, height );
	
	return this->child->render();
}

void TerminalNodeLeaf::setChild ( NcursesTerminal * nt ) {
	this->child = nt;
}

NcursesTerminal * TerminalNodeLeaf::getChild() {
	return this->child;
}

void TerminalNodeLeaf::poke() {
	if ( this->child ) {
		try {
			this->child->poke();
		} catch ( DeadChildException * ex ) {
			/* expunge the dead terminal */
			this->dead  = true;
			std::clog << "Dead terminal" << std::endl;
			delete ex;
		}
	}
}

void TerminalNodeLeaf::set_cursor() {
	this->child->set_cursor();
}

void TerminalNodeLeaf::focus() {
	this->child->focus();
}

void TerminalNodeLeaf::type( char ch ) {
	if ( this->child )
		this->child->type(ch);
}

bool TerminalNodeLeaf::isDead() {
	return this->dead;
}
