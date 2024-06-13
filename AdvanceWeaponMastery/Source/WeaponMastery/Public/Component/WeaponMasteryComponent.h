/*
* Copyright (c) Reverse-A @2024
*/
#pragma once
#pragma region Includes
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Utilities/DiagnosticSystem.h>
#include <Data/WeaponData.h>
#include "WeaponMasteryComponent.generated.h"
#pragma endregion

#pragma region Forward Declarations
class USoundCue;
class UWeaponData;
class AWeaponModule;
#pragma endregion
#pragma region Structures

USTRUCT(BlueprintType)
struct FWeaponMastery
{
	GENERATED_BODY()

public:
	// The type of attainment for the weapon (e.g., Purchasable).
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EAttainmentType WeaponAttainmentType;

	// The current state of the weapon (e.g., equipped, dropped).
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponStateType;

	// The type of weapon (e.g., ShortRange, LongRange).
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponType WeaponType;
	
	// The rarity effect process for the weapon.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties",
		meta =
		(
			EditCondition =
			"WeaponType == EWeaponType::ShortRange || WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::Heavy"
		)
	)
	ERarityEffectProcess RarityEffectProcess;

	// The rarity type of the weapon (e.g., common, rare).
	UPROPERTY
	(
		EditAnywhere,
		Category = "Weapon Properties",
		meta = (EditCondition ="RarityEffectProcess == ERarityEffectProcess::Apply")
	)
	ERarityType WeaponRarityType;

	// The type of damage inflicted by the weapon (e.g., single, random).
	UPROPERTY
	(
		EditAnywhere,
		Category = "Weapon Properties", 
		meta = (ShowOnlyInnerProperties),
		meta = 	
		(
			EditCondition ="WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::ShortRange || WeaponType == EWeaponType::Heavy"
		)
	)
	EWeaponDamageType WeaponDamageType = EWeaponDamageType::Select;

	// The amount of damage inflicted by the weapon.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties",
		meta = 
		(
			EditCondition = 
			"WeaponDamageType == EWeaponDamageType::SingleDamage && (WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::ShortRange)"
		)
	)
	int32 Damage;

	// The minimum random damage inflicted by the weapon.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties",
		meta =
		(
			EditCondition =
			"WeaponDamageType == EWeaponDamageType::RandomDamage && (WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::ShortRange)"
		)
	)	
	int32 RandomDamageMin = 0;

	// The maximum random damage inflicted by the weapon.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties",
		meta =
		(
			EditCondition =
			"WeaponDamageType == EWeaponDamageType::RandomDamage && (WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::ShortRange)"
		)
	)
	int32 RandomDamageMax = 0;

	// The penetration value of the weapon.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::ShortRange")
	)
	float Penetration;

	// The range value of the weapon.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::ShortRange" )
	)
	float Range;

	// The type of long-range weapon (e.g., assault rifle, sniper).
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties|Long Range Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	ELongRangeWeaponType LongRangeWeaponType;

	// The type of ammunition used by the weapon.
	UPROPERTY
	(
		VisibleAnywhere, Category = "Weapon Properties|Long Range Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	EAmmunitionType AmmunitionType;

	// The firing type of the weapon (e.g., projectile, hitscan).
	UPROPERTY
	(
		VisibleAnywhere, Category = "Weapon Properties|Long Range Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	EFireType FireType;

	// Indicates whether the weapon is fully automatic.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties|Long Range Properties",
		meta =
		(
			EditCondition =
			"LongRangeWeaponType == ELongRangeWeaponType::AssaultRifle || LongRangeWeaponType == ELongRangeWeaponType::Submachine || LongRangeWeaponType == ELongRangeWeaponType::PDW || LongRangeWeaponType == ELongRangeWeaponType::LightMachine"
		)
	)
	bool bFullAutomatic = false;

	// The firing mode of the weapon (e.g., single-shot, automatic).
	UPROPERTY
	(
		VisibleAnywhere, Category = "Weapon Properties|Long Range Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	EFireModeType FireModeType;

	// The accuracy value of the weapon.
	UPROPERTY
	(
		EditAnywhere, Category = "Weapon Properties|Long Range Properties", 
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	float Accuracy;

	// The magazine capacity of the weapon.
	UPROPERTY
	(
		EditAnywhere,
		Category = "Weapon Properties|Long Range Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	int32 MagazineCapacity;

	// The rate of fire of the weapon, measured in rounds per minute
	UPROPERTY
	(
		EditAnywhere, 
		Category = "Weapon Properties|Long Range Properties", 
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	float FireRate;

	// The time it takes to reload the weapon, measured in seconds
	UPROPERTY
	(
		EditAnywhere, 
		Category = "Weapon Properties|Long Range Properties", 
		meta = (EditCondition = "WeaponType == EWeaponType::LongRange")
	)
	float ReloadTime;

	// The type of short-range weapon.
	UPROPERTY
	(
		EditAnywhere,
		Category = "Weapon Properties|Short Range Properties",
		meta = (EditCondition =	"WeaponType == EWeaponType::ShortRange")
	)
	EShortRangeWeaponType ShortRangeWeaponType;

	// The attack speed of the short-range weapons, could be measured in attacks per second.
	UPROPERTY
	(
		EditAnywhere,
		Category = "Weapon Properties|Short Range Properties",
		meta = (EditCondition = "WeaponType == EWeaponType::ShortRange || WeaponType == EWeaponType::Heavy")
	)
	float AttackSpeed;

	// The weight of the weapon, affecting the player's movement speed and agility.
	UPROPERTY
	(
		EditAnywhere,
		Category = "Weapon Properties",
		meta = 
		(
			EditCondition = "WeaponType == EWeaponType::ShortRange || WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::Heavy"
		)
	)
	float Weight;

	// The durability of the weapon, indicating its resistance to wear and tear.
	UPROPERTY
	(
		EditAnywhere,
		Category = "Weapon Properties",
		meta = 		
		(
			EditCondition = "WeaponType == EWeaponType::ShortRange || WeaponType == EWeaponType::LongRange || WeaponType == EWeaponType::Heavy"
		)
	)	
	float Durability;

	// The price of the weapon in the game's currency.
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Price;

	FWeaponMastery()
	: WeaponAttainmentType	(EAttainmentType::Select)
	, WeaponStateType		(EWeaponState::Select)
	, WeaponType			(EWeaponType::Select)
	, RarityEffectProcess	(ERarityEffectProcess::Select)
	, WeaponRarityType		(ERarityType::Select)
	, WeaponDamageType		(EWeaponDamageType::Select)
	, Damage				(0)
	, RandomDamageMin		(0)
	, RandomDamageMax		(0)
	, Penetration			(0.0f)
	, Range					(0.0f)
	, LongRangeWeaponType	(ELongRangeWeaponType::Select)
	, AmmunitionType		(EAmmunitionType::Select)
	, FireType				(EFireType::Select)
	, bFullAutomatic		(false)
	, FireModeType			(EFireModeType::Select)
	, Accuracy				(0.0f)
	, MagazineCapacity		(0)
	, FireRate				(0.0f)
	, ReloadTime			(0.0f)
	, ShortRangeWeaponType	(EShortRangeWeaponType::Select)
	, AttackSpeed			(0.0f)
	, Weight				(0.0f)
	, Durability			(0.0f)
	, Price					(0.0f)
	{
		/* Constructor Body */
	}

};

// Structure representing the details of a weapon.
USTRUCT(BlueprintType)
struct FWeaponDetails
{
	GENERATED_BODY()

public:
	// The rarity of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Rarity;

	// The type of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString WeaponTypeName;

	// The name of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Name;

	// The type of ammunition used by the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString AmmunitionTypeName;

	// The damage inflicted by the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Damage;

	// The penetration capability of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Penetration;

	// The effective range of the weapon
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Range;

	// The accuracy of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Accuracy;

	// The firing type of the weapon (applicable for long-range weapons).
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Long Range", meta = (ShowOnlyInnerProperties))
	FString FireType;

	// The firing mode of the weapon (applicable for long-range weapons).
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Long Range", meta = (ShowOnlyInnerProperties))
	FString FireMode;

	// The magazine capacity of the weapon (applicable for long-range weapons).
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Long Range", meta = (ShowOnlyInnerProperties))
	FString MagazineCapacity;

	// The fire rate of the weapon (applicable for long-range weapons).
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Long Range", meta = (ShowOnlyInnerProperties))
	FString FireRate;

	// The reload time of the weapon (applicable for long-range weapons).
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Long Range", meta = (ShowOnlyInnerProperties))
	FString ReloadTime;

	// The attack speed of the weapon (applicable for short-range weapons).
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Short Range", meta = (ShowOnlyInnerProperties))
	FString AttackSpeed;

	// The weight of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Weight;

	// The durability of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Durability;

	// The price of the weapon.
	UPROPERTY(VisibleAnywhere, Category = "WeaponDetails|Common", meta = (ShowOnlyInnerProperties))
	FString Price;

	FWeaponDetails()
	: Rarity				(TEXT(""))
	, WeaponTypeName		(TEXT(""))
	, Name					(TEXT(""))
	, AmmunitionTypeName	(TEXT(""))
	, Damage				(TEXT(""))
	, Penetration			(TEXT(""))
	, Range					(TEXT(""))
	, Accuracy				(TEXT(""))
	, FireMode				(TEXT(""))
	, MagazineCapacity		(TEXT(""))
	, FireRate				(TEXT(""))
	, ReloadTime			(TEXT(""))
	, AttackSpeed			(TEXT(""))
	, Weight				(TEXT(""))
	, Durability			(TEXT(""))
	, Price					(TEXT(""))
	{		/*Constructor Body*/	 }
};

/**
 * Structure representing the visual and auditory effects associated with a weapon.
 */
USTRUCT()
struct FWeaponEffectAndProps
{
	GENERATED_BODY()

public:
	// Sound cue to play when the weapon is equipped.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Sound")
	USoundCue* EquipSoundCue;

	// Sound cue to play when the weapon is dropped.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Sound")
	USoundCue* DropSoundCue;

	// Sound cue to play when the weapon is thrown or dropped.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Sound")
	USoundCue* ThrowSoundCue;

	// Material representing common rarity.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Visual")
	TObjectPtr<UMaterial> CommonMaterial;

	// Material representing rare rarity.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Visual")
	TObjectPtr<UMaterial> RareMaterial;

	// Material representing epic rarity.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Visual")
	TObjectPtr<UMaterial> EpicMaterial;

	// Material representing legendary rarity.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Visual")
	TObjectPtr<UMaterial> LegendaryMaterial;

	// Material representing mystic rarity.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Visual")
	TObjectPtr<UMaterial> MysticMaterial;

	// Material representing unique rarity.
	UPROPERTY(EditAnywhere, Category = "WeaponEffectsAndProperties|Visual")
	TObjectPtr<UMaterial> UniqueMaterial;

	/**
	 * Default constructor to initialize member variables.
	 */
	FWeaponEffectAndProps()
	: EquipSoundCue			(nullptr)
	, DropSoundCue			(nullptr)
	, ThrowSoundCue			(nullptr)
	, CommonMaterial		(nullptr)
	, RareMaterial			(nullptr)
	, EpicMaterial			(nullptr)
	, LegendaryMaterial		(nullptr)
	, MysticMaterial		(nullptr)
	, UniqueMaterial		(nullptr)
	{  
		// For promotional and educational purposes 
		InitializeMaterialsviaFilePath();
	}

	/**
	 * Initializes materials by loading them from file paths.
	 */
	void InitializeMaterialsviaFilePath()
	{
		// Define file paths for each material.
		FString CommonMaterialPath		= TEXT("/WeaponMastery/Assets/Materials/M_Overlay_Common.M_Overlay_Common");
		FString RareMaterialPath		= TEXT("/WeaponMastery/Assets/Materials/M_Overlay_Rare.M_Overlay_Rare");
		FString EpicMaterialPath		= TEXT("/WeaponMastery/Assets/Materials/M_Overlay_Epic.M_Overlay_Epic");
		FString LegendaryMaterialPath	= TEXT("/WeaponMastery/Assets/Materials/M_Overlay_Legendary.M_Overlay_Legendary");
		FString MysticMaterialPath		= TEXT("/WeaponMastery/Assets/Materials/M_Overlay_Mystic.M_Overlay_Mystic");
		FString UniqueMaterialPath		= TEXT("/WeaponMastery/Assets/Materials/M_Overlay_Unique.M_Overlay_Unique");

		// Load materials from file paths.
		CommonMaterial		= Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *CommonMaterialPath));
		RareMaterial		= Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *RareMaterialPath));
		EpicMaterial		= Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *EpicMaterialPath));
		LegendaryMaterial	= Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *LegendaryMaterialPath));
		MysticMaterial		= Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *MysticMaterialPath));
		UniqueMaterial		= Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *UniqueMaterialPath));

		// Log a warning if any material fails to load.
		if (!CommonMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load CommonMaterial from path: %s"), *CommonMaterialPath);
		}
		if (!RareMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load RareMaterial from path: %s"), *RareMaterialPath);
		}
		if (!EpicMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load EpicMaterial from path: %s"), *EpicMaterialPath);
		}
		if (!LegendaryMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load LegendaryMaterial from path: %s"), *LegendaryMaterialPath);
		}
		if (!MysticMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load MysticMaterial from path: %s"), *MysticMaterialPath);
		}
		if (!UniqueMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load UniqueMaterial from path: %s"), *UniqueMaterialPath);
		}
	}
	// Getter functions for accessing individual materials.

	// Getter for CommonMaterial
	TObjectPtr<UMaterial> GetCommonMaterial() const
	{
		return CommonMaterial;
	}

	// Getter for RareMaterial
	TObjectPtr<UMaterial> GetRareMaterial() const
	{
		return RareMaterial;
	}

	// Getter for EpicMaterial
	TObjectPtr<UMaterial> GetEpicMaterial() const
	{
		return EpicMaterial;
	}

	// Getter for LegendaryMaterial
	TObjectPtr<UMaterial> GetLegendaryMaterial() const
	{
		return LegendaryMaterial;
	}

	// Getter for MysticMaterial
	TObjectPtr<UMaterial> GetMysticMaterial() const
	{
		return MysticMaterial;
	}

	// Getter for UniqueMaterial
	TObjectPtr<UMaterial> GetUniqueMaterial() const
	{
		return UniqueMaterial;
	}
};
#pragma endregion
#pragma region UWeaponMasteryComponent
UCLASS(ClassGroup=(Custom), DefaultToInstanced, meta=(BlueprintSpawnableComponent))
class WEAPONMASTERY_API UWeaponMasteryComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region WeaponMasteryComponent
public:	
	// Default constructor that sets initial values for this component's properties.
	UWeaponMasteryComponent();

	// Called after a property has been changed in the editor.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostLoad() override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void ConfigureDataAssets();

public:
	void SetWeaponType(EWeaponType InWeaponType);
#pragma endregion
#pragma region WeaponData
private:
	// Pointer to the weapon data object
	// This variable holds a reference to the UWeaponData object that contains the weapon's attributes.
	// It is visible anywhere in the editor, making it accessible for inspection and debugging purposes.
	UPROPERTY(VisibleAnywhere, Transient, Category = "WeaponData")
	TObjectPtr<UWeaponData> WeaponData;
public:
	// Setter function to assign weapon data
	// This function sets the WeaponData member variable to the given UWeaponData object.
	// @param InWeaponData - A TObjectPtr to the UWeaponData object that contains the weapon's attributes.
	void SetWeaponData(TObjectPtr<UWeaponData> InWeaponData)
	{
		WeaponData = InWeaponData;
	}

	// Getter function to access the weapon data
	// This function returns the current UWeaponData object associated with this instance.
	// @return TObjectPtr<UWeaponData> - A pointer to the UWeaponData object containing the weapon's attributes.
	FORCEINLINE TObjectPtr<UWeaponData> GetWeaponData()
	{
		return WeaponData;
	}
#pragma endregion
#pragma region AWeaponModule
private:
	// Pointer to the weapon module object
	// This variable holds a reference to the AWeaponModule object associated with this weapon.
	UPROPERTY()
	TObjectPtr<AWeaponModule> WeaponModule;

