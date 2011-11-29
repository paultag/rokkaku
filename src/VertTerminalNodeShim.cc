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

#include "VertTerminalNodeShim.hh"

#include <iostream>

VertTerminalNodeShim::VertTerminalNodeShim(
	TerminalTreeNode * left,
	TerminalTreeNode * right
) {
	this->leftNode  = left;
	this->rightNode = right;
}

VertTerminalNodeShim::~VertTerminalNodeShim() {}

void VertTerminalNodeShim::render( int rX1, int rY1, int rX2, int rY2 ) {
	int middleX = ((rX2 - rX1) / 2) + rX1;
	
	if ( this->leftNode ) /* if we've found a way to unalloc the node */
		this->leftNode->render( rX1, rY1, middleX, rY2 );
	
	if ( this->rightNode )
		this->rightNode->render( middleX, rY1, rX2, rY2 );
}
void VertTerminalNodeShim::flush() {
	if ( this->leftNode )
		this->leftNode->flush();
	if ( this->rightNode )
		this->rightNode->flush();
}
void VertTerminalNodeShim::prune_tree( TerminalTreeNode ** newSelfRoot ) {
	if ( this->leftNode )
		this->leftNode->prune_tree( &this->leftNode );
	
	if ( this->rightNode )
		this->rightNode->prune_tree( &this->rightNode );
	
	/* ok, now that our sub-nodes are pruned, we can check to see if we can
	   prune ourself from the "tree" */
	
	bool nb = ( this->rightNode == 0 );
	bool nt = ( this->leftNode  == 0 );
	
	if ( nb || nt ) {
		/* either nb or nt is null */
		if ( nb && nt ) {
			/* they're both null */
			*newSelfRoot = NULL;
		} else if ( nb ) {
			/* just nb */
			*newSelfRoot = this->leftNode;
		} else {
			/* just nt */
			*newSelfRoot = this->rightNode;
		}
		delete this; /* we're no longer needed */
	} /* else, we do nothing */
}
bool VertTerminalNodeShim::isDead() {
	return false;
}
void VertTerminalNodeShim::replace_node(
	TerminalTreeNode ** newSelfRoot,
	TerminalTreeNode * node
) {

}

