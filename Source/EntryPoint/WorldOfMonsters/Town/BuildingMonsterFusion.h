



/////////////////////////////////////////////////////////////////////////////////
// The BuildingMonsterFusion class
class BuildingMonsterFusion : public Building
{
public:
    BuildingMonsterFusion();
    virtual ~BuildingMonsterFusion();

    // Monster fusion
    inline Bool FusionUnlocked() const;

    inline Void GetRecipe( MonsterID iMonsterID, MonsterID outFoodMonsters[4], UInt * outRequiredRank ) const;

    Bool CanFuse( MonsterID iMonsterID ) const;
    Void Fuse( MonsterID iMonsterID );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Monster fusion
    Bool m_bFusionUnlocked;
};



