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

#include <stddef.h>
#include "PlayerView.h"
#include "pixmaps.h"

#include <QFont>

#define CARD_HEIGHT 96
#define CARD_WIDTH  71

static QPixmap getCardImage (const Card* card) {
  if (card == NULL) {
    return pixmaps::greyCardBackImage ();
  }
  return pixmaps::cardImage (card->id ());
}

PlayerView::PlayerView (const Player* player, QWidget* parent) : QFrame (parent), m_player (player) {
  setFixedSize (650, 100);

  for (int i = 0; i < 5; ++i) {
    wm_cardViews[i] = new QLabel (this);
    wm_cardViews[i]->setPixmap (getCardImage (player->card (i)));
    wm_cardViews[i]->setGeometry (i * (CARD_WIDTH + 5), 0, CARD_WIDTH, CARD_HEIGHT);
  }

  QFont font ("FreeSans", 12);
  wm_name = new QLabel (this);
  wm_name->setText (player->name ());
  wm_name->move (5 * (CARD_WIDTH + 5) + 5, 0);
  wm_name->setFont (font);

  wm_money = new QLabel (this);
  wm_money->move (5 * (CARD_WIDTH + 5) + 5 + 200, 0);
  wm_money->setFont (font);

  QString str;
  str.sprintf ("\342\202\254 %d", 1200);
  wm_money->setText (str);

  //wm_name->setStyleSheet ("background-color: #A0FFA0;");

  //setStyleSheet (".PlayerView {border: 2px solid #E00000;}");
}

void PlayerView::updateCardViews (void) {
  for (int i = 0; i < 5; ++i) {
    wm_cardViews[i]->setPixmap (getCardImage (m_player->card (i)));
  }
}
