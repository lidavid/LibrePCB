/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
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
#include "cmdschematicnetpointadd.h"
#include "../schematic.h"
#include "../schematicnetpoint.h"

namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdSchematicNetPointAdd::CmdSchematicNetPointAdd(Schematic& schematic, const QUuid& netsignal,
                                                 const Point& position, UndoCommand* parent) throw (Exception) :
    UndoCommand(QCoreApplication::translate("CmdSchematicNetPointAdd", "Add netpoint"), parent),
    mSchematic(schematic), mNetSignal(netsignal), mPosition(position), mNetPoint(0)
{
}

CmdSchematicNetPointAdd::~CmdSchematicNetPointAdd() noexcept
{
    if ((mNetPoint) && (!mIsExecuted))
        delete mNetPoint;
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

void CmdSchematicNetPointAdd::redo() throw (Exception)
{
    if (!mNetPoint) // only the first time
        mNetPoint = mSchematic.createNetPoint(mNetSignal, mPosition); // throws an exception on error

    mSchematic.addNetPoint(mNetPoint); // throws an exception on error

    try
    {
        UndoCommand::redo(); // throws an exception on error
    }
    catch (Exception &e)
    {
        mSchematic.removeNetPoint(mNetPoint);
        throw;
    }
}

void CmdSchematicNetPointAdd::undo() throw (Exception)
{
    mSchematic.removeNetPoint(mNetPoint); // throws an exception on error

    try
    {
        UndoCommand::undo();
    }
    catch (Exception& e)
    {
        mSchematic.addNetPoint(mNetPoint);
        throw;
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project