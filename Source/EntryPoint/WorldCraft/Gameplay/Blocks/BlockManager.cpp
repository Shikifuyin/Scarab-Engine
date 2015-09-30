/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Blocks/BlockManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Blocks : BlockManager
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BlockManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BlockManager implementation
BlockManager::BlockManager():
    Manager<BlockManager>(), m_mapBlockDB()
{
    // nothing to do
}
BlockManager::~BlockManager()
{
    // nothing to do
}

Void BlockManager::Initialize()
{
    m_mapBlockDB.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapBlockDB.SetComparator( _Compare_BlockID );
    m_mapBlockDB.Create();
}
Void BlockManager::Cleanup()
{
    SelectMemory( TEXT("Blocks") );

    BlockMap::Iterator itBlock = m_mapBlockDB.Begin();
    while( !(itBlock.IsNull()) ) {
        Delete( itBlock.GetItem() );
        ++itBlock;
    }
    m_mapBlockDB.Destroy();

    UnSelectMemory();
}

