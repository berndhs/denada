#include "delib-debug.h"
#include <stdlib.h>
#include <iostream>
#include <qapplication.h>
#include <QPoint>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>


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


using namespace std;

namespace deliberate
{


static DebugLog *staticLog(0);

void MyOwnMessageOutput (QtMsgType type, const QMessageLogContext &context , const QString& msg)
{
#if DELIBERATE_DEBUG
  switch (type) {
  case QtDebugMsg:
    if (staticLog) {
      staticLog->Log ("Qt Debug: ", msg);
    } else {
      cout << "Qt Debug: " << msg.toStdString() << endl;
    }
    break;
  case QtWarningMsg:
    if (staticLog) {
      staticLog->Log ("Qt Warn: ", msg);
    } else {
      cout << "Qt Warn: " << msg.toStdString() << endl;
    }
    break;
  case QtCriticalMsg:
    if (staticLog) {
      staticLog->Log ("Qt Critical: ", msg);
    } else {
      cout << "Qt Critical: " << msg.toStdString() << endl;
    }
    break;
  case QtFatalMsg:
    cout << "Qt Fatal: " << msg.toStdString() << endl;
    if (staticLog) {
      staticLog->Log ("Qt Fatal: ", msg);
    } else {
      cout << "Qt Fatal: " << msg.toStdString() << endl;
    }
    abort();
    break;
  default:
    cout << " unknown Qt msg type: " << msg.toStdString() << endl;
    if (staticLog) {
      staticLog->Log ("Qt Debug: ", msg);
    } else {
      cout << "Qt Debug: " << msg.toStdString() << endl;
    }
    break;
  }
#else
  switch (type) {
  case QtFatalMsg:
    cout << "Qt Fatal: " << msg.toStdString() << endl;
    abort();
    break;
  case QtDebugMsg:
  case QtWarningMsg:
  case QtCriticalMsg:
  default:
      cout << "Qt Fatal, don't know what to do " << endl;
    break;
  }
#endif

}

void UseMyOwnMessageHandler ()
{
  qInstallMessageHandler (deliberate::MyOwnMessageOutput);
}


void
StartDebugLog (bool gui)
{
  if (staticLog == 0) {
    staticLog = new DebugLog ;
  }
  staticLog->StartLogging ();
  staticLog->UseGui (gui);
  if (gui) {
    staticLog->move (QPoint (0,0));
    staticLog->show ();
  }
}

void
StartFileLog (QString filename)
{
  if (staticLog == 0) {
    staticLog = new DebugLog ;
  }
  staticLog->StartLogging ();
  staticLog->LogToFile (filename);
}

void
StopDebugLog ()
{
  if (staticLog) {
    staticLog->StopLogging ();
    staticLog->hide ();
  }
}

bool
DebugLogRecording ()
{
  if (staticLog) {
    return staticLog->IsLogging();
  } else {
    return false;
  }
}


DebugLog::DebugLog ()
  :QDialog(0),
   isLogging (false),
   logToFile (false)
{
  setupUi (this);
  Connect ();
  hide ();
  clock.start ();
}

DebugLog::DebugLog (QWidget * parent)
  :QDialog (parent),
   isLogging (false),
   logToFile (false)
{
  setupUi (this);
  Connect ();
  hide ();
  clock.start ();
}

DebugLog::~DebugLog ()
{
  logFile.close ();
}

void
DebugLog::Connect ()
{
  connect (closeButton, SIGNAL (clicked()), this, SLOT(Close()));
  connect (stopButton, SIGNAL (clicked()), this, SLOT (StopLogging()));
  connect (startButton, SIGNAL (clicked()), this, SLOT (StartLogging()));
  connect (saveButton, SIGNAL (clicked()), this, SLOT (SaveLog()));
}

void
DebugLog::Close ()
{
  isLogging = false;
  hide ();
}

void
DebugLog::quit ()
{
  Close ();
}

void
DebugLog::closeEvent (QCloseEvent *event)
{
  Q_UNUSED (event)
  Close ();
}

bool
DebugLog::Log (const QString& msg)
{
  if (isLogging && useGui) {
    logBox->append (msg);
    update ();
  }
  if (logToFile) {
    logFile.write (msg.toUtf8());
    logFile.write ("\n");
    logFile.flush ();
  }
  return isLogging;
}

bool
DebugLog::Log (const char* kind, const QString& msg)
{
  QString realMessage (QString(kind) + " " + 
                       QString::number (clock.elapsed()) + " - " + msg);
  if (isLogging && useGui) {
    logBox->append (QString(kind) + " " +
                       QString::number (clock.elapsed()) + " - " + msg);
    update ();
  }
  if (logToFile) {
    logFile.write (realMessage.toUtf8());
    logFile.write ("\n");
    logFile.flush ();
  }
  return isLogging;
}

void
DebugLog::SaveLog ()
{
  QString saveFile = QFileDialog::getSaveFileName (this, tr("Save Log File"),
                     "./debug-log.log",
                     tr("Text Files (*.log *.txt );; All Files (*.*)"));
  if (saveFile.length() > 0) {
    QFile file(saveFile);
    file.open (QFile::WriteOnly);
    file.write (logBox->toPlainText().toLocal8Bit());
    file.close ( );
  }
}

void
DebugLog::LogToFile (QString filename)
{
  logFile.setFileName (filename);
  bool isopen = logFile.open (QFile::WriteOnly);
  logToFile = isopen;

  cout << " log to file " << filename.toStdString() << endl;
  cout << " log file open is " << isopen << endl;
}

int
Hang (int msec, const QString & message)
{
  QMessageBox  box;
  box.setText (message);
  box.setStandardButtons (QMessageBox::Ok 
                        | QMessageBox::Cancel
                        | QMessageBox::Abort);
  if (msec > 0) {
    QTimer::singleShot (msec, &box, SLOT (reject()));
  }
  int response = box.exec ();
qDebug () << " Hang box result " << response;
  switch (response) {
  case QMessageBox::Ok:
    return 1;
  case QMessageBox::Cancel:
    return 0;
  case QMessageBox::Abort:
    abort ();
    break;
  default:
    break;
  }
  return 0;
}


} // namespace
