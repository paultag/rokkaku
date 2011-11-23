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

#include "TerminalTree.hh"

#include <iostream>

void TerminalNode::render( int x1, int x2, int y1, int y2 ) {
	/* Nullop */
}

TerminalNode::TerminalNode() {
	/* Nullop */
}

TerminalNode::~TerminalNode() {
	/* Nullop */
}

TerminalTree::TerminalTree() {
	this->rootNode = NULL;
}

TerminalTree::~TerminalTree() {
	/* Nullop */
}

void TerminalTree::render( int x1, int x2, int y1, int y2 ) {
	if ( this->rootNode )
		this->rootNode->render( x1, x2, y1, y2 );
}

void TerminalTree::setRootNode( TerminalNode * n ) {

}
