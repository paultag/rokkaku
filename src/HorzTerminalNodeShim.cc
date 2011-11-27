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

#include "HorzTerminalNodeShim.hh"

#include <iostream>

HorzTerminalNodeShim::HorzTerminalNodeShim(
	TerminalTreeNode * top,
	TerminalTreeNode * bottom
) {
	this->topNode    = top;
	this->bottomNode = bottom;
}

HorzTerminalNodeShim::~HorzTerminalNodeShim() {}

void HorzTerminalNodeShim::render( int rX1, int rY1, int rX2, int rY2 ) {
	int middleY = ((rY2 - rY1) / 2);
	
	if ( this->topNode ) /* if we've found a way to unalloc the node */
		this->topNode->render( rX1, rY1, rX2, middleY );
	
	if ( this->bottomNode )
		this->bottomNode->render( rX1, middleY, rX2, rY2 );
}
void HorzTerminalNodeShim::flush() {
	if ( this->topNode )
		this->topNode->flush();
	if ( this->bottomNode )
		this->bottomNode->flush();
}
void HorzTerminalNodeShim::prune_tree( TerminalTreeNode ** newSelfRoot ) {
	if ( this->topNode )
		this->topNode->prune_tree( &this->topNode );
	
	if ( this->bottomNode )
		this->bottomNode->prune_tree( &this->bottomNode );
	
	/* ok, now that our sub-nodes are pruned, we can check to see if we can
	   prune ourself from the "tree" */
	
	bool nb = ( this->bottomNode == 0 );
	bool nt = ( this->topNode    == 0 );
	
	if ( nb || nt ) {
		/* either nb or nt is null */
		if ( nb && nt ) {
			/* they're both null */
			*newSelfRoot = NULL;
		} else if ( nb ) {
			/* just nb */
			*newSelfRoot = this->topNode;
		} else {
			/* just nt */
			*newSelfRoot = this->bottomNode;
		}
		
		delete this; /* we're no longer needed */
	} /* else, we do nothing */
}
bool HorzTerminalNodeShim::isDead() {
	return false;
}
