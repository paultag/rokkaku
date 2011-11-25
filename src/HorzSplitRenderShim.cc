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

#include "HorzSplitRenderShim.hh"

HorzSplitRenderShim::HorzSplitRenderShim( TerminalNode * topNode,
	TerminalNode * bottomNode
) {
	this->topNode    = topNode;
	this->bottomNode = bottomNode;
}

HorzSplitRenderShim::~HorzSplitRenderShim() {
	/* dealloc */
}

bool HorzSplitRenderShim::render( int rX1, int rY1, int rX2, int rY2 ) {
	int middleY = ( rY2 / 2 ); /* XXX: Fixme */
	
	bool r1 = false;
	bool r2 = false;
	
	if ( this->topNode )
		r1 = this->topNode->render(    rX1, rY1, rX2, middleY );
	
	if ( this->bottomNode )
		r2 = this->bottomNode->render( rX1, middleY, rX2, rY2 );
	
	return (r1 || r2);
}

void HorzSplitRenderShim::poke() {
	if ( this->topNode )
		this->topNode->poke();
	
	if ( this->bottomNode )
		this->bottomNode->poke();
}

bool HorzSplitRenderShim::isDead() {
	return this->dead;
}
