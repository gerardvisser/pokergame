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

#ifndef POKERENGINE__CARD_INCLUDED
#define POKERENGINE__CARD_INCLUDED

class Card {
private:
  int m_id;
  int m_suit;
  int m_value;

  Card (int id);

public:
  int id (void) const;
  int suit (void) const;
  int value (void) const;

  friend class CardDeck;
};

#endif
