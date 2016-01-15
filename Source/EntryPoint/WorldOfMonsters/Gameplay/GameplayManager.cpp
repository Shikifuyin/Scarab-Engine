/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/Gameplay/GameplayManager.cpp
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
    // Random number generator
    RandFn->XorShift128Seed( (ULong)(SystemFn->GetUTCTime(TIMEUNIT_SECONDS) & 0x00000000ffffffffui64), 1000 );

    // Game parameters
    XMLDocument * pParameterFile = XMLDocument::CreateDocument( GAMEPLAY_PARAMETERS_FILE );

    Assert( pParameterFile != NULL );
    Assert( StringFn->Cmp(pParameterFile->GetTagName(), TEXT("GameParameters")) == 0 );

    m_hGameParameters.Load( pParameterFile );

    XMLDocument::DestroyDocument( pParameterFile );
    pParameterFile = NULL;

    // Game database
    m_mapSkills.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapSkills.SetComparator( _Compare_SkillIDs );
    m_mapSkills.Create();

    m_mapMonsters.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapMonsters.SetComparator( _Compare_MonsterIDs );
    m_mapMonsters.Create();

    _LoadGameDatabase( GAMEPLAY_MONSTERFILES_PATH );
}
Void GameplayManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );

    // Game database
    m_mapMonsters.Destroy();

    for( SkillMap::Iterator it = m_mapSkills.Begin(); !(it.IsNull()); ++it )
        Delete( it.GetItem() );
    m_mapSkills.Destroy();

    UnSelectMemory();
}

