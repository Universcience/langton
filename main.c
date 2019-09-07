/*  Quick and dirty implementation of ncurses-based Langton's ant.
 *  Copyright (C) 2019 - Jérôme Kirman
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

#include "langton.h"

//FIXME: Decouple simulation and interface steps.

/* Keyboard commands :
 * TODO: Pan ; Focus ants
 *
 * UP    - Speed up
 * DOWN  - Speed down
 * LEFT  - Play backwards
 * RIGHT - Play forward
 * SPACE - Pause
 * p - Also pause
 * n - Next step
 * b - Step back
 *
 * e - Pan up
 * s - Pan left
 * d - Pan down
 * f - Pan right
 *
 * < - Previous ant
 * > - Next ant
 * w - Follow ant
 * x - Lock on ant
 * c - Center view
 *
 * h - Help (TODO)
 * q - Quit
 */

unsigned clamp (unsigned min, unsigned val, unsigned max);
char d2c (dir d);
int display (color c);

unsigned clamp (unsigned min, unsigned val, unsigned max)
{
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}

char d2c (dir d)
{
	switch (d) {
		case UP:
			return '^';
		case LEFT:
			return '<';
		case DOWN:
			return '>';
		case RIGHT:
			return 'v';
		default:
			return '?';
	}
}

//TODO: Handle more colors.
int display (color c)
{
	switch (c) {
		case 0:
			return A_NORMAL;
		case 1:
			return A_REVERSE;
		default:
			return A_NORMAL;
	}
}

int main (int argc, char* argv[])
{
	(void) argc; (void) argv;

	grid* g = create_grid();
	place origin = {1,1};
	add_ant(g, origin, 0, "RL");

	// Init ncurses
	initscr(); cbreak(); noecho();
	keypad(stdscr, TRUE);
	timeout(0);

	start_color();
	use_default_colors();

	int cmd;
	place corner = {0,0};
	unsigned speed = 16;
	bool pause = false;
	bool forward = true;
	bool follow = true;
	bool lock = false;
	bool over = false;

	while (!over)
	{
		// Display simulation state
		move(0,0);
		for (int i = 0 ; i < COLS ; ++i)
			addch(' ');

		move(0, 5);
		if (pause)
			addstr("[PAUSED]");

		move(0, 15);
		printw("Step : %i", g->step);

		move(0, 30);
		for (unsigned i = 0 ; i < 21-speed ; ++i)
			addch(forward ? '>' : '<');

		move(0, 52);
		if (lock)
			addstr(" [LOCK] ");
		else if (follow)
			addstr("[FOLLOW]");

		// TODO: Center view if required.

		// Draw grid
		for (int i = corner.x ; i < corner.x + COLS ; ++i)
			for (int j = corner.y ; j < corner.y + LINES-1 ; ++j) {
				if (i < g->size.x && j < g->size.y)
					attron(display(g->grid[i][j]));
				else
					attron(A_INVIS);
				mvaddch(j-corner.y+1, i-corner.x, '.');
				attrset(A_NORMAL);
			}

		// Draw ants and refresh
		attron(A_BOLD);
		for (int i = 0 ; i < g->pop ; ++i) {
			place ap = g->ants[i].pos;
			if (ap.x >= corner.x && ap.x < corner.x + COLS
			 && ap.y >= corner.y && ap.y < corner.y + LINES-1)
				mvaddch(ap.y-corner.y, ap.x-corner.x, d2c(g->ants[i].d));
		}
		attroff(A_BOLD);
		move(0,0);
		cmd = getch();
		flushinp();

		// Keyboard commands
		switch (cmd) {
			case KEY_UP:
				speed--;
				break;
			case KEY_DOWN:
				speed++;
				break;
			case KEY_LEFT:
				forward = false;
				break;
			case KEY_RIGHT:
				forward = true;
				break;
			case 'n':
				pause = true;
				do_step(g);
				break;
			case 'b':
				pause = true;
				do_bstep(g);
				break;
			case ' ':
			case 'p':
				pause ^= true;
				break;
			case 'w':
				follow ^= true;
				break;
			case 'x':
				lock ^= true;
				break;
			case 'c':
			case '<':
			case '>':
			case 'e':
			case 's':
			case 'd':
			case 'f':
				//TODO
				break;
			case 'q':
				over = true;
				break;
			default:
				mvprintw(0, 0, "%i", cmd);
		}
		speed = clamp(1, speed, 20);

		// Ant step
		usleep((unsigned) 1 << speed);

		if (pause)
			continue;

		if (forward)
			do_step(g);
		else
			do_bstep(g);
	}

	free(g->ants);
	for (int i = 0 ; i < g->size.x ; ++i)
		free(g->grid[i]);
	free(g->grid);
	free(g);

	endwin();
	return EXIT_SUCCESS;
}
