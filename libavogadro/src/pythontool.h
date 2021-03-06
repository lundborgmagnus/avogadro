/**********************************************************************
  PythonTool - PythonTool Tool for Avogadro

  Copyright (C) 2008 Tim Vandermeersch

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

#ifndef PYTHONTOOL_H
#define PYTHONTOOL_H

#include <avogadro/global.h>
#include <avogadro/glwidget.h>
#include <avogadro/tool.h>
#include <avogadro/pythoninterpreter.h>
#include <avogadro/pythonscript.h>

#include <QGLWidget>
#include <QObject>
#include <QAction>

namespace Avogadro {
  
  class PythonToolPrivate;

  class PythonTool : public Tool
  {
    Q_OBJECT

    public:
      //! Constructor
      PythonTool(QObject *parent = 0, const QString &filename = QString(""));
      //! Destructor
      virtual ~PythonTool();

      //! \name To python delegated functions
      //@{
      QString name() const;
      QString description() const; 
      QUndoCommand* mouseEvent(const QString &what, GLWidget *widget, QMouseEvent *event);
      QUndoCommand* mousePressEvent(GLWidget *widget, QMouseEvent *event);
      QUndoCommand* mouseReleaseEvent(GLWidget *widget, QMouseEvent *event);
      QUndoCommand* mouseMoveEvent(GLWidget *widget, QMouseEvent *event);
      QUndoCommand* wheelEvent(GLWidget *widget, QWheelEvent *event);
      bool paint(GLWidget *widget);
      QWidget *settingsWidget();
      //@}

    private:
      void loadScript(const QString &filename);

      PythonToolPrivate * const d;

    private Q_SLOTS:
      void settingsWidgetDestroyed();
  };

  class PythonToolFactory : public QObject, public PluginFactory
  {
    Q_OBJECT
    Q_INTERFACES(Avogadro::PluginFactory)

    public:
      PythonToolFactory(const QString &filename) : m_filename(filename)
      {
        PythonTool tool(0, filename);
        m_name = tool.name();
        m_desc = tool.description();
      }
      Plugin *createInstance(QObject *parent = 0) 
      { 
        return new PythonTool(parent, m_filename); 
      }
      Plugin::Type type() const { return Plugin::ToolType; }
      QString name() const { return m_name; }
      QString description() const { return m_desc; }
    private:
      QString m_filename;
      QString m_name, m_desc;
  };



} // end namespace Avogadro

#endif
