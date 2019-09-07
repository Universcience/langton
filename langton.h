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

#ifndef LANGTON_H_
#define LANGTON_H_

#include <stdbool.h>

typedef struct {
	int x;
	int y;
} place;

typedef enum {
	UP    = 0,
	LEFT  = 1,
	DOWN  = 2,
	RIGHT = 3
} dir;

typedef int color; // Cell color

typedef struct {
	place pos;  // Current position
	dir d;      // Current direction
	dir* moves; // Full instruction set
	color max;  // Size of instruction set
} ant;

typedef struct {
	color** grid; // Contents of the grid
	place size;   // Total physical size
	ant* ants;    // List of ants
	int pop;      // Population count
	int step;     // Current step
} grid;

// Create a new blank, empty grid
grid* create_grid ();

// Add a new ant to the grid with given coordinates and instruction set
void add_ant (grid* g, place pos, dir d, const char* moves);

// Step forwards and backwards.
void do_step (grid* g);
void do_bstep (grid* g);

/* Ant programming:

At each step, each ant on a c-colored cell will rotate as instructed by
the c-th entry in its move set, then increment the color of the current cell
(rolling over to zero if it was the last color of its move set), and then step
forward on the next cell.

An ant instruction set can be represented by a null-terminated string which
contains the rotation for each color in sequence ; the directions are relative
to the current ant heading. They can be interpreted from "uldrULDR0123", with
'U' = no rotation, 'L'/'R' = left/right turn, 'D' = turn around.

In case of overlap, the ants will act sequentially, always in the same order.
*/

#endif // LANGTON_H_
