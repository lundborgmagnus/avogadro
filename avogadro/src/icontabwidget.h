/**********************************************************************
  icontabwidget.h - IconTabWidget that only displays icons

  Copyright (C) 2007 Donald Ephraim Curtis <dcurtis3@sourceforge.net>

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.openmolecules.net/>

  Avogadro is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Avogadro is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
 **********************************************************************/

#ifndef ICONTABWIDGET_H
#define ICONTABWIDGET_H

#include <QTabBar>
#include <QTabWidget>

namespace Avogadro {

  class IconTabWidget : public QTabWidget
  {
    Q_OBJECT

    public:
      IconTabWidget(QWidget *parent=0);
  };

  class IconTabBar : public QTabBar
  {
    Q_OBJECT
    public:
      IconTabBar(QWidget *parent=0);

    protected:
      virtual QSize tabSizeHint(int index) const;
  };
}

#endif // ICONTABWIDGET_H

