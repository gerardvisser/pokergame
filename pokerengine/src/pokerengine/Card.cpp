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

#include <pokerengine/Card.h>

Card::Card (int id) : m_id (id) {
  m_value = id % 13;
  if (m_value == 0)
    m_value = 12;
  else
    --m_value;
  m_suit = id / 13;
  /* 0: Clubs, 1: Spades, 2: Hearts, 3: Diamonds */
}

int Card::id (void) const {
  return m_id;
}

int Card::suit (void) const {
  return m_suit;
}

int Card::value (void) const {
  return m_value;
}
