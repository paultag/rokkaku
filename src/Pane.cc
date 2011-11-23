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

#include "Shibuya.hh"
#include "Pane.hh"

#include <iostream>
#include <sstream>
#include <panel.h>

unsigned int sequence = 1;

Pane::Pane(int width, int height, int x, int y) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width  = width;
	this->win = newwin(height, width, y, x);
	this->pan = new_panel(this->win);
	this->title = "Default Title";
	std::stringstream newID;
	newID << sequence;
	this->sequenceID = newID.str();
	/* XXX: flagize this
	keypad(this->win, true); */
}

Pane::~Pane() {
	del_panel(this->pan);
	this->win = NULL;
}

void Pane::focus() {
	this->log("Focusing on me");
	top_panel(this->pan);
}

void Pane::render_frame() {
	this->log("Rendering the frame");
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	wattron(win, COLOR_PAIR(2));
	box(win, 0, 0);
	String t = " [ " + this->title + " ] ";
	mvwprintw( win, 0,
		((this->width / 2) - (t.length() / 2)), t.c_str());
	wattroff( win, COLOR_PAIR(2));

}

String Pane::getId() {
	return this->sequenceID;
}

void Pane::move_to(int x, int y) {
	move_panel(this->pan, x, y);
}

void Pane::resize( int width, int height ) {
	this->log( "Resizing" );
	WINDOW * old_win  = this->win;
	WINDOW * temp_win = newwin(height, width, this->y, this->x);
	replace_panel(this->pan, temp_win);
	this->win = temp_win;
	delwin(old_win);
	this->width  = width;
	this->height = height;
}

void Pane::setTitle( String s ) {
	this->title = s;
}

WINDOW * Pane::getWindow() {
	return this->win;
}
