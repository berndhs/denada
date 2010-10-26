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

#include "config-edit.h"
#include "delib-debug.h"
#include "deliberate.h"
#include <QTreeWidgetItem>
#include <QList>

namespace deliberate {

ConfigEdit::ConfigEdit (QWidget * parent)
:QDialog (parent),
 configModel (0)
{
  setupUi (this);

  connect (cancelButton, SIGNAL (clicked()), this, SLOT (Cancel()));
  connect (saveButton, SIGNAL (clicked()), this, SLOT (Save()));
  configModel = new QStandardItemModel (this);
  configView->setModel (configModel);

  QStandardItem * head = new QStandardItem ("Key");
  configModel->setHorizontalHeaderItem (0,head);
  head = new QStandardItem ("Value");
  configModel->setHorizontalHeaderItem (1,head);

  exemptGroups << "sizes" ;
}

ConfigEdit::~ConfigEdit ()
{
}

void
ConfigEdit::Cancel ()
{
  Done (false);
}

void
ConfigEdit::Exec ()
{
  Load ();
  exec ();
}

void
ConfigEdit::Run ()
{
  Load ();
  show ();
}

void
ConfigEdit::Done (bool saved)
{
  if (saved) {
    accept ();
  } else {
    reject ();
  }
  emit Finished (saved);
}


void
ConfigEdit::Load ()
{
  int nr = configModel->rowCount();
  if (nr > 0) {
    configModel->removeRows (0,nr);
  }
  Qt::ItemFlags  flags;
  QSettings & Zett = Settings();
  QString   name, data;
  fileName->setText (Zett.fileName());
  QStringList top = Zett.childKeys ();
  QStringList::iterator topit;
  QStandardItem * keyItem, *dataItem;
  QList <QStandardItem*> itemList;

  name = QString ("program");
  data = Zett.value (name).toString(); 
  keyItem = new QStandardItem (name);
  keyItem->setEditable (false);
  itemList.clear();
  itemList << keyItem;
  dataItem = new QStandardItem (data);
  dataItem->setEditable (false);
  itemList << dataItem;
  configModel->appendRow (itemList);

  QStringList groups = Zett.childGroups();
  QStringList::iterator  grit, subit;
  for (grit = groups.begin(); grit != groups.end(); grit++) {
    if (exemptGroups.contains (*grit)) {
      continue;
    }
    Zett.beginGroup (*grit);
    QStandardItem *groupItem = new QStandardItem (*grit);
    configModel->appendRow (groupItem);  
    QStringList subs = Zett.childKeys ();
    for (subit = subs.begin(); subit != subs.end(); subit++) {
      QList<QStandardItem*> subList;
      name = *subit;
      data = Zett.value(name).toString();  
      keyItem = new QStandardItem (name);
      keyItem->setEditable (false);
      subList << keyItem;
      dataItem = new QStandardItem (data);
      dataItem->setEditable (true);
      subList << dataItem;
      groupItem->appendRow (subList);
    }
    Zett.endGroup ();
  } 
  update ();
}

void
ConfigEdit::Save ()
{
  QSettings & Zett = Settings();
  int nr = configModel->rowCount();
  QStandardItem * rowhead, * keyItem, * valueItem;
  QModelIndex     index;
  for (int r = 0; r< nr; r++) {
    rowhead = configModel->item (r,0);
    if (rowhead) {
      QString prefix = rowhead->text();
      QString key, value;
      index = configModel->indexFromItem (rowhead);
      if (configModel->hasChildren (index)) {
        int row;
        for (row = 0; row < rowhead->rowCount(); row++) {
          keyItem = rowhead->child (row,0);
          valueItem = rowhead->child (row,1);
          if (keyItem && valueItem) {
            key = prefix + "/" + keyItem->text ();
            value = valueItem->text();
            Zett.setValue (key,value);
          }
        }
      }
    }
  }
  Zett.sync();
  Done (true);
}


} // namespace
