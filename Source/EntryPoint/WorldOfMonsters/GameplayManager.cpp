/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/GameplayManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Of Monsters : Global gameplay interface
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
#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GameplayManager implementation
GameplayManager::GameplayManager():
    Manager<GameplayManager>(),
    m_mapSkills(), m_mapMonsters()
{
    // nothing to do
}
GameplayManager::~GameplayManager()
{
    // nothing to do
}

Void GameplayManager::Initialize()
{
    RandFn->XorShift128Seed( (ULong)(SystemFn->GetUTCTime(TIMEUNIT_SECONDS) & 0x00000000ffffffffui64), 1000 );

    m_mapSkills.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapSkills.SetComparator( _Compare_SkillIDs );
    m_mapSkills.Create();

    m_mapMonsters.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapMonsters.SetComparator( _Compare_MonsterIDs );
    m_mapMonsters.Create();

    _LoadGameDatabase();
}
Void GameplayManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );

    for( MonsterMap::Iterator it = m_mapMonsters.Begin(); !(it.IsNull()); ++it )
        Delete( it.GetItem() );
    m_mapMonsters.Destroy();

    for( SkillMap::Iterator it = m_mapSkills.Begin(); !(it.IsNull()); ++it )
        Delete( it.GetItem() );
    m_mapSkills.Destroy();

    UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Void GameplayManager::_LoadGameParameters()
{
    SelectMemory( TEXT("Scratch") );

    // Get parameter file
    XMLDocument * pParameterFile = XMLDocument::CreateDocument( GAMEPLAY_PARAMETERS_FILE );
    Assert( StringFn->Cmp(pParameterFile->GetTagName(), TEXT("GameplayParameters")) == 0 );
    Assert( pParameterFile != NULL );

    // Extract global parameters
    XMLNode * pGlobalParamsNode = pParameterFile->GetChildByTag( TEXT("Global"), 0 );
    Assert( pGlobalParamsNode != NULL );

    

    UnSelectMemory();
}
Void GameplayManager::_LoadGameDatabase()
{
    SelectMemory( TEXT("Scratch") );

    // List all monster files
    Bool bIsDirectory = false;
    GChar strFileName[FILENAME_LENGTH + 1];

    Bool bContinue = SystemFn->ListDirectoryFirst( MONSTER_XML_PATH, &bIsDirectory, strFileName, FILENAME_LENGTH );
    Assert( !bIsDirectory );
    while( bContinue ) {
        // Build path for that file
        GChar strFullPath[PATHNAME_LENGTH + 1];
        StringFn->Format( strFullPath, TEXT("%s/%s"), MONSTER_XML_PATH, strFileName );

        // Load XML file
        XMLDocument * pMonsterFile = XMLDocument::CreateDocument( strFullPath );
        Assert( pMonsterFile != NULL );
        Assert( StringFn->Cmp(pMonsterFile->GetTagName(), TEXT("MonsterFile")) == 0 );

        // Build skill models
        XMLNode * pSkillSetNode = pMonsterFile->GetChildByTag( TEXT("SkillSet"), 0 );
        Assert( pSkillSetNode != NULL );

        UInt iCount = pSkillSetNode->GetChildCount();
        for ( UInt i = 0; i < iCount; ++i ) {
            XMLNode * pSkillNode = pSkillSetNode->GetChildByTag( TEXT("Skill"), i );
            Assert( pSkillNode != NULL );

            Skill * pSkill = New Skill( pSkillNode );
            Bool bInserted = m_mapSkills.Insert( pSkill->GetID(), pSkill );
            Assert( bInserted );
        }

        // Build monster models
        iCount = pMonsterFile->GetChildCount() - 1;
        for( UInt i = 0; i < iCount;  ++i ) {
            XMLNode * pMonsterNode = pMonsterFile->GetChildByTag( TEXT("Monster"), i );
            Assert( pMonsterNode != NULL );

            Monster * pMonster = New Monster( pMonsterNode );
            Bool bInserted = m_mapMonsters.Insert( pMonster->GetID(), pMonster );
            Assert( bInserted );
        }

        // Done with this one
        XMLDocument::DestroyDocument( pMonsterFile );
        pMonsterFile = NULL;

        // Next file
        bContinue = SystemFn->ListDirectoryNext( &bIsDirectory, strFileName, FILENAME_LENGTH );
        Assert( !bIsDirectory );
    }

    UnSelectMemory();
}

