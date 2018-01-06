/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2017, 2018 Gerard Visser.

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

#ifndef POKERENGINE__GAME_INCLUDED
#define POKERENGINE__GAME_INCLUDED

#include <pokerengine/Player.h>

class Game {
private:
  CardDeck* m_cardDeck;
  Player* m_players[4];
  const int m_chipValue;
  int m_dealerIndex;
  int m_maxBet;
  int m_pot;

public:
  Game (void);
  Game (const Game&) = delete;
  Game (Game&&) = delete;
  ~Game (void);

  Game& operator= (const Game&) = delete;
  Game& operator= (Game&&) = delete;

  void addToMaxBet (int val);
  void addToPot (int val);
  CardDeck& cardDeck (void) const;
  int chipValue (void) const;
  void deal (void);
  const Player* dealer (void) const;
  std::vector<const Player*>* determineWinners (void);
  int maxBet (void) const;
  Player* const * players (void) const;
  int pot (void) const;

  friend class BetIterator;
  friend class CardDrawIterator;
};

#endif
