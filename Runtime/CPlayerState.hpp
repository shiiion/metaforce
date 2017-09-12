#ifndef __URDE_CPLAYERSTATE_HPP__
#define __URDE_CPLAYERSTATE_HPP__

#include "RetroTypes.hpp"
#include "CBasics.hpp"
#include "CStaticInterference.hpp"
#include "IOStreams.hpp"
#include "rstl.hpp"
#include "World/CHealthInfo.hpp"

namespace urde
{

class CPlayerState
{
    friend class CWorldTransManager;
public:
    enum class EItemType : s32
    {
        Invalid = -1,
        PowerBeam = 0,
        IceBeam = 1,
        WaveBeam = 2,
        PlasmaBeam = 3,
        Missiles = 4,
        ScanVisor = 5,
        MorphBallBombs = 6,
        PowerBombs = 7,
        Flamethrower = 8,
        ThermalVisor = 9,
        ChargeBeam = 10,
        SuperMissile = 11,
        GrappleBeam = 12,
        XRayVisor = 13,
        IceSpreader = 14,
        SpaceJumpBoots = 15,
        MorphBall = 16,
        CombatVisor = 17,
        BoostBall = 18,
        SpiderBall = 19,
        PowerSuit = 20,
        GravitySuit = 21,
        VariaSuit = 22,
        PhazonSuit = 23,
        EnergyTanks = 24,
        UnknownItem1 = 25,
        HealthRefill = 26,
        UnknownItem2 = 27,
        Wavebuster = 28,
        Truth = 29,
        Strength = 30,
        Elder = 31,
        Wild = 32,
        Lifegiver = 33,
        Warrior = 34,
        Chozo = 35,
        Nature = 36,
        Sun = 37,
        World = 38,
        Spirit = 39,
        Newborn = 40,

        /* This must remain at the end of the list */
        Max
    };

    enum class EPlayerVisor : u32
    {
        Combat,
        XRay,
        Scan,
        Thermal,

        /* This must remain at the end of the list */
        Max
    };

    enum class EPlayerSuit : s32
    {
        Invalid = -1,
        Power,
        Gravity,
        Varia,
        Phazon,
        FusionPower,
        FusionGravity,
        FusionVaria,
        FusionPhazon
    };

    enum class EBeamId : s32
    {
        Invalid = -1,
        Power,
        Ice,
        Wave,
        Plasma,
        Phazon,
        Phazon2 = 27
    };

private:

    static const u32 PowerUpMaxValues[41];
    struct CPowerUp
    {
        int x0_amount = 0;
        int x4_capacity = 0;
        CPowerUp() {}
        CPowerUp(int amount, int capacity) : x0_amount(amount), x4_capacity(capacity) {}
    };
    union
    {
        struct { bool x0_24_alive : 1; bool x0_25_firingComboBeam : 1; bool x0_26_fusion : 1; };
        u32 dummy = 0;
    };

    u32 x4_enabledItems = 0;
    EBeamId x8_currentBeam = EBeamId::Power;
    CHealthInfo xc_health = {99.f, 50.f};
    EPlayerVisor x14_currentVisor = EPlayerVisor::Combat;
    EPlayerVisor x18_transitioningVisor = x14_currentVisor;
    float x1c_visorTransitionFactor = 0.2f;
    EPlayerSuit x20_currentSuit = EPlayerSuit::Power;
    rstl::prereserved_vector<CPowerUp, 41> x24_powerups;
    rstl::reserved_vector<std::pair<CAssetId, float>, 846> x170_scanTimes;
    std::pair<u32, u32> x180_scanCompletionRate = {};
    CStaticInterference x188_staticIntf;
public:

    u32 GetMissileCostForAltAttack() const;
    float GetComboFireAmmoPeriod() const;
    static constexpr float GetMissileComboChargeFactor() { return 1.8f; }
    u32 CalculateItemCollectionRate() const;

    CHealthInfo& HealthInfo();
    const CHealthInfo &GetHealthInfo() const;
    u32 GetPickupTotal() { return 99; }
    void SetIsFusionEnabled(bool val) { x0_26_fusion = val; }
    bool IsFusionEnabled() const { return x0_26_fusion; }
    EPlayerSuit GetCurrentSuit() const;
    EPlayerSuit GetCurrentSuitRaw() const { return x20_currentSuit; }
    EBeamId GetCurrentBeam() const { return x8_currentBeam; }
    void SetCurrentBeam(EBeamId beam) { x8_currentBeam = beam; }
    bool CanVisorSeeFog(const CStateManager& stateMgr) const;
    EPlayerVisor GetCurrentVisor() const { return x14_currentVisor; }
    EPlayerVisor GetTransitioningVisor() const { return x18_transitioningVisor; }
    EPlayerVisor GetActiveVisor(const CStateManager& stateMgr) const;
    void UpdateStaticInterference(CStateManager& stateMgr, float dt);
    void IncreaseScanTime(u32 time, float val);
    void SetScanTime(CAssetId res, float time);
    float GetScanTime(CAssetId time) const;
    bool GetIsVisorTransitioning() const;
    float GetVisorTransitionFactor() const;
    void UpdateVisorTransition(float dt);
    bool StartVisorTransition(EPlayerVisor visor);
    void ResetVisor();
    bool ItemEnabled(EItemType type) const;
    void DisableItem(EItemType type);
    void EnableItem(EItemType type);
    bool HasPowerUp(EItemType type) const;
    u32 GetItemCapacity(EItemType type) const;
    u32 GetItemAmount(EItemType type) const;
    void DecrPickup(EItemType type, s32 amount);
    void IncrPickup(EItemType type, s32 amount);
    void ResetAndIncrPickUp(EItemType type, s32 amount);
    static float GetEnergyTankCapacity() { return 100.f; }
    static float GetBaseHealthCapacity() { return 99.f; }
    float CalculateHealth(u32 health);
    void ReInitalizePowerUp(EItemType type, u32 capacity);
    void InitializePowerUp(EItemType type, u32 capacity);
    u32 GetLogScans() const { return x180_scanCompletionRate.first; }
    u32 GetTotalLogScans() const { return x180_scanCompletionRate.second; }
    void SetScanCompletionRate(const std::pair<u32, u32>& p) { x180_scanCompletionRate = p; }
    bool IsPlayerAlive() const { return x0_24_alive; }
    void SetPlayerAlive(bool alive) { x0_24_alive = alive; }
    bool IsFiringComboBeam() const { return x0_25_firingComboBeam; }
    void SetFiringComboBeam(bool f) { x0_25_firingComboBeam = f; }
    void InitializeScanTimes();
    CStaticInterference& GetStaticInterference() { return x188_staticIntf; }
    const rstl::reserved_vector<std::pair<CAssetId, float>, 846>& GetScanTimes() const { return x170_scanTimes; }
    CPlayerState();
    CPlayerState(CBitStreamReader& stream);
    void PutTo(CBitStreamWriter& stream);

};
}

#endif // __URDE_CPLAYERSTATE_HPP__
