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
#include <string.h>
#include <pokerengine/Player.h>

Player::Player (const char* name, int money) : m_active (false), m_money (money), m_totalBet (0) {
  m_hand = new Hand ();
  int len = strlen (name) + 1;
  m_name = (char*) malloc (len);
  memcpy (m_name, name, len);
}

Player::~Player (void) {
  delete m_hand;
  free (m_name);
}

void Player::addMoney (int val) {
  m_money += val;
}

const Card* Player::card (int index) const {
  return m_hand->card (index);
}

void Player::clearTotalBet (void) {
  m_totalBet = 0;
}

int Player::handRanking (void) const {
  return m_hand->ranking ();
}

void Player::increaseTotalBet (int val) {
  m_totalBet += val;
}

bool Player::isActive (void) const {
  return m_active;
}

int Player::money (void) const {
  return m_money;
}

const char* Player::name (void) const {
  return m_name;
}

void Player::newHand (CardDeck* deck) {
  for (int i = 0; i < 5; ++i) {
    m_hand->setCard (i, deck->drawCard ());
  }
}

void Player::removeMoney (int val) {
  m_money -= val;
}

void Player::setActive (bool val) {
  m_active = val;
}

void Player::setCard (int index, const Card* card) {
  m_hand->setCard (index, card);
}

int Player::totalBet (void) const {
  return m_totalBet;
}
