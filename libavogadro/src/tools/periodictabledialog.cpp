/**********************************************************************
  Copyright (C) 2007 Geoffrey R. Hutchison

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.sourceforge.net/>

  This code is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation version 2.1 of the License.

  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
 ***********************************************************************/

#include "periodictabledialog.h"

namespace Avogadro {

PeriodicTableDialog::PeriodicTableDialog(QWidget *parent)
    : QDialog(parent
#ifdef Q_WS_MAC
            , Qt::Tool
#endif
            )
{
  ui.setupUi(this);

  // Qt Designer only creates visual layout -- here we do logical layout
  // (fortunately, the elements are arranged in atomic number order)
  elementGroup = new QButtonGroup(this);
  unsigned int element = 1;
  foreach(QToolButton *child, findChildren<QToolButton*>()) {
    elementGroup->addButton(child, element++);
    child->setCheckable(true);
  }

  connect(elementGroup, SIGNAL(buttonClicked(int)),
          this, SLOT(buttonClicked(int)));
}

PeriodicTableDialog::~PeriodicTableDialog()
{
}

void PeriodicTableDialog::setSelectedElement(int id)
{
  currentElement = id;
  elementGroup->button(id)->setChecked(true);
}

void PeriodicTableDialog::buttonClicked(int id)
{
  if (currentElement != id) {
    emit elementChanged(id);
    currentElement = id;
  }
}

} // end namespace Avogadro

#include "periodictabledialog.moc"