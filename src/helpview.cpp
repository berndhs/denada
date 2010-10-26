
#include "helpview.h"
/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2010, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#include "delib-debug.h"
#include <QFile>
#include <QString>

namespace deliberate {


 HelpView::HelpView (QWidget *parent)
 :QDialog(parent)
 {
   setupUi (this);
   ConnectButtons ();
   hide();
 }

 HelpView::~HelpView ()
 {
 
 }
 
 void
 HelpView::update ()
 {
   box->update ();
   QWidget::update ();
 }

 void 
 HelpView::ConnectButtons ()
 {
   connect (closeButton, SIGNAL (clicked()), this, SLOT (DoClose()));
   connect (backButton, SIGNAL (clicked()), this, SLOT (DoBack()));
   connect (forwardButton, SIGNAL (clicked()), this, SLOT (DoForward()));
 }

 void
 HelpView::DoClose ()
 {
   hide();
 }
 
 void
 HelpView::DoBack ()
 {
   box->back();
 }
 
 void
 HelpView::DoForward ()
 {
   box->forward();
 }
 
 void
 HelpView::Show (QString urlString)
 {
   QUrl url(urlString);
   box->load (url);
   show ();
 }
 
} // namespace