public:
	// Setter function to set the weapon module
	// This function sets the WeaponModule member variable to the given AWeaponModule object.
	// @param InWeaponModule - A TObjectPtr to the AWeaponModule object to be associated with this weapon.
	void SetWeaponModule(TObjectPtr<AWeaponModule> InWeaponModule);
#pragma endregion
#pragma region Property Definitions
protected:
	// Property to store weapon mastery data
	// This property holds data related to the weapon's mastery attributes.
	// It is editable anywhere in the editor and displays only inner properties.
	UPROPERTY(meta = (DisplayPriority = 1, ShowOnlyInnerProperties), EditAnywhere, Category = "Advance Weapon Master")
	FWeaponMastery WeaponMastery;

	// Property to store weapon effects data
	// This property holds data related to the weapon's effects.
	// It is editable anywhere in the editor and displays only inner properties.
	UPROPERTY(meta = (DisplayPriority = 2, ShowOnlyInnerProperties), EditAnywhere, Category = "Advance Weapon Master")
	FWeaponEffectAndProps WeaponEffectAndProps;

	// Property to store weapon details data
	// This property holds detailed data about the weapon.
	// It is editable anywhere in the editor and displays only inner properties.
	UPROPERTY(meta = (DisplayPriority = 3, ShowOnlyInnerProperties), EditAnywhere, Category = "Advance Weapon Master")
	FWeaponDetails WeaponDetails;

