/**********************************************************************
  ElementTranslator - Class to handle translation/i18n of element names

  Copyright (C) 2007 Geoffrey R. Hutchison

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

#include "elementtranslate.h"

namespace Avogadro {

  ElementTranslator::ElementTranslator() : QObject()
  {
  }

  ElementTranslator::~ElementTranslator()
  {
  }

  QString ElementTranslator::name(int element)
  {
    QString result;
    switch(element) {
    case 1:
      result = tr("Hydrogen");
      break;
    case 2:
      result = tr("Helium");
      break;
    case 3:
      result = tr("Lithium");
      break;
    case 4:
      result = tr("Beryllium");
      break;
    case 5:
      result = tr("Boron");
      break;
    case 6:
      result = tr("Carbon");
      break;
    case 7:
      result = tr("Nitrogen");
      break;
    case 8:
      result = tr("Oxygen");
      break;
    case 9:
      result = tr("Fluorine");
      break;
    case 10:
      result = tr("Neon");
      break;
    case 11:
      result = tr("Sodium");
      break;
    case 12:
      result = tr("Magnesium");
      break;
    case 13:
      result = tr("Aluminum");
      break;
    case 14:
      result = tr("Silicon");
      break;
    case 15:
      result = tr("Phosphorus");
      break;
    case 16:
      result = tr("Sulfur");
      break;
    case 17:
      result = tr("Chlorine");
      break;
    case 18:
      result = tr("Argon");
      break;
    case 19:
      result = tr("Potassium");
      break;
    case 20:
      result = tr("Calcium");
      break;
    case 21:
      result = tr("Scandium");
      break;
    case 22:
      result = tr("Titanium");
      break;
    case 23:
      result = tr("Vanadium");
      break;
    case 24:
      result = tr("Chromium");
      break;
    case 25:
      result = tr("Manganese");
      break;
    case 26:
      result = tr("Iron");
      break;
    case 27:
      result = tr("Cobalt");
      break;
    case 28:
      result = tr("Nickel");
      break;
    case 29:
      result = tr("Copper");
      break;
    case 30:
      result = tr("Zinc");
      break;
    case 31:
      result = tr("Gallium");
      break;
    case 32:
      result = tr("Germanium");
      break;
    case 33:
      result = tr("Arsenic");
      break;
    case 34:
      result = tr("Selenium");
      break;
    case 35:
      result = tr("Bromine");
      break;
    case 36:
      result = tr("Krypton");
      break;
    case 37:
      result = tr("Rubidium");
      break;
    case 38:
      result = tr("Strontium");
      break;
    case 39:
      result = tr("Yttrium");
      break;
    case 40:
      result = tr("Zirconium");
      break;
    case 41:
      result = tr("Niobium");
      break;
    case 42:
      result = tr("Molybdenum");
      break;
    case 43:
      result = tr("Technetium");
      break;
    case 44:
      result = tr("Ruthenium");
      break;
    case 45:
      result = tr("Rhodium");
      break;
    case 46:
      result = tr("Palladium");
      break;
    case 47:
      result = tr("Silver");
      break;
    case 48:
      result = tr("Cadmium");
      break;
    case 49:
      result = tr("Indium");
      break;
    case 50:
      result = tr("Tin");
      break;
    case 51:
      result = tr("Antimony");
      break;
    case 52:
      result = tr("Tellurium");
      break;
    case 53:
      result = tr("Iodine");
      break;
    case 54:
      result = tr("Xenon");
      break;
    case 55:
      result = tr("Caesium");
      break;
    case 56:
      result = tr("Barium");
      break;
    case 57:
      result = tr("Lanthanum");
      break;
    case 58:
      result = tr("Cerium");
      break;
    case 59:
      result = tr("Praseodymium");
      break;
    case 60:
      result = tr("Neodymium");
      break;
    case 61:
      result = tr("Promethium");
      break;
    case 62:
      result = tr("Samarium");
      break;
    case 63:
      result = tr("Europium");
      break;
    case 64:
      result = tr("Gadolinium");
      break;
    case 65:
      result = tr("Terbium");
      break;
    case 66:
      result = tr("Dysprosium");
      break;
    case 67:
      result = tr("Holmium");
      break;
    case 68:
      result = tr("Erbium");
      break;
    case 69:
      result = tr("Thulium");
      break;
    case 70:
      result = tr("Ytterbium");
      break;
    case 71:
      result = tr("Lutetium");
      break;
    case 72:
      result = tr("Hafnium");
      break;
    case 73:
      result = tr("Tantalum");
      break;
    case 74:
      result = tr("Tungsten");
      break;
    case 75:
      result = tr("Rhenium");
      break;
    case 76:
      result = tr("Osmium");
      break;
    case 77:
      result = tr("Iridium");
      break;
    case 78:
      result = tr("Platinum");
      break;
    case 79:
      result = tr("Gold");
      break;
    case 80:
      result = tr("Mercury");
      break;
    case 81:
      result = tr("Thallium");
      break;
    case 82:
      result = tr("Lead");
      break;
    case 83:
      result = tr("Bismuth");
      break;
    case 84:
      result = tr("Polonium");
      break;
    case 85:
      result = tr("Astatine");
      break;
    case 86:
      result = tr("Radon");
      break;
    case 87:
      result = tr("Francium");
      break;
    case 88:
      result = tr("Radium");
      break;
    case 89:
      result = tr("Actinium");
      break;
    case 90:
      result = tr("Thorium");
      break;
    case 91:
      result = tr("Protactinium");
      break;
    case 92:
      result = tr("Uranium");
      break;
    case 93:
      result = tr("Neptunium");
      break;
    case 94:
      result = tr("Plutonium");
      break;
    case 95:
      result = tr("Americium");
      break;
    case 96:
      result = tr("Curium");
      break;
    case 97:
      result = tr("Berkelium");
      break;
    case 98:
      result = tr("Californium");
      break;
    case 99:
      result = tr("Einsteinium");
      break;
    case 100:
      result = tr("Fermium");
      break;
    case 101:
      result = tr("Mendelevium");
      break;
    case 102:
      result = tr("Nobelium");
      break;
    case 103:
      result = tr("Lawrencium");
      break;
    case 104:
      result = tr("Rutherfordium");
      break;
    case 105:
      result = tr("Dubnium");
      break;
    case 106:
      result = tr("Seaborgium");
      break;
    case 107:
      result = tr("Bohrium");
      break;
    case 108:
      result = tr("Hassium");
      break;
    case 109:
      result = tr("Meitnerium");
      break;
    case 110:
      result = tr("Darmstadtium");
      break;
    case 111:
      result = tr("Roentgenium");
      break;
    case 112:
      result = tr("Ununbium");
      break;
    case 113:
      result = tr("Ununtrium");
      break;
    case 114:
      result = tr("Ununquadium");
      break;
    case 115:
      result = tr("Ununpentium");
      break;
    case 116:
      result = tr("Ununhexium");
      break;
    case 117:
      result = tr("Ununseptium");
      break;
    case 118:
      result = tr("Ununoctium");
      break;

    default:
      result = tr("Unknown");
    }

    return result;
  }

}

#include "elementtranslate.moc"

