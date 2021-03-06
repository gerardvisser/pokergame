/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2018 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stddef.h>
#include <pokerengine/CardDrawIterator.h>

#define PLAYER_COUNT 4

CardDrawIterator::CardDrawIterator (const Game* game) : m_index (0) {
  int i = (game->m_dealerIndex + 1) % PLAYER_COUNT;
  for (int j = 0; j < PLAYER_COUNT; ++j) {
    wm_players[j] = game->m_players[i];
    i = (i + 1) % PLAYER_COUNT;
  }
  if (!hasNext ()) {
    /* TODO: Handle error. Here there are no active players available.  */
  }
}

bool CardDrawIterator::hasNext (void) {
  while (m_index < PLAYER_COUNT && !wm_players[m_index]->isActive ()) {
    ++m_index;
  }
  return m_index < PLAYER_COUNT;
}

Player* CardDrawIterator::next (void) {
  if (!hasNext ())
    return NULL;
  Player* result = wm_players[m_index];
  ++m_index;
  return result;
}