public:
	// Getter function to access the weapon mastery data
	// This function returns the current FWeaponMastery object associated with this weapon.
	// @return FWeaponMastery - The weapon mastery data.
	FWeaponMastery GetWeaponMastery() const
	{
		return WeaponMastery;
	}

	// Getter function to access the weapon effects data
	// This function returns the current FWeaponEffects object associated with this weapon.
	// @return FWeaponEffects - The weapon effects data.
	FWeaponEffectAndProps GetWeaponEffectAndProps() const
	{
		return WeaponEffectAndProps;
	}

	// Getter function to access the weapon details data
	// This function returns the current FWeaponDetails object associated with this weapon.
	// @return FWeaponDetails - The weapon details data.
	FWeaponDetails GetWeaponDetails() const
	{
		return WeaponDetails;
	}
#pragma endregion 
#pragma region Data-Props
protected:
	// Pointers to various data objects used for weapon attributes
	// These are UPROPERTY macro declarations which expose these pointers to Unreal Engine's reflection system
 
	// Pointer to attainment data object, which stores data related to the attainment of weapons.
	UPROPERTY(VisibleAnywhere, Category =  "DataCollection", Transient)
	TObjectPtr<UAttainmentData>				AttainmentData;

	// Pointer to rarity data object, which stores data related to the rarity of weapons.
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<URarityData>					RarityData;

	// Pointer to weapon state data object, which stores data related to the state of the weapon (e.g., dropped, equipped and more).
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<UWeaponStateData>			WeaponStateData;

	// Pointer to weapon type data object, which stores data related to the type of the weapon (e.g., short-range, long-ranged).
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<UWeaponTypeData>				WeaponTypeData;

	// Pointer to long-range weapon type data object, which stores data specific to long-range weapons.
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<ULongRangeWeaponTypeData>	LongRangeWeaponTypeData;

	// Pointer to ammunition type data object, which stores data related to the type of ammunition the weapon uses.
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<UAmmunitionTypeData>			AmmunitionTypeData;

	// Pointer to fire type data object, which stores data related to the fire type of the weapon(e.g., single or fully automatic).
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<UFireTypeData>				FireTypeData;

	// Pointer to fire mode data object, which stores data related to the fire mode of the weapon (e.g., semi-automatic, automatic).
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<UFireModeData>				FireModeData;

	// Pointer to short-range weapon type data object, which stores data specific to short-range weapons.
	UPROPERTY(VisibleAnywhere, Category = "DataCollection", Transient)
	TObjectPtr<UShortRangeWeaponTypeData>	ShortRangeWeaponTypeData;


	/**
	 * @brief Creates and duplicate a new instance of weapon data.
	 *
	 * This function is responsible for creating a new duplicated instance of weapon data.
	 * It loads the weapon data asset, duplicates it, and assigns a new name based on the owner's class name.
	 * The duplicated instance is then returned.
	 *
	 * @return A pointer to the newly created weapon data instance.
	 */
	void CreateWeaponDataInstance();

	/* Update Functions */
	// Functions to update the various weapon attributes based on the input type.
	// These functions take an input of specific enum types and update the corresponding data object.

	/**
	 * Updates the attainment data based on the specified attainment type.
	 *
	 * This function updates the attainment data for a weapon based on the specified attainment type.
	 *
	 * @param InAttainmentType The type of attainment to update data for.
	 *        - Select
	 *        - None
	 *        - Pickable
	 *        - Purchasable
	 *        - Loot-able
	 *        - Craft-able
	 *        - Tradeable
	 *        - Reward
	 *        - Upgradable
	 *
	 * @note This function modifies the internal state of the weapon's attainment data based on the specified type.
	 * @warning Ensure that the provided attainment type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported attainment types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateAttainmentData(EAttainmentType InAttainmentType);

	/**
	 * Updates the rarity effect process data based on the specified rarity effect process.
	 *
	 * This function updates the rarity effect process data based on the specified process type.
	 *
	 * @param InRarityEffectProcess The type of rarity effect process to update data for.
	 *        - Select
	 *        - Apply
	 *        - Remove
	 *
	 * @note This function modifies the internal state of the weapon's rarity effect process data based on the specified type.
	 * @warning Ensure that the provided rarity effect process type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported rarity effect process types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateRarityEffectProcessData(ERarityEffectProcess InRarityEffectProcess);


	/**
	 * Updates the rarity data based on the specified rarity type.
	 *
	 * This function updates the rarity data for a weapon based on the specified rarity type.
	 *
	 * @param InRarityType The type of rarity to update data for.
	 *        - Select
	 *        - Common
	 *        - Rare
	 *        - Epic
	 *        - Legendary
	 *        - Mystic
	 *        - Unique
	 *
	 * @note This function modifies the internal state of the weapon's rarity data based on the specified type.
	 * @warning Ensure that the provided rarity type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported rarity types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateRarityData(ERarityType InRarityType);


	/**
	 * Updates the weapon state data based on the specified weapon state.
	 *
	 * This function updates the weapon state data based on the specified state type.
	 *
	 * @param InWeaponState The type of weapon state to update data for.
	 *        - Select
	 *        - Initial
	 *        - Equipped
	 *        - EquippedSecondary
	 *        - Dropped
	 *        - Tradeable
	 *        - Purchasable
	 *        - Loot-able
	 *        - Craft-able
	 *        - Upgradable
	 *        - Reward
	 *        - Broken
	 *        - InInventory
	 *        - InStorage
	 *        - MAX
	 *
	 * @note This function modifies the internal state of the weapon's state data based on the specified type.
	 * @warning Ensure that the provided weapon state type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported weapon state types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateWeaponStateData(EWeaponState InWeaponState);


	/**
	 * Updates the weapon type data based on the specified weapon type.
	 *
	 * This function updates the weapon type data based on the specified type.
	 *
	 * @param InWeaponType The type of weapon to update data for.
	 *        - Select
	 *        - Unarmed
	 *        - ShortRange
	 *        - LongRange
	 *        - Throwable
	 *        - Explosive
	 *        - Heavy
	 *
	 * @note This function modifies the internal state of the weapon's type data based on the specified type.
	 * @warning Ensure that the provided weapon type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported weapon types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateWeaponTypeData(EWeaponType InWeaponType);

	/**
	 * Updates the long-range weapon type data based on the specified long-range weapon type.
	 *
	 * This function updates the long-range weapon type data based on the specified type.
	 *
	 * @param InLongRangeWeaponType The type of long-range weapon to update data for.
	 *        - Select
	 *        - Pistol
	 *        - AssaultRifle
	 *        - Sniper
	 *        - BattleRifle
	 *        - Submachine
	 *        - PDW
	 *        - LightMachine
	 *        - HeavyMachine
	 *        - Shotgun
	 *        - RocketLauncher
	 *        - GrenadeLauncher
	 *        - Bow
	 *        - Crossbow
	 *        - HiddenBlade
	 *        - ThrowingKnife
	 *
	 * @note This function modifies the internal state of the weapon's long-range weapon type data based on the specified type.
	 * @warning Ensure that the provided long-range weapon type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported long-range weapon types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateLongRangeWeaponTypeData(ELongRangeWeaponType InLongRangeWeaponType);


	/**
	 * Updates the ammunition type data based on the specified ammunition type.
	 *
	 * This function updates the ammunition type data based on the specified type.
	 *
	 * @param InAmmunitionType The type of ammunition to update data for.
	 *        - Select
	 *        - EAT_9mm
	 *        - EAT_45ACP
	 *        - EAT_556mm
	 *        - EAT_762mm
	 *        - EAT_50Caliber
	 *        - EAT_12Gauge
	 *        - EAT_40mm
	 *        - EAT_Rocket
	 *        - EAT_Grenade
	 *        - EAT_Arrow
	 *        - EAT_Bolt
	 *        - EAT_Cartridge
	 *
	 * @note This function modifies the internal state of the weapon's ammunition type data based on the specified type.
	 * @warning Ensure that the provided ammunition type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported ammunition types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateAmmunitionTypeData(EAmmunitionType InAmmunitionType);


	/**
	 * Updates the fire type data based on the specified fire type.
	 *
	 * This function updates the fire type data based on the specified type.
	 *
	 * @param InFireType The type of fire to update data for.
	 *        - Select
	 *        - ProjectileType
	 *        - HitScan
	 *
	 * @note This function modifies the internal state of the weapon's fire type data based on the specified type.
	 * @warning Ensure that the provided fire type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported fire types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateFireTypeData(EFireType InFireType);

	/**
	 * Updates the fire mode type data based on the specified fire mode type.
	 *
	 * This function updates the fire mode type data based on the specified type.
	 *
	 * @param InFireModeType The type of fire mode to update data for.
	 *        - Select
	 *        - SingleShot
	 *        - SemiAutomatic
	 *        - Automatic
	 *
	 * @note This function modifies the internal state of the weapon's fire mode type data based on the specified type.
	 * @warning Ensure that the provided fire mode type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported fire mode types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateFireModeTypeData(EFireModeType InFireModeType);


	/**
	 * Updates the short-range weapon type data based on the specified short-range weapon type.
	 *
	 * This function updates the short-range weapon type data based on the specified type.
	 *
	 * @param InShortRangeWeaponType The type of short-range weapon to update data for.
	 *        - Select
	 *        - Sword
	 *        - Dagger
	 *        - Mace
	 *        - GreatHammer
	 *        - Axe
	 *        - GreatAxe
	 *        - Spear
	 *        - Sickle
	 *        - HiddenBlade
	 *
	 * @note This function modifies the internal state of the weapon's short-range weapon type data based on the specified type.
	 * @warning Ensure that the provided short-range weapon type is valid and appropriate for the context in which this function is called.
	 * @todo Implement error handling for invalid or unsupported short-range weapon types.
	 * @deprecated This function may be deprecated in future versions in favor of a more generalized data update mechanism.
	 */
	void UpdateShortRangeWeaponTypeData(EShortRangeWeaponType InShortRangeWeaponType);


