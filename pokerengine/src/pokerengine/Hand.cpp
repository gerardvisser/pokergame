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

#include <stddef.h>
#include <string.h>
#include <pokerengine/Hand.h>

#define ZERO     1
#define ONE      13
#define TWO      169
#define THREE    2197
#define FOUR     28561
#define CATEGORY 371293

#define NONE             0
#define ONE_PAIR         1
#define TWO_PAIR         2
#define THREE_OF_A_KIND  3
#define STRAIGHT         4
#define FLUSH            5
#define FULL_HOUSE       6
#define FOUR_OF_A_KIND   7
#define STRAIGHT_FLUSH   8

static int cardValueForCount (int count, const int* cardValueCount, int fromIndex = 13);
static bool hasStraight (const int* cardValueCount, int startIndex);

Hand::Hand (void) {
  for (int i = 0; i < 5; ++i) {
    m_cards[i] = NULL;
  }
  m_cardCount = 0;
  m_dirty = true;
}

void Hand::calculateRanking (void) {
  if (m_cardCount < 5) {
    m_ranking = 0;
    return;
  }
  int combination = 0;
  int cardValueCount[13];
  memset (cardValueCount, 0, 13 * sizeof (int));
  for (int i = 0; i < 5; ++i) {
    int vcIndex = m_cards[i]->value ();
    ++cardValueCount[vcIndex];
    if (cardValueCount[vcIndex] >= 2) {
      if (cardValueCount[vcIndex] >= 3) {
        combination += 2;
      } else {
        ++combination;
      }
    }
  }

  int category, v;
  switch (combination) {
  case 1:
    category = ONE_PAIR;
    m_ranking = cardValueForCount (2, cardValueCount) * FOUR;
    v = cardValueForCount (1, cardValueCount);
    m_ranking += v * THREE;
    v = cardValueForCount (1, cardValueCount, v);
    m_ranking += v * TWO;
    v = cardValueForCount (1, cardValueCount, v);
    m_ranking += v * ONE;
    break;

  case 2:
    category = TWO_PAIR;
    v = cardValueForCount (2, cardValueCount);
    m_ranking = v * FOUR;
    v = cardValueForCount (2, cardValueCount, v);
    m_ranking += v * THREE;
    v = cardValueForCount (1, cardValueCount);
    m_ranking += v * TWO;
    break;

  case 3:
    category = THREE_OF_A_KIND;
    m_ranking = cardValueForCount (3, cardValueCount) * FOUR;
    v = cardValueForCount (1, cardValueCount);
    m_ranking += v * THREE;
    v = cardValueForCount (1, cardValueCount, v);
    m_ranking += v * TWO;
    break;

  case 4:
    category = FULL_HOUSE;
    m_ranking = cardValueForCount (3, cardValueCount) * FOUR;
    m_ranking += cardValueForCount (2, cardValueCount) * THREE;
    break;

  case 5:
    category = FOUR_OF_A_KIND;
    m_ranking = cardValueForCount (4, cardValueCount) * FOUR;
    m_ranking += cardValueForCount (1, cardValueCount) * THREE;
    break;

  default:
    v = cardValueForCount (1, cardValueCount);
    m_ranking = v * FOUR;
    v = cardValueForCount (1, cardValueCount, v);
    m_ranking += v * THREE;
    v = cardValueForCount (1, cardValueCount, v);
    m_ranking += v * TWO;
    v = cardValueForCount (1, cardValueCount, v);
    m_ranking += v * ONE;
    v = cardValueForCount (1, cardValueCount, v);
    m_ranking += v * ZERO;
    if (hasStraight (cardValueCount, v)) {
      if (hasFlush ()) {
        category = STRAIGHT_FLUSH;
      } else {
        category = STRAIGHT;
      }
    } else if (hasFlush ()) {
      category = FLUSH;
    } else {
      category = NONE;
    }
  }
  m_ranking += category * CATEGORY;
}

const Card* Hand::card (int index) const {
  if (index < 0 || index > 4) {
    /* TODO: handle error.  */
  }
  return m_cards[index];
}

bool Hand::hasFlush (void) const {
  int i = 1;
  while (i < 5 && m_cards[i - 1]->suit () == m_cards[i]->suit ()) {
    ++i;
  }
  return i == 5;
}

int Hand::ranking (void) {
  if (m_dirty) {
    calculateRanking ();
    m_dirty = false;
  }
  return m_ranking;
}

void Hand::setCard (int index, const Card* card) {
  if (index < 0 || index > 4) {
    /* TODO: handle error.  */
  }
  if (card == NULL) {
    if (m_cards[index] != NULL) {
      m_dirty = true;
      --m_cardCount;
    }
  } else {
    if (m_cards[index] != NULL) {
      if (m_cards[index]->id () != card->id ())
        m_dirty = true;
    } else {
      m_dirty = true;
      ++m_cardCount;
    }
  }
  m_cards[index] = card;
}

static int cardValueForCount (int count, const int* cardValueCount, int fromIndex) {
  int i = fromIndex - 1;
  while (i > -1 && cardValueCount[i] != count) {
    --i;
  }
  return i;
}

static bool hasStraight (const int* cardValueCount, int startIndex) {
  int index = startIndex;
  while (index < 13 && cardValueCount[index] == 1) {
    ++index;
  }
  return index - startIndex == 5;
}
