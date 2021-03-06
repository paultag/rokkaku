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

#include "TerminalTree.hh"

TerminalTree::TerminalTree() {
}

TerminalTree::~TerminalTree() {
}

bool TerminalTree::renderTree() {
	if ( ! this->rootNode )
		return false;
	
	int rX2, rY2;
	int rX1 = 0;
	int rY1 = 0;
	getmaxyx(stdscr, rY2, rX2);
	return this->rootNode->render( rX1, rY1, rX2, rY2 );
}

void TerminalTree::pokeTree() {
	if ( this->rootNode )
		this->rootNode->flush();
}

void TerminalTree::pruneTree() {
	if ( ! this->rootNode )
		return; /* we can't prune */
	this->rootNode->prune_tree( &this->rootNode );
}

void TerminalTree::setRootNode( TerminalTreeNode * ttn ) {
	this->rootNode = ttn;
}

void TerminalTree::replace_node( TerminalTreeNode * ttn, TerminalTreeNode * toReplace ) {
	if ( this->rootNode == toReplace ) {
		this->rootNode = ttn;
	} else {
		this->rootNode->replace_node( toReplace, ttn );
	}
}

TerminalTreeNode * TerminalTree::getRootNode() {
	return this->rootNode;
}