#pragma endregion
#pragma region WeaponDetails
private:
	/**
	 * Initializes the weapon master component's data.
	 */
	void InitializeWeaponMasterComponentDatas();

	/**
	 * Initializes the weapon data.
	 */
	void InitializeWeaponData();

protected:
	/**
	 * Updates the weapon state based on the specified attainment type.
	 *
	 * @param InAttainmentType The attainment type to update the weapon state for.
	 */
	void UpdateWeaponState(EAttainmentType InAttainmentType);

	/**
	 * Resets the operation of updating weapon rarity.
	 */
	void ResetUpdatingWeaponRarityOperation();

	/**
	 * Updates the weapon rarity based on the specified rarity type.
	 *
	 * @param InRarityType The rarity type to update the weapon rarity for.
	 */
	void UpdateWeaponRarity(ERarityType InRarityType);

	/**
	 * Gets the overlay material by rarity type.
	 *
	 * @param InRarityType The rarity type to get the overlay material for.
	 * @return The overlay material corresponding to the specified rarity type.
	 */
	UMaterialInterface* GetOverlayMaterialByRarity(ERarityType InRarityType);

	bool bWeaponRarityProcess = false;

/* Long Range Weapon Properties Updating */
public:
	/**
	 * Updates the ammunition type based on the specified long-range weapon type.
	 *
	 * @param InLongRangeWeaponType The long-range weapon type to update the ammunition type for.
	 */
	void UpdateAmmunitionType(ELongRangeWeaponType InLongRangeWeaponType);

	/**
	 * Updates the fire type based on the specified long-range weapon type.
	 *
	 * @param InLongRangeWeaponType The long-range weapon type to update the fire type for.
	 */
	void UpdateFireType(ELongRangeWeaponType InLongRangeWeaponType);

	/**
	 * Updates the fire mode based on the specified long-range weapon type.
	 *
	 * @param InLongRangeWeaponType The long-range weapon type to update the fire mode for.
	 */
	void UpdateFireMode(ELongRangeWeaponType InLongRangeWeaponType);

