/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2017 Gerard Visser.

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

#include <stdlib.h>
#include <vector>
#include <pokerengine/CardDeck.h>
#include "util/random.h"

static const Card** cards = NULL;

CardDeck::CardDeck (void) {
  m_cards = (const Card**) malloc (52 * sizeof (void*));
  m_index = 52;
}

CardDeck::~CardDeck (void) {
  free (m_cards);
}

void CardDeck::deinit (void) {
  if (cards != NULL) {
    for (int i = 0; i < 52; ++i) {
      delete cards[i];
    }
    free (cards);
    cards = NULL;
  }
}

const Card* CardDeck::drawCard (void) {
  if (m_index == 52) {
    /* TODO: handle error!  */
  }
  const Card* result = m_cards[m_index];
  ++m_index;
  return result;
}

void CardDeck::init (void) {
  if (cards == NULL) {
    cards = (const Card**) malloc (52 * sizeof (void*));
    for (int i = 0; i < 52; ++i) {
      cards[i] = new Card (i);
    }
  }
}

void CardDeck::shuffle (void) {
  std::vector<const Card*> list;
  list.reserve (52);
  for (int i = 0; i < 52; ++i) {
    list.push_back (cards[i]);
  }
  for (int i = 0; i < 52; ++i) {
    int index = pokerengine::randomInt (52 - i);
    m_cards[i] = list[index];
    list.erase (list.begin () + index);
  }
  m_index = 0;
}
