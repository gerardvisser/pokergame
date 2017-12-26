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

#ifndef POKERENGINE__HAND_INCLUDED
#define POKERENGINE__HAND_INCLUDED

#include <pokerengine/Card.h>

class Hand {
private:
  const Card* m_cards[5];
  int m_cardCount;
  int m_ranking;
  bool m_dirty;

public:
  Hand (void);

  const Card* card (int index) const;
  int ranking (void);
  void setCard (int index, const Card* card);

private:
  void calculateRanking (void);
  bool hasFlush (void) const;
};

#endif