/*ShortRangeWeaponProperties*/
public:
	/**
	 * Returns a string representation of the attainment type.
	 *
	 * @param InAttainmentType The attainment type to convert to string.
	 *        - Select
	 *        - None
	 *        - Pickable
	 *        - Purchasable
	 *        - Loot-able
	 *        - Craft-able
	 *        - Tradeable
	 *        - Reward
	 *        - Upgradable
	 * @return A string representation of the attainment type.
	 */
	FString GetWeaponAttainmentTypeAsString(EAttainmentType InAttainmentType);

	/**
	 * Returns a string representation of the weapon state type.
	 *
	 * @param InWeaponState The weapon state type to convert to string.
	 *        - Select
	 *        - Initial
	 *        - Equipped
	 *        - EquippedSecondary
	 *        - Dropped
	 *        - Tradeable
	 *        - Purchasable
	 *        - Loot-able
	 *        - Craft-able
	 *        - Upgradable
	 *        - Reward
	 *        - Broken
	 *        - InInventory
	 *        - InStorage
	 * @return A string representation of the weapon state type.
	 */
	FString GetWeaponStateTypeAsString(EWeaponState InWeaponState);

	/**
	 * Returns a string representation of the rarity effect process.
	 *
	 * @param InRarityEffectProcess The rarity effect process to convert to string.
	 *        - Select
	 *        - Apply
	 *        - Remove
	 * @return A string representation of the rarity effect process.
	 */
	FString GetWeaponRarityEffectProcessAsString(ERarityEffectProcess InRarityEffectProcess);

	
	/**
	 * Returns a string representation of the rarity type.
	 *
	 * @param InRarityType The rarity type to convert to string.
	 *        - Select
	 *        - Common
	 *        - Rare
	 *        - Epic
	 *        - Legendary
	 *        - Mystic
	 *        - Unique
	 * @return A string representation of the rarity type.
	 */
	FString GetWeaponRarityTypeAsString(ERarityType InRarityType);


	/**
	 * Returns a string representation of the weapon type.
	 *
	 * @param InWeaponType The weapon type to convert to string.
	 *        - Select
	 *        - Unarmed
	 *        - ShortRange
	 *        - LongRange
	 *        - Throwable
	 *        - Explosive
	 *        - Heavy
	 * @return A string representation of the weapon type.
	 */
	FString GetWeaponTypeAsString(EWeaponType InWeaponType);

	/**
	 * Returns a string representation of the long-range weapon type.
	 *
	 * @param InLongRangeWeaponType The long-range weapon type to convert to string.
	 *        - Select
	 *        - Pistol
	 *        - AssaultRifle
	 *        - Sniper
	 *        - BattleRifle
	 *        - Submachine
	 *        - PDW
	 *        - LightMachine
	 *        - HeavyMachine
	 *        - Shotgun
	 *        - RocketLauncher
	 *        - GrenadeLauncher
	 *        - Bow
	 *        - Crossbow
	 *        - HiddenBlade
	 *        - ThrowingKnife
	 * @return A string representation of the long-range weapon type.
	 */
	FString GetLongRangeWeaponNameAsString(ELongRangeWeaponType InLongRangeWeaponType);


	/**
	 * Returns a string representation of the short-range weapon type.
	 *
	 * @param InShortRangeWeaponType The short-range weapon type to convert to string.
	 *        - Select
	 *        - Sword
	 *        - Dagger
	 *        - Mace
	 *        - GreatHammer
	 *        - Axe
	 *        - GreatAxe
	 *        - Spear
	 *        - Sickle
	 *        - HiddenBlade
	 * @return A string representation of the short-range weapon type.
	 */
	FString GetShortRangeWeaponNameAsString(EShortRangeWeaponType InShortRangeWeaponType);

	/**
	 * Returns a string representation of the ammunition type.
	 *
	 * @param InAmmunitionType The ammunition type to convert to string.
	 *        - Select
	 *        - 9mm
	 *        - .45 ACP
	 *        - 5.56mm
	 *        - 7.62mm
	 *        - .50 Caliber
	 *        - 12 Gauge
	 *        - 40mm Grenade
	 *        - Rocket
	 *        - Grenade
	 *        - Arrow
	 *        - Bolt
	 *        - Cartridge
	 * @return A string representation of the ammunition type.
	 */
	FString GetAmmunitionTypeAsString(EAmmunitionType InAmmunitionType);


	/**
	 * Returns a string representation of the damage type.
	 *
	 * @param InDamageType The damage type to convert to string.
	 *        - Select
	 *        - Single Damage
	 *        - Random Damage
	 * @return A string representation of the damage type.
	 */
	FString GetWeaponDamageTypeAsString(EWeaponDamageType InDamageType);


	/**
	 * Returns a string representation of the damage value.
	 *
	 * @param InDamage The damage value to convert to string.
	 * @return A string representation of the damage value.
	 */
	FString GetWeaponDamageAsString(int32 InDamage);


	/**
	 * Returns a string representation of the random damage range.
	 *
	 * @param InRandomDamageMin The minimum random damage value.
	 * @param InRandomDamageMax The maximum random damage value.
	 * @return A string representation of the random damage range.
	 */
	FString GetWeaponRandomDamageAsString(int32 InRandomDamageMin, int32 InRandomDamageMax);

	/**
	 * Returns a string representation of the penetration value.
	 *
	 * @param InPenetrationValue The penetration value to convert to string.
	 * @return A string representation of the penetration value.
	 */
	FString GetWeaponPenetrationValueAsString(float InPenetrationValue);

	/**
	 * Returns a string representation of the range value.
	 *
	 * @param InRangeValue The range value to convert to string.
	 * @return A string representation of the range value.
	 */
	FString GetWeaponRangeValueAsString(float InRangeValue);

	/**
	 * Returns a string representation of the accuracy value.
	 *
	 * @param InAccuracy The accuracy value to convert to string.
	 * @return A string representation of the accuracy value.
	 */
	FString GetWeaponAccuracyAsString(float InAccuracy);


	/**
	 * Returns a string representation of the fire type.
	 *
	 * @param InFireType The fire type to convert to string.
	 *        - Select
	 *        - ProjectileType
	 *        - HitScanType
	 * @return A string representation of the fire type.
	 */
	FString GetWeaponFireTypeAsString(EFireType InFireType);

	/**
	 * Returns a string representation of whether the weapon is fully automatic.
	 *
	 * @param bInAutomatic Whether the weapon is fully automatic.
	 * @return A string representation indicating whether the weapon is fully automatic or not.
	 */
	FString GetWeaponFullAutomaticAsString(bool bInAutomatic);

	/**
	 * Returns a string representation of the weapon's firing mode.
	 *
	 * @param InFireModeType The firing mode of the weapon.
	 *        - Select
	 *        - SingleShot
	 *        - SemiAutomatic
	 *        - Automatic
	 * @return A string representation of the weapon's firing mode.
	 */
	FString GetWeaponFireModeAsString(EFireModeType InFireModeType);

	/**
	 * Returns a string representation of the magazine capacity.
	 *
	 * @param InMagazineCapacity The magazine capacity to convert to string.
	 * @return A string representation of the magazine capacity.
	 */
	FString GetWeaponMagazineCapacityAsString(int32 InMagazineCapacity);

	/**
	 * Returns a string representation of the fire rate.
	 *
	 * @param InFireRate The fire rate to convert to string.
	 * @return A string representation of the fire rate.
	 */
	FString GetWeaponFireRateAsString(float InFireRate);

	/**
	 * Returns a string representation of the reload time.
	 *
	 * @param InReloadTime The reload time to convert to string.
	 * @return A string representation of the reload time.
	 */
	FString GetWeaponReloadTimeAsString(float InReloadTime);

	/**
	 * Returns a string representation of the attack speed.
	 *
	 * @param InAttackSpeed The attack speed to convert to string.
	 * @return A string representation of the attack speed.
	 */
	FString GetWeaponAttackSpeedAsString(float InAttackSpeed);

	/**
	 * Returns a string representation of the weight.
	 *
	 * @param InWeight The weight to convert to string.
	 * @return A string representation of the weight.
	 */
	FString GetWeaponWeightAsString(float InWeight);

	/**
	 * Returns a string representation of the durability.
	 *
	 * @param InDurability The durability to convert to string.
	 * @return A string representation of the durability.
	 */
	FString GetWeaponDurabilityAsString(float InDurability);

	/**
	 * Returns a string representation of the price.
	 *
	 * @param InPrice The price to convert to string.
	 * @return A string representation of the price.
	 */
	FString GetWeaponPriceAsString(float InPrice);

#pragma endregion
};
#pragma endregion