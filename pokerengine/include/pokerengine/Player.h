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

#ifndef POKERENGINE__PLAYER_INCLUDED
#define POKERENGINE__PLAYER_INCLUDED

#include <pokerengine/CardDeck.h>
#include <pokerengine/Hand.h>

class Player {
private:
  char* m_name;
  Hand* m_hand;
  int m_totalBet;
  int m_money;
  bool m_active;

public:
  Player (const char* name, int money = 1200);
  Player (const Player&) = delete;
  Player (Player&&) = delete;
  virtual ~Player (void);

  Player& operator= (const Player&) = delete;
  Player& operator= (Player&&) = delete;

  void addMoney (int val);
  const Card* card (int index) const;
  void clearTotalBet (void);
  int handRanking (void) const;
  bool isActive (void) const;
  int money (void) const;
  const char* name (void) const;
  void newHand (CardDeck* deck);
  void removeMoney (int val);
  void setActive (bool val);
  void setCard (int index, const Card* card);
  int totalBet (void) const;

  virtual int getBet (int callAmount, bool canRaise) = 0;
  virtual bool isHuman (void) const = 0;

protected:
  void increaseTotalBet (int val);
};

#endif
