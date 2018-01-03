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

#include "ComputerPlayer.h"
#include "util/random.h"

ComputerPlayer::ComputerPlayer (const char* name) : Player (name) {
}

ComputerPlayer::~ComputerPlayer (void) {
}

const std::vector<int>& ComputerPlayer::cardsToReplace (void) {
  /* TODO: To be improved.  */
  return *m_cardsToReplace;
}

int ComputerPlayer::getBet (int callAmount, bool canRaise) {
  /* TODO: To be improved.  */
  int result = pokerengine::randomInt (4 + callAmount);
  if (result < callAmount)
    return 0;
  if (!canRaise)
    result = callAmount;
  increaseTotalBet (result);
  return result;
}

bool ComputerPlayer::isHuman (void) const {
  return false;
}
