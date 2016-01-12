


/////////////////////////////////////////////////////////////////////////////////
// The BuildingRuneEvolution class
class BuildingRuneEvolution : public Building
{
public:
    BuildingRuneEvolution();
    virtual ~BuildingRuneEvolution();

    // Rune fusion
    inline Bool EvolutionUnlocked() const;

    Bool CanPowerUp( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const;
    Void PowerUp( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount );

    Bool CanEvolve( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount ) const;
    Void Evolve( UInt iTargetRune, UInt arrFoodRunes[5], UInt iFoodCount );

private:
    // Dungeon access
    BuildingDungeon * m_pDungeon;

    // Rune fusion
    Bool m_bEvolutionUnlocked;
};


