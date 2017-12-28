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

#include "PokerTable.h"
#include <pokerengine/CardDeck.h>

#include <stdio.h>
//#include <unistd.h>

PokerTable::PokerTable (Game* game, QWidget* parent) : QWidget (parent), wm_game (game) {
  const int playerViewHeight = 106;

  for (int i = 0; i < 4; ++i) {
    wm_playerView[i] = new PlayerView (game->players ()[i], this);
    wm_playerView[i]->move (20, playerViewHeight * i + 20);
  }

  wm_deal = new QPushButton ("Deal", this);
  wm_deal->setGeometry (20, playerViewHeight * 4 + 25, 100, 30);

  wm_bet = new QPushButton ("Bet/Raise", this);
  wm_bet->setGeometry (20 + 100 + 5, playerViewHeight * 4 + 25, 100, 30);
  wm_bet->setEnabled (false);

  wm_call = new QPushButton ("Call", this);
  wm_call->setGeometry (20 + 2 * (100 + 5), playerViewHeight * 4 + 25, 100, 30);
  wm_call->setEnabled (false);

  wm_done = new QPushButton ("Done", this);
  wm_done->setGeometry (20 + 3 * (100 + 5), playerViewHeight * 4 + 25, 100, 30);
  wm_done->setEnabled (false);

  /*QObject::*/connect (wm_deal, SIGNAL (clicked (bool)), this, SLOT (onDealClicked (bool)));
  QObject::connect (wm_bet, SIGNAL (clicked (bool)), this, SLOT (onBetClicked (bool)));
  QObject::connect (wm_call, SIGNAL (clicked (bool)), this, SLOT (onCallClicked (bool)));
  QObject::connect (wm_done, SIGNAL (clicked (bool)), this, SLOT (onDoneClicked (bool)));

  QSize qsize = this->size ();
  printf ("WIDTH=%d, HEIGHT=%d\n", qsize.width (), qsize.height ());
}

PokerTable::~PokerTable (void) {
}

void PokerTable::onBetClicked (bool checked) {
  printf ("Bet/Raise clicked.\n");
}

void PokerTable::onCallClicked (bool checked) {
  printf ("Call clicked.\n");
}

void PokerTable::onDealClicked (bool checked) {
  printf ("Deal clicked.\n");

  wm_game->deal ();
  for (int i = 0; i < 4; ++i) {
    wm_playerView[i]->updateCardViews ();
  }

//  wm_deal->setEnabled (false);
//  static int count = 0;
//  ++count;
//  int id = count;
//  printf ("Deal clicked (id=%d).\n", id);

//  wm_game->deal ();
//  for (int i = 0; i < 4; ++i) {
//    wm_playerView[i]->updateCardViews ();
//  }
//  sleep (5);
//  printf ("Exiting deal (id=%d)\n", id);
}

void PokerTable::onDoneClicked (bool checked) {
  printf ("Done clicked.\n");
}
