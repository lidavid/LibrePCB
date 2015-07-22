/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/

#include <QtCore>
#include "cmdprojectlibraryaddelement.h"
#include "../projectlibrary.h"
#include <librepcblibrary/elements.h>

namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

template <typename ElementType>
CmdProjectLibraryAddElement<ElementType>::CmdProjectLibraryAddElement(
        ProjectLibrary& library, const FilePath& elementDirectory, UndoCommand* parent) throw (Exception) :
    UndoCommand(tr("Add element to library"), parent),
    mLibrary(library), mElemDirPath(elementDirectory), mElement(nullptr)
{
}

template <typename ElementType>
CmdProjectLibraryAddElement<ElementType>::~CmdProjectLibraryAddElement() noexcept
{
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

template <typename ElementType>
void CmdProjectLibraryAddElement<ElementType>::redo() throw (Exception)
{
    mElement = nullptr;
    mElement = addElement(); // throws an exception on error
    Q_ASSERT(mElement);

    try
    {
        UndoCommand::redo(); // throws an exception on error
    }
    catch (Exception &e)
    {
        removeElement();
        throw;
    }
}

template <typename ElementType>
void CmdProjectLibraryAddElement<ElementType>::undo() throw (Exception)
{
    throw RuntimeError(__FILE__, __LINE__, QString(), tr("not supported yet"));

    Q_ASSERT(mElement);
    removeElement(); // throws an exception on error
    mElement = nullptr;

    try
    {
        UndoCommand::undo();
    }
    catch (Exception& e)
    {
        mElement = addElement(); // throws an exception on error
        Q_ASSERT(mElement);
        throw;
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

template <>
const library::Symbol* CmdProjectLibraryAddElement<library::Symbol>::addElement()
{
    return mLibrary.addSymbol(mElemDirPath);
}

template <>
const library::Footprint* CmdProjectLibraryAddElement<library::Footprint>::addElement()
{
    return mLibrary.addFootprint(mElemDirPath);
}

template <>
const library::Model3D* CmdProjectLibraryAddElement<library::Model3D>::addElement()
{
    return mLibrary.add3dModel(mElemDirPath);
}

template <>
const library::SpiceModel* CmdProjectLibraryAddElement<library::SpiceModel>::addElement()
{
    return mLibrary.addSpiceModel(mElemDirPath);
}

template <>
const library::Package* CmdProjectLibraryAddElement<library::Package>::addElement()
{
    return mLibrary.addPackage(mElemDirPath);
}

template <>
const library::GenericComponent* CmdProjectLibraryAddElement<library::GenericComponent>::addElement()
{
    return mLibrary.addGenComp(mElemDirPath);
}

template <>
const library::Component* CmdProjectLibraryAddElement<library::Component>::addElement()
{
    return mLibrary.addComp(mElemDirPath);
}

template <>
void CmdProjectLibraryAddElement<library::Symbol>::removeElement()
{
    mLibrary.removeSymbol(mElement->getUuid());
}

template <>
void CmdProjectLibraryAddElement<library::Footprint>::removeElement()
{
    mLibrary.removeFootprint(mElement->getUuid());
}

template <>
void CmdProjectLibraryAddElement<library::Model3D>::removeElement()
{
    mLibrary.remove3dModel(mElement->getUuid());
}

template <>
void CmdProjectLibraryAddElement<library::SpiceModel>::removeElement()
{
    mLibrary.removeSpiceModel(mElement->getUuid());
}

template <>
void CmdProjectLibraryAddElement<library::Package>::removeElement()
{
    mLibrary.removePackage(mElement->getUuid());
}

template <>
void CmdProjectLibraryAddElement<library::GenericComponent>::removeElement()
{
    mLibrary.removeGenComp(mElement->getUuid());
}

template <>
void CmdProjectLibraryAddElement<library::Component>::removeElement()
{
    mLibrary.removeComp(mElement->getUuid());
}

/*****************************************************************************************
 *  Explicit Template Instantiation
 ****************************************************************************************/

template class CmdProjectLibraryAddElement<library::Symbol>;
template class CmdProjectLibraryAddElement<library::Footprint>;
template class CmdProjectLibraryAddElement<library::Model3D>;
template class CmdProjectLibraryAddElement<library::SpiceModel>;
template class CmdProjectLibraryAddElement<library::Package>;
template class CmdProjectLibraryAddElement<library::GenericComponent>;
template class CmdProjectLibraryAddElement<library::Component>;

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project