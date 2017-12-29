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
#include <pokerengine/Game.h>
#include <pokerengine/HumanPlayer.h>
#include "ComputerPlayer.h"

#define CHIP_VALUE   12
#define PLAYER_COUNT 4

Game::Game (void) {
  m_cardDeck = new CardDeck ();
  m_players[0] = new ComputerPlayer ("Gene");
  m_players[1] = new ComputerPlayer ("Otto");
  m_players[2] = new ComputerPlayer ("Frank");
  m_players[3] = new HumanPlayer ("Whitey");
  m_dealerIndex = 0;
  m_pot = 0;
}

Game::~Game (void) {
  for (int i = 0; i < PLAYER_COUNT; ++i) {
    delete m_players[i];
  }
  delete m_cardDeck;
}

void Game::addToMaxBet (int val) {
  m_maxBet += val;
}

void Game::deal (void) {
  m_cardDeck->shuffle ();
  for (int i = 0; i < PLAYER_COUNT; ++i) {
    m_players[i]->removeMoney (CHIP_VALUE);
    m_pot += CHIP_VALUE;
    m_players[i]->newHand (m_cardDeck);
    m_players[i]->setActive (true);
    m_players[i]->clearTotalBet ();
  }
  m_maxBet = 0;
  ++m_dealerIndex;
  m_dealerIndex %= PLAYER_COUNT;
}

const Player* Game::dealer (void) const {
  return m_players[m_dealerIndex];
}

int Game::maxBet (void) const {
  return m_maxBet;
}

const Player* const * Game::players (void) const {
  return m_players;
}

int Game::pot (void) const {
  return m_pot;
}
