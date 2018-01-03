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

#include <stddef.h>
#include <pokerengine/HumanPlayer.h>

HumanPlayer::HumanPlayer (const char* name) : Player (name) {
}

HumanPlayer::~HumanPlayer (void) {
}

void HumanPlayer::call (void) {
  m_raise = 0;
}

const std::vector<int>& HumanPlayer::cardsToReplace (void) {
  return *m_cardsToReplace;
}

void HumanPlayer::clearCardsToReplace (void) {
  m_cardsToReplace->clear ();
}

int HumanPlayer::getBet (int callAmount, bool canRaise) {
  if (m_raise == -1)
    return 0;
  int result = callAmount + m_raise;
  increaseTotalBet (result);
  return result;
}

bool HumanPlayer::isHuman (void) const {
  return true;
}

int HumanPlayer::raise (void) {
  if (m_raise < 1)
    m_raise = 1;
  else
    ++m_raise;
  return m_raise;
}

void HumanPlayer::resetRaise (void) {
  m_raise = -1;
}

bool HumanPlayer::replaceCard (int index) {
  if (card (index) != NULL) {
    m_cardsToReplace->push_back (index);
    setCard (index, NULL);
    return true;
  }
  return false;
}
