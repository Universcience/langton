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

#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "langton.h"

#define NEWSIZE 3

void expand (grid* g);
dir c2d (char c);

grid* create_grid ()
{
	grid* g = calloc(1, sizeof(grid));
	g->size.x = g->size.y = NEWSIZE;
	g->grid = calloc(g->size.x, sizeof(color*));
	for (int i = 0 ; i < g->size.x ; ++i)
		g->grid[i] = calloc(g->size.x, sizeof(color));
	g->ants = NULL;
	g->pop = 0;

	return g;
}

// Maps a char to its direction.
dir c2d (char c)
{
	switch (c) {
		case 'u': case 'U': case '0':
			return 0;
		case 'l': case 'L': case '1':
			return 1;
		case 'd': case 'D': case '2':
			return 2;
		case 'r': case 'R': case '3':
			return 3;
		default:
			assert(false);
			return 0;
	}
}

void add_ant (grid* g, place pos, dir d, const char* moves)
{
	assert (g && moves && *moves);

	ant a = { pos, d, calloc(strlen(moves), sizeof(dir)), strlen(moves) };
	for (int i = 0 ; i < a.max ; ++i)
		a.moves[i] = c2d(moves[i]);

	g->ants = realloc(g->ants, (g->pop+1) * sizeof(ant));
	g->ants[g->pop] = a;
	g->pop++;
}

// Expand the grid by doubling its sides ; (n+1,n+1) -> (2n+1,2n+1).
/*                                   . . . . . . . . .
 *                                   . . . . . . . . .
 *                  ~ ~ ~ ~ ~        . . ~ ~ ~ ~ ~ . .
 *     0 1 2        ~ 0 1 2 ~        . . ~ 0 1 2 ~ . .
 *     3 4 5   ->   ~ 3 4 5 ~   ->   . . ~ 3 4 5 ~ . .
 *     6 7 8        ~ 6 7 8 ~        . . ~ 6 7 8 ~ . .
 *                  ~ ~ ~ ~ ~        . . ~ ~ ~ ~ ~ . .
 *                                   . . . . . . . . .
 *                                   . . . . . . . . .
 *
 *    1: 3x3         2: 5x5               3: 9x9
 */
void expand (grid* g)
{
	assert (g->size.x < INT_MAX/2 && g->size.y < INT_MAX/2);

	// Grid will become nsize, with quad fresh cells on each side.
	place nsize = { 2*g->size.x - 1 , 2*g->size.y - 1 };
	place quad = { (g->size.x - 1)/2 , (g->size.y - 1)/2 };

	color** ngrid = calloc(nsize.x, sizeof(color*));
	for (int i = 0 ; i < nsize.x ; ++i) {
		ngrid[i] = calloc(nsize.y, sizeof(color));
		if (i >= quad.x && i <= 3*quad.x) {
			for (int j = quad.y ; j <= 3*quad.y ; ++j)
				ngrid[i][j] = g->grid[i-quad.x][j-quad.y];
			free(g->grid[i-quad.x]);
		}
	}
	free(g->grid);
	g->grid = ngrid;
	g->size = nsize;

	// Update ant coordinates.
	for (int i = 0 ; i < g->pop ; ++i) {
		g->ants[i].pos.x += quad.x;
		g->ants[i].pos.y += quad.y;
	}
}

void do_step (grid* g)
{
	for (int i = 0 ; i < g->pop ; ++i) {
		ant* a = &g->ants[i];
		color* c = &g->grid[a->pos.x][a->pos.y];

		// Turn around, increment color, step forward.
		a->d = (a->d + a->moves[*c])%4;
		*c = ((*c)+1)%a->max;
		switch (a->d) {
			case UP:
				a->pos.y--;
				break;
			case DOWN:
				a->pos.y++;
				break;
			case LEFT:
				a->pos.x--;
				break;
			case RIGHT:
				a->pos.x++;
				break;
		}

		// Expand if OoB.
		if ( a->pos.x < 0 || a->pos.x >= g->size.x
		  || a->pos.y < 0 || a->pos.y >= g->size.y )
			expand(g);
	}

	g->step++;
}

void do_bstep (grid* g)
{
	for (int i = 0 ; i < g->pop ; ++i) {
		ant* a = &g->ants[i];
		color* c;

		// Step backwards, decrement color, reverse turn.
		switch (a->d) {
			case UP:
				a->pos.y++;
				break;
			case DOWN:
				a->pos.y--;
				break;
			case LEFT:
				a->pos.x++;
				break;
			case RIGHT:
				a->pos.x--;
				break;
		}

		// Expand if OoB. (Happens when rewinding before the start...)
		if ( a->pos.x < 0 || a->pos.x >= g->size.x
		  || a->pos.y < 0 || a->pos.y >= g->size.y )
			expand(g);

		c = &g->grid[a->pos.x][a->pos.y];
		// Modulo and bitwise shenanigans ensure no negative results.
		*c = (((*c)-1)%a->max + a->max)%a->max;
		a->d = (a->d - a->moves[*c]) & 0x3;
	}

	g->step--;
}
