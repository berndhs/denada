#include "denada.h"

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

#include <QApplication>
#include "deliberate.h"
#include "version.h"
#include "helpview.h"
#include <QSize>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QCursor>

using namespace deliberate;

namespace denada
{

DeNada::DeNada (QWidget *parent)
  :QMainWindow (parent),
   initDone (false),
   app (0),
   configEdit (this),
   helpView (0),
   runAgain (false)
{
  mainUi.setupUi (this);
  helpView = new HelpView (this);
  Connect ();
}

void
DeNada::Init (QApplication &ap)
{
  app = &ap;
  connect (app, SIGNAL (lastWindowClosed()), this, SLOT (Exiting()));
  Settings().sync();
  initDone = true;
}

bool
DeNada::Again ()
{
  bool again = runAgain;
  runAgain = false;
  return again;
}

bool
DeNada::Run ()
{
  runAgain = false;
  if (!initDone) {
    Quit ();
    return false;
  }
  qDebug () << " Start DeNada";
  QSize defaultSize = size();
  QSize newsize = Settings().value ("sizes/main", defaultSize).toSize();
  resize (newsize);
  Settings().setValue ("sizes/main",newsize);
  show ();
  return true;
}

void
DeNada::Connect ()
{
  connect (mainUi.actionQuit, SIGNAL (triggered()), 
           this, SLOT (Quit()));
  connect (mainUi.actionSettings, SIGNAL (triggered()),
           this, SLOT (EditSettings()));
  connect (mainUi.actionAbout, SIGNAL (triggered()),
           this, SLOT (About ()));
  connect (mainUi.actionLicense, SIGNAL (triggered()),
           this, SLOT (License ()));
  connect (mainUi.actionRestart, SIGNAL (triggered()),
           this, SLOT (Restart ()));
}

void
DeNada::Restart ()
{
  qDebug () << " restart called ";
  runAgain = true;
  Quit ();
}


void
DeNada::Quit ()
{
  CloseCleanup ();
  if (app) {
    app->quit();
  }
}

void
DeNada::closeEvent (QCloseEvent *event)
{
  Q_UNUSED (event)
  CloseCleanup ();
}

void
DeNada::CloseCleanup ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
}

void
DeNada::EditSettings ()
{
  configEdit.Exec ();
  SetSettings ();
}

void
DeNada::SetSettings ()
{
  Settings().sync ();
}

void
DeNada::About ()
{
  QString version (deliberate::ProgramVersion::Version());
  QStringList messages;
  messages.append (version);
  messages.append (configMessages);

  QMessageBox  box;
  box.setText (version);
  box.setDetailedText (messages.join ("\n"));
  QTimer::singleShot (30000, &box, SLOT (accept()));
  box.exec ();
}

void
DeNada::Exiting ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
}

void
DeNada::License ()
{
  if (helpView) {
    helpView->Show ("qrc:/help/LICENSE.txt");
  }
}


} // namespace

