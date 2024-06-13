#pragma once

UENUM(BlueprintType)
enum class EObtainingMethod
{
	None,
	Pickable,
	Purchasable
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_Fists        UMETA(DisplayName = "Fists"),
    EWT_Melee		 UMETA(DisplayName = "MeleeWeapon"),
    EWT_Ranged		 UMETA(DisplayName = "RangedWeapon")
};

UENUM(BlueprintType)
enum class ERarityCategory : uint8
{
    Common,
    Rare,
    Epic,
    Unique
};


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    EWS_Initial             UMETA(DisplayName = "Initial"),
    EWS_Equipped			UMETA(DisplayName = "Equipped"),
    EWS_EquippedSecondary	UMETA(DisplayName = "Equipped Secondary"),
    EWS_Dropped				UMETA(DisplayName = "Dropped"),

	EWS_Tradeable			UMETA(DisplayName = "TradeableState"),

    EWS_MAX					UMETA(DisplayName = "DefaultMAX")
};


#pragma region Range Weapon
UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_Default			UMETA(DisplayName = "Default"),
	EFT_Projectile		UMETA(DisplayName = "ProjectileType"),
	EFT_HitScan			UMETA(DisplayName = "HitScanType")
};

UENUM(BlueprintType)
enum class ERangedWeaponType : uint8
{
	Default				UMETA(DisplayName = "Default"),
	ERT_Pistol			UMETA(DisplayName = "Pistol"),
	ERT_Rifle			UMETA(DisplayName = "Rifle"),
	ERT_Submachine		UMETA(DisplayName = "Sub-machine Gun"),
	ERT_Heavymachine	UMETA(DisplayName = "Heavy-machine Gun"),
	ERT_Shotgun			UMETA(DisplayName = "Shotgun"),
	ERT_Sniper			UMETA(DisplayName = "Sniper Rifle"),
	ERT_Bow				UMETA(DisplayName = "Bow"),
	ERT_Crossbow		UMETA(DisplayName = "Crossbow"),
	ERT_Throwing		UMETA(DisplayName = "Throwing Weapon"),
	ERT_Launcher		UMETA(DisplayName = "Launcher"),
	ERT_Cannon			UMETA(DisplayName = "Cannon"),
	ERT_MAX
};

UENUM(BlueprintType)
enum class EAmmunitionType : uint8
{
	Default				UMETA(DisplayName = "Default"),
	EAT_9mm             UMETA(DisplayName = "9mm"),
	EAT_45ACP           UMETA(DisplayName = ".45 ACP"),
	EAT_556mm           UMETA(DisplayName = "5.56mm"),
	EAT_762mm           UMETA(DisplayName = "7.62mm"),
	EAT_12Gauge         UMETA(DisplayName = "12 Gauge"),
	EAT_50Caliber       UMETA(DisplayName = ".50 Caliber"),
	EAT_40mm            UMETA(DisplayName = "40mm Grenade"),
	EAT_Arrow           UMETA(DisplayName = "Arrow"),
	EAT_Bolt            UMETA(DisplayName = "Bolt"),
	EAT_Rocket          UMETA(DisplayName = "Rocket"),
	EAT_Cartridge       UMETA(DisplayName = "Cartridge"),
	EAT_MAX
};

UENUM(BlueprintType, Meta = (DisplayName = "Weapon Firing Mode", ToolTip = "Specifies the firing mode of a weapon"))
enum class EWeaponFiringMode
{
	Default			UMETA(DisplayName = "Default"),
	SingleShot,      // Single fire mode
	SemiAutomatic,   // Semi-automatic fire mode
	Automatic        // Automatic fire mode
};



#pragma endregion

#pragma region MeleeWeapon
UENUM(BlueprintType)
enum class EMeleeWeaponType : uint8
{
	Default			   UMETA(DisplayName = "Default"),
	EMT_Sword          UMETA(DisplayName = "Sword"),
	EMT_Katana         UMETA(DisplayName = "Katana"),
	EMT_Axe            UMETA(DisplayName = "Axe"),
	EMT_Mace           UMETA(DisplayName = "Mace"),
	EMT_Dagger         UMETA(DisplayName = "Dagger"),
	EMT_Club           UMETA(DisplayName = "Club"),
	EMT_Spear          UMETA(DisplayName = "Spear"),
	EMT_Whip           UMETA(DisplayName = "Whip"),
	EMT_Shield         UMETA(DisplayName = "Shield"),
	EMT_HiddenBlade    UMETA(DisplayName = "HiddenBlade")
};
#pragma endregion
