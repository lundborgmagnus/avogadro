/**********************************************************************
  RibbonEngine - Engine for "ribbon" display

  Copyright (C) 2007-2008 by Marcus D. Hanwell

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

#include "ribbonengine.h"

#include <config.h>
#include <avogadro/molecule.h>
#include <avogadro/atom.h>
#include <avogadro/residue.h>
#include <avogadro/color.h>
#include <avogadro/glwidget.h>
#include <avogadro/painterdevice.h>

#include <avogadro/protein.h>

#include <QMessageBox>
#include <QString>
#include <QDebug>

using Eigen::Vector3d;

namespace Avogadro {

  RibbonEngine::RibbonEngine(QObject *parent) : Engine(parent),
    m_settingsWidget(0), m_type(0), m_radius(1.0), m_useNitrogens(2)
  {
    setDescription(tr("Renders residues as ribbons"));

    // Initialise variables
    m_update = true;
    // Pretty colours for the chains - we can add more. Need a colour picker...
    m_chainColors.push_back(Color(1., 0., 0.));
    m_chainColors.push_back(Color(0., 1., 0.));
    m_chainColors.push_back(Color(0., 0., 1.));
    m_chainColors.push_back(Color(1., 0., 1.));
    m_chainColors.push_back(Color(1., 1., 0.));
    m_chainColors.push_back(Color(0., 1., 1.));
  }

  Engine *RibbonEngine::clone() const
  {
    RibbonEngine *engine = new RibbonEngine(parent());
    engine->setAlias(alias());
    engine->m_type = m_type;
    engine->m_radius = m_radius;
    engine->setUseNitrogens(m_useNitrogens);
    engine->setEnabled(isEnabled());

    return engine;
  }

  RibbonEngine::~RibbonEngine()
  {
    // Delete the settings widget if it exists
    if(m_settingsWidget)
      m_settingsWidget->deleteLater();
  }

  bool RibbonEngine::renderOpaque(PainterDevice *pd)
  {
    // Check if the chains need updating before drawing them
    if (m_update) updateChains(pd);

    pd->painter()->setColor(&m_chainColors[0]);
    for (int i = 0; i < m_helixes3.size(); ++i)
      pd->painter()->drawCylinder(m_helixes3[i][0], m_helixes3[i][1], 2.3);
    pd->painter()->setColor(&m_chainColors[1]);
    for (int i = 0; i < m_helixes4.size(); ++i)
      pd->painter()->drawCylinder(m_helixes4[i][0], m_helixes4[i][1], 2.3);

    if (m_type == 0) {
      for (int i = 0; i < m_chains.size(); i++) {
        if (m_chains[i].size() <= 1)
          continue;
        pd->painter()->setColor(&m_chainColors[i % m_chainColors.size()]);
        pd->painter()->drawSpline(m_chains[i], m_radius);
      }
    }
    else {
      // Render cylinders between the points and spheres at each point
      for (int i = 0; i < m_chains.size(); i++) {
        if (m_chains[i].size() <= 1)
          continue;
        pd->painter()->setColor(&m_chainColors[i % m_chainColors.size()]);
        pd->painter()->drawSphere(&m_chains[i][0], m_radius);
        for (int j = 1; j < m_chains[i].size(); j++) {
          pd->painter()->drawSphere(&m_chains[i][j], m_radius);
          pd->painter()->drawCylinder(m_chains[i][j-1], m_chains[i][j], m_radius);
        }
      }
    }

    return true;
  }

  bool RibbonEngine::renderQuick(PainterDevice *pd)
  {
    pd->painter()->setColor(&m_chainColors[0]);
    for (int i = 0; i < m_helixes3.size(); ++i)
      pd->painter()->drawCylinder(m_helixes3[i][0], m_helixes3[i][1], 2.3);
    pd->painter()->setColor(&m_chainColors[1]);
    for (int i = 0; i < m_helixes4.size(); ++i)
      pd->painter()->drawCylinder(m_helixes4[i][0], m_helixes4[i][1], 2.3);

    // Just render cylinders between the backbone...
    double tRadius = m_radius / 2.0;
    for (int i = 0; i < m_chains.size(); i++) {
      if (m_chains[i].size() <= 1)
        continue;
      pd->painter()->setColor(&m_chainColors[i % m_chainColors.size()]);
      pd->painter()->drawSphere(&m_chains[i][0], tRadius);
      for (int j = 1; j < m_chains[i].size(); j++) {
        pd->painter()->drawSphere(&m_chains[i][j], tRadius);
        pd->painter()->drawCylinder(m_chains[i][j-1], m_chains[i][j], tRadius);
      }
    }

    return true;
  }

  double RibbonEngine::radius(const PainterDevice *, const Primitive *) const
  {
    return m_radius;
  }

  void RibbonEngine::setPrimitives(const PrimitiveList &primitives)
  {
    Engine::setPrimitives(primitives);
    m_update = true;
  }

  void RibbonEngine::addPrimitive(Primitive *primitive)
  {
    Engine::addPrimitive(primitive);
    m_update = true;
  }

  void RibbonEngine::updatePrimitive(Primitive *)
  {
    m_update = true;
  }

  void RibbonEngine::removePrimitive(Primitive *primitive)
  {
    Engine::removePrimitive(primitive);
    m_update = true;
  }

  void RibbonEngine::updateChains(PainterDevice *pd)
  {
    if (!isEnabled()) return;
    // Get a list of residues for the molecule
    const Molecule *molecule = pd->molecule();
    Protein protein((Molecule*)molecule);

    // 4-turn helixes
    for (int i = 0; i < protein.num4turnHelixes(); ++i) {
      QList<unsigned long int> helix = protein.helix4BackboneAtoms(i);

      Eigen::Vector3d p1 = Eigen::Vector3d::Zero();
      for (int i = 0; i < 16; ++i)
        p1 += *(molecule->atomById(helix.at(i))->pos());
      p1 /= 16.0;

      Eigen::Vector3d p2 = Eigen::Vector3d::Zero();
      for (int i = helix.size() - 16; i < helix.size(); ++i)
        p2 += *(molecule->atomById(helix.at(i))->pos());
      p2 /= 16.0;

      Eigen::Vector3d ab = p1 - p2;
      ab.normalize();
      ab *= 3.5;

      p1 += ab; 
      p2 -= ab; 
    
      QVector<Vector3d> helixPoints;
      helixPoints.append(p1);
      helixPoints.append(p2);
      m_helixes4.append(helixPoints);
    }
    // 3-turn helixes
    for (int i = 0; i < protein.num3turnHelixes(); ++i) {
      QList<unsigned long int> helix = protein.helix3BackboneAtoms(i);

      Eigen::Vector3d p1 = Eigen::Vector3d::Zero();
      for (int i = 0; i < 12; ++i)
        p1 += *(molecule->atomById(helix.at(i))->pos());
      p1 /= 12.0;

      Eigen::Vector3d p2 = Eigen::Vector3d::Zero();
      for (int i = helix.size() - 12; i < helix.size(); ++i)
        p2 += *(molecule->atomById(helix.at(i))->pos());
      p2 /= 12.0;

      Eigen::Vector3d ab = p1 - p2;
      ab.normalize();
      ab *= 3.0;

      p1 += ab; 
      p2 -= ab; 
    
      QVector<Vector3d> helixPoints;
      helixPoints.append(p1);
      helixPoints.append(p2);
      m_helixes3.append(helixPoints);
    }

 
    m_chains.clear();
    QList<Primitive *> list;
    list = primitives().subList(Primitive::ResidueType);
    unsigned int currentChain = 0;
    QVector<Vector3d> pts;

    foreach(Primitive *p, list) {
      Residue *r = static_cast<Residue *>(p);
      if(r->name() =="HOH") {
        continue;
      }

      if(r->chainNumber() != currentChain) {
        // this residue is on a new chain
        if(pts.size() > 0)
          m_chains.push_back(pts);
        currentChain = r->chainNumber();
        pts.clear();
      }

      QList<unsigned long int> atoms = r->atoms();
      foreach (unsigned long int atom, atoms) {
        // should be CA
        QString atomId = r->atomId(atom);
        atomId = atomId.trimmed();
        if (atomId == "CA") {
          pts.push_back(*molecule->atomById(atom)->pos());
        }
        else if (atomId == "N" && m_useNitrogens == 2) {
          pts.push_back(*molecule->atomById(atom)->pos());
        }
      } // end atoms in residue

    } // end primitive list (i.e., all residues)
    m_chains.push_back(pts); // Add the last chain (possibly the only chain)
    m_update = false;
  }

  Engine::PrimitiveTypes RibbonEngine::primitiveTypes() const
  {
    return Engine::Atoms;
  }

  Engine::ColorTypes RibbonEngine::colorTypes() const
  {
    return Engine::IndexedColors;
  }

  void RibbonEngine::setType(int value)
  {
    m_type = value;
    emit changed();
  }

  void RibbonEngine::setRadius(int value)
  {
    m_radius = 0.1 * value;
    emit changed();
  }

  void RibbonEngine::setUseNitrogens(int setting)
  {
    m_useNitrogens = setting;
    m_update = true;
    emit changed();
  }

  QWidget* RibbonEngine::settingsWidget()
  {
    if(!m_settingsWidget)
    {
      m_settingsWidget = new RibbonSettingsWidget();
      connect(m_settingsWidget->renderType, SIGNAL(activated(int)),
              this, SLOT(setType(int)));
      connect(m_settingsWidget->radiusSlider, SIGNAL(valueChanged(int)),
              this, SLOT(setRadius(int)));
      connect(m_settingsWidget->useNitrogens, SIGNAL(stateChanged(int)),
              this, SLOT(setUseNitrogens(int)));
      connect(m_settingsWidget, SIGNAL(destroyed()),
              this, SLOT(settingsWidgetDestroyed()));
      m_settingsWidget->renderType->setCurrentIndex(m_type);
      m_settingsWidget->radiusSlider->setValue(int(10 * m_radius));
      m_settingsWidget->useNitrogens->setCheckState((Qt::CheckState)m_useNitrogens);
    }
    return m_settingsWidget;
  }

  void RibbonEngine::settingsWidgetDestroyed()
  {
    m_settingsWidget = 0;
  }
  void RibbonEngine::writeSettings(QSettings &settings) const
  {
    Engine::writeSettings(settings);
    settings.setValue("radius", 10*m_radius);
    settings.setValue("type", m_type);
    settings.setValue("useNitrogens", m_useNitrogens);
  }

  void RibbonEngine::readSettings(QSettings &settings)
  {
    Engine::readSettings(settings);
    setType(settings.value("type", 0).toInt());
    setRadius(settings.value("radius", 10).toInt());
    setUseNitrogens(settings.value("useNitrogens", 2).toInt());
    if (m_settingsWidget) {
      m_settingsWidget->renderType->setCurrentIndex(m_type);
      m_settingsWidget->radiusSlider->setValue(int(10 * m_radius));
      m_settingsWidget->useNitrogens->setCheckState((Qt::CheckState)m_useNitrogens);
    }
  }

}

#include "ribbonengine.moc"

Q_EXPORT_PLUGIN2(ribbonengine, Avogadro::RibbonEngineFactory)

