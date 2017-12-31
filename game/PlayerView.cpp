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

#define PADDING_LEFT 2
#define PADDING_TOP  2


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
    wm_cardViews[i]->setGeometry (PADDING_LEFT + i * (CARD_WIDTH + 5), PADDING_TOP, CARD_WIDTH, CARD_HEIGHT);
  }

  QFont font ("FreeSans", 12);
  wm_name = new QLabel (this);
  wm_name->setText (player->name ());
  wm_name->move (PADDING_LEFT + 5 * (CARD_WIDTH + 5) + 5, PADDING_TOP);
  wm_name->setFont (font);

  wm_money = new QLabel (this);
  wm_money->move (PADDING_LEFT + 5 * (CARD_WIDTH + 5) + 5 + 200, PADDING_TOP);
  wm_money->setFont (font);

  QString str;
  str.sprintf ("\342\202\254 %d", player->money ());
  wm_money->setText (str);

  wm_action = new QLabel (this);
  wm_action->setFixedWidth (200);
  wm_action->move (PADDING_LEFT + 5 * (CARD_WIDTH + 5) + 5, PADDING_TOP + 18);
  wm_action->setFont (font);

  // /*wm_name->*/setStyleSheet ("background-color: #A0FFA0;");
  //setStyleSheet ("background-color: #F0F0F0;");

  //setStyleSheet (".PlayerView {border: 2px solid #000000;}");
}

void PlayerView::updateAction (QString str) {
  wm_action->setText (str);
}

void PlayerView::updateCardViews (void) {
  for (int i = 0; i < 5; ++i) {
    wm_cardViews[i]->setPixmap (getCardImage (m_player->card (i)));
  }
}
