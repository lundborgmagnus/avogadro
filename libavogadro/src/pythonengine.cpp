/**********************************************************************
  PythonEngine - Engine for python scripts

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

#include "pythonengine.h"

#include <config.h>
#include <avogadro/primitive.h>
#include <avogadro/atom.h>
#include <avogadro/bond.h>
#include <avogadro/molecule.h>
#include <avogadro/color.h>
#include <avogadro/glwidget.h>
#include <avogadro/painterdevice.h>

#include <QMessageBox>
#include <QLayout>
#include <QDebug>

using namespace std;
using namespace Eigen;
using namespace boost::python;

namespace Avogadro {

  class PythonEnginePrivate
  {
    public:
      PythonEnginePrivate() : script(0), settingsWidget(0)
      {}

      PythonInterpreter      interpreter;
      PythonScript          *script;
      boost::python::object  instance;
      QWidget               *settingsWidget;
  };

  PythonEngine::PythonEngine(QObject *parent, const QString &filename) : Engine(parent), d(new PythonEnginePrivate)
  {
    setDescription(tr("Python script rendering")); // FIXME

    loadScript(filename);
  }

  PythonEngine::~PythonEngine()
  {
  }

  Engine* PythonEngine::clone() const
  {
    PythonEngine* engine = new PythonEngine(parent());

    engine->setAlias(alias());
    engine->setEnabled(isEnabled());

    return engine;
  }

  QString PythonEngine::name() const
  {
    if (!PyObject_HasAttrString(d->instance.ptr(), "name"))
      return tr("Unknown Python Engine");
  
    try {
      prepareToCatchError();
      const char *name = extract<const char*>(d->instance.attr("name")());
      return QString(name);
    } catch(error_already_set const &) {
      catchError();
      return tr("Unknown Python Engine");
    }
  }
   
  QString PythonEngine::description() const
  {
    if (!PyObject_HasAttrString(d->instance.ptr(), "description"))
      return tr("N/A");

    try {
      prepareToCatchError();
      const char *desc = extract<const char*>(d->instance.attr("description")());
      return QString(desc);
    } catch(error_already_set const &) {
      catchError();
      return tr("N/A");
    }
  }
 
 
  bool PythonEngine::renderOpaque(PainterDevice *pd)
  {
    if (!d->script)
      return false; // nothing we can do

    boost::python::reference_existing_object::apply<PainterDevice*>::type converter;
    PyObject *obj = converter(pd);
    object real_obj = object(handle<>(obj));

    try {
      prepareToCatchError();
      d->instance.attr("renderOpaque")(real_obj);
    } catch(error_already_set const &) {
      catchError();
    }

    return true;
  }

  QWidget* PythonEngine::settingsWidget()
  {
    if (!d->script)
      return 0; // nothing we can do

    if(!d->settingsWidget)
    {
      d->settingsWidget = new QWidget();

      try {
        prepareToCatchError();
        QWidget *widget = extract<QWidget*>(d->instance.attr("settingsWidget")());
        if (widget)
          d->settingsWidget->layout()->addWidget(widget);
      } catch (error_already_set const &) {
        catchError();
      }

      connect(d->settingsWidget, SIGNAL(destroyed()), this, SLOT(settingsWidgetDestroyed()));
    }

    return d->settingsWidget;
  }

  void PythonEngine::settingsWidgetDestroyed()
  {
    d->settingsWidget = 0;
  }

  void PythonEngine::writeSettings(QSettings &settings) const
  {
    Engine::writeSettings(settings);
  }

  void PythonEngine::readSettings(QSettings &settings)
  {
    Engine::readSettings(settings);
  }

  void PythonEngine::loadScript(const QString &filename)
  {
    QFileInfo info(filename);
    d->interpreter.addSearchPath(info.canonicalPath());

    pythonError()->append(tr("PythonEngine: checking ") + filename + "...");
    
    PythonScript *script = new PythonScript(filename);

    if(script->module()) {
      // make sure there is an Engine class defined
      if (PyObject_HasAttrString(script->module().ptr(), "Engine")) {
        pythonError()->append(tr("  + 'Engine' class found"));

        try {
          prepareToCatchError();
          // instatiate the new Engine
          d->instance = script->module().attr("Engine")();
        } catch (error_already_set const &) {
          catchError();
          return;
        }

        d->script = script;

      } else {
        delete script;
        pythonError()->append(tr("  - script has no 'Engine' class defined"));
      }
    } else {
      delete script;
      pythonError()->append(tr("  - no module"));
    }
  }

  Engine::Layers PythonEngine::layers() const
  {
    if (!d->script)
      return Engine::Opaque; // nothing we can do

    try {
      prepareToCatchError();
      // return layers from python script if the function is defined
      if (PyObject_HasAttrString(d->instance.ptr(), "layers"))
        return extract<Engine::Layers>(d->instance.attr("layers")());
    } catch(error_already_set const &) {
      catchError();
    }

    // return NoFlags, don't print an error, don't want to overwhelm new users with errors
    return Engine::Opaque;
  }

  double PythonEngine::transparencyDepth() const
  {
    if (!d->script)
      return 0.0; // nothing we can do
    
    try {
      prepareToCatchError();
      // return transparencyDepth from python script if the function is defined
      if (PyObject_HasAttrString(d->instance.ptr(), "transparencyDepth"))
        return extract<double>(d->instance.attr("transparencyDepth")());
    } catch(error_already_set const &) {
      catchError();
    }

    return 0.0;
  }


}

#include "pythonengine.moc"