PlayerTown * GameplayManager::LoadPlayerTown( const GChar * strTownName ) const
{
    // Build file name
    GChar strFileName[PATHNAME_LENGTH + 1];
    StringFn->Format( strFileName, TEXT("%s/%s"), GAMEPLAY_PLAYERTOWNFILES_PATH, strTownName );

    // Parse file to XML
    XMLDocument * pPlayerTownFile = XMLDocument::CreateDocument( strFileName );
    Assert( pPlayerTownFile != NULL );
    Assert( StringFn->Cmp(pPlayerTownFile->GetTagName(), TEXT("PlayerTown")) == 0 );

    // Create resource
    SelectMemory( TEXT("Scratch") );
    PlayerTown * pPlayerTown = New PlayerTown( strTownName );
    UnSelectMemory();

    // Load from XML
    pPlayerTown->Load( pPlayerTownFile );

    // Release XML document
    XMLDocument::DestroyDocument( pPlayerTownFile );
    pPlayerTownFile = NULL;

    // Done
    return pPlayerTown;
}
PlayerTown * GameplayManager::CreatePlayerTown( const GChar * strTownName ) const
{
    // Build file name
    GChar strFileName[PATHNAME_LENGTH + 1];
    GChar strFullPath[PATHNAME_LENGTH + 1];
    StringFn->Format( strFileName, TEXT("%s.xml"), strTownName );
    StringFn->Format( strFullPath, TEXT("%s/%s"), GAMEPLAY_PLAYERTOWNFILES_PATH, strFileName );

    // Check file existence
    GChar strTempPath[PATHNAME_LENGTH + 1];
    Bool bFileExists = SystemFn->SearchFile( strTempPath, PATHNAME_LENGTH, GAMEPLAY_PLAYERTOWNFILES_PATH, strFileName );
    if ( bFileExists )
        return NULL; // name already taken !

    // Create resource
    SelectMemory( TEXT("Scratch") );
    PlayerTown * pPlayerTown = New PlayerTown( strTownName );
    UnSelectMemory();

    // Save to XML
    XMLDocument * pPlayerTownFile = XMLDocument::CreateDocument( TEXT("PlayerTown"), XML_VERSION );
    pPlayerTown->Save( pPlayerTownFile );

    // Render XML to file
    Bool bRendered = pPlayerTownFile->Render( strFullPath );
    Assert( bRendered );

    // Release XML document
    XMLDocument::DestroyDocument( pPlayerTownFile );
    pPlayerTownFile = NULL;

    // Done
    return pPlayerTown;
}
Void GameplayManager::SavePlayerTown( const PlayerTown * pPlayerTown ) const
{
    // Build file name
    GChar strFileName[PATHNAME_LENGTH + 1];
    GChar strFullPath[PATHNAME_LENGTH + 1];
    StringFn->Format( strFileName, TEXT("%s.xml"), pPlayerTown->GetName() );
    StringFn->Format( strFullPath, TEXT("%s/%s"), GAMEPLAY_PLAYERTOWNFILES_PATH, strFileName );

    // Check file existence
    GChar strTempPath[PATHNAME_LENGTH + 1];
    Bool bFileExists = SystemFn->SearchFile( strTempPath, PATHNAME_LENGTH, GAMEPLAY_PLAYERTOWNFILES_PATH, strFileName );
    if ( bFileExists ) {
        // Move to a temp file
        StringFn->Format( strTempPath, TEXT("%s.tmp"), strFullPath );
        SystemFn->MoveFile( strTempPath, strFullPath, true );
    }

    // Save to XML
    XMLDocument * pPlayerTownFile = XMLDocument::CreateDocument( TEXT("PlayerTown"), XML_VERSION );
    pPlayerTown->Save( pPlayerTownFile );

    // Render XML to file
    Bool bRendered = pPlayerTownFile->Render( strFullPath );
    Assert( bRendered );

    // Destroy temp file (old version)
    if ( bFileExists ) {
        Bool bDestroyed = SystemFn->DestroyFile( strTempPath );
        Assert( bDestroyed );
    }

    // Release XML document
    XMLDocument::DestroyDocument( pPlayerTownFile );
    pPlayerTownFile = NULL;
}
Void GameplayManager::ReleasePlayerTown( PlayerTown * pPlayerTown, Bool bSave ) const
{
    // Save if needed
    if ( bSave )
        SavePlayerTown( pPlayerTown );

    // Release resource
    SelectMemory( TEXT("Scratch") );
    Delete( pPlayerTown );
    UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Void GameplayManager::_LoadGameDatabase( const GChar * strPath )
{
    // List everything in current path
    Bool bIsDirectory = false;
    GChar strFileName[FILENAME_LENGTH + 1];

    Bool bContinue = SystemFn->ListDirectoryFirst( strPath, &bIsDirectory, strFileName, FILENAME_LENGTH );

    while( bContinue ) {
        // Build sub path
        GChar strSubPath[PATHNAME_LENGTH + 1];
        StringFn->Format( strSubPath, TEXT("%s/%s"), strPath, strFileName );

        // Sub-directory case
        if ( bIsDirectory ) {
            // Recurse
            _LoadGameDatabase( strSubPath );

            // Next file
            bContinue = SystemFn->ListDirectoryNext( &bIsDirectory, strFileName, FILENAME_LENGTH );
            continue;
        }

        // File case, load XML
        XMLDocument * pMonsterFile = XMLDocument::CreateDocument( strSubPath );

        Assert( pMonsterFile != NULL );
        Assert( StringFn->Cmp(pMonsterFile->GetTagName(), TEXT("MonsterFile")) == 0 );

            // Load skills
        const XMLNode * pSkillListNode = pMonsterFile->GetChildByTag( TEXT("SkillList"), 0 );
        Assert( pSkillListNode != NULL );

        UInt iCount = pSkillListNode->GetChildCount();
        for ( UInt i = 0; i < iCount; ++i ) {
            const XMLNode * pSkillNode = pSkillListNode->GetChildByTag( TEXT("Skill"), i );
            Assert( pSkillNode != NULL );

            Skill * pSkill = Skill::StaticLoad( pSkillNode );

            Bool bInserted = m_mapSkills.Insert( pSkill->GetID(), pSkill );
            Assert( bInserted );
        }

            // Load monsters
        const XMLNode * pMonsterListNode = pMonsterFile->GetChildByTag( TEXT("MonsterList"), 0 );
        Assert( pMonsterListNode != NULL );

        iCount = pMonsterListNode->GetChildCount();
        for( UInt i = 0; i < iCount;  ++i ) {
            XMLNode * pMonsterNode = pMonsterListNode->GetChildByTag( TEXT("Monster"), i );
            Assert( pMonsterNode != NULL );

            Monster hMonster;
            hMonster.Load( pMonsterNode );

            Bool bInserted = m_mapMonsters.Insert( hMonster.GetID(), hMonster );
            Assert( bInserted );
        }

        // Done with this one
        XMLDocument::DestroyDocument( pMonsterFile );
        pMonsterFile = NULL;

        // Next file
        bContinue = SystemFn->ListDirectoryNext( &bIsDirectory, strFileName, FILENAME_LENGTH );
    }
}

