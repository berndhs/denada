#ifndef DELIB_DEBUG_H
#define DELIB_DEBUG_H



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

//
// Usage:
//
// - Static Log:
//
//    Make sure DELIBERATE_DEBUG is set to a non-zero value , e.g.
//    #define DELIBERATE_DEBUG=1
//
//    The static log will capture all messages that go to qDebug().
//
//    When you want to start seeing messages in the log (for example,
//    at the beginning of your program execution), call
//      StartDebugLog ();
//
//    When you cant to stop logging, simply call
//      StopDebugLog();
//
// - Dynamic Logging
//
//   Make sure DELIBERATE_DEBUG is set to a non-zeo value, e.g.
//   #define DELIBERATE_DEBUG=1
//
//   You can allocate as many DebugLog objects as you like, e.g.
//     DebugLog leftLog;
//     DebugLog rightLog;
//     DebugLog middleLog (this);  // where "this" points to a QWidget
//
//   These DebugLog objects will *not* capture messages from qDebug(),
//   only from calls to the Log(char*) function.
//
//   You can turn the individual logs on and off any time you want,
//   independent of each other:
//     leftLog.StartLogging ();
//     rightLog.StopLogging ();
//

#include <qapplication.h>
#include <iostream>
#include "ui_DebugLog.h"

#include <QDebug>
#include <QCloseEvent>
#include <QFile>
#include <QTime>

namespace deliberate
{

void UseMyOwnMessageHandler ();

void StartDebugLog (bool gui=true);
void StartFileLog (QString filename);
void StopDebugLog ();
bool DebugLogRecording ();

void MyOwnMessageOutput (QtMsgType type, const char* msg);

int Hang (int msec = -1, const QString & message = QString("Hanging"));

/** \brief DebugLog - Facilities for displaying and storing a log */

class DebugLog : public QDialog, public Ui_LogDialog
{
  Q_OBJECT

public:

  DebugLog (QWidget * parent);
  DebugLog ();
  ~DebugLog ();

  void LogToFile (QString filename);

  bool Log (const QString & msg);
  bool Log (const char * kind, const QString & msg);
  void closeEvent (QCloseEvent *event);

public slots:

  void Close ();
  void quit ();
  void StopLogging () {
    isLogging = false;
  }
  void StartLogging () {
    isLogging = true;
  }
  bool IsLogging () {
    return isLogging;
  }
  bool IsUsingGui () {
    return useGui;
  }
  bool UseGui (bool gui=true) {
    useGui=gui;
    return gui;
  }
  void SaveLog ();

private slots:


private:

  void  Connect ();

  bool  isLogging;
  bool  useGui;
  bool  logToFile;
  QFile logFile;
  QTime clock;

};


}
#endif
