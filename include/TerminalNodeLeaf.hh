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

#ifndef _TERMINALNODELEAF_HH_
#define _TERMINALNODELEAF_HH_ CAN_HAS

#include "NcursesTerminal.hh"
#include "TerminalNode.hh"
#include <vector>

class TerminalNodeLeaf : public TerminalNode {
	private:
		NcursesTerminal * child;
		bool dead;
		
	public:
		TerminalNodeLeaf();
		
		virtual bool render( int rX1, int rY1, int rX2, int rY2 );
		virtual void poke();
		virtual bool isDead();
		virtual void set_cursor();
		
		void setChild( NcursesTerminal * nt );
		void type( char ch );
};

extern std::vector<TerminalNodeLeaf *> rokkaku_terminal_leafs;

#endif
