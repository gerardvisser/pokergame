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

#ifndef POKERENGINE__HUMAN_PLAYER_INCLUDED
#define POKERENGINE__HUMAN_PLAYER_INCLUDED

#include <pokerengine/Player.h>

class HumanPlayer : public Player {
private:
  int m_raise;

public:
  HumanPlayer (const char* name);
  virtual ~HumanPlayer (void);

  void call (void);
  const std::vector<int>& cardsToReplace (void) override;
  void clearCardsToReplace (void);
  int getBet (int callAmount, bool canRaise) override;
  bool isHuman (void) const override;
  int raise (void);
  void resetRaise (void);
  bool replaceCard (int index);
};

#endif
