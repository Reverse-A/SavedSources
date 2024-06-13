#pragma region Includes
#include "Component/WeaponMasteryComponent.h"
#include "Engine/AssetManager.h"
#include "UObject/ConstructorHelpers.h"

#include <Actor/WeaponModule.h>

#include <thread>
#include <chrono>

#pragma endregion
#pragma region Constructor
FName NAME_Weapon = FName("Weapon Information");


void UWeaponMasteryComponent::CreateWeaponDataInstance()
{
    if (WeaponData == nullptr)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            // Get the class of the owning actor.
            UClass* OwnerClass = Owner->GetClass();
            if (OwnerClass)
            {
                // Get the name of the class without the "BP_" prefix and "_C" suffix.
                FString BlueprintName = OwnerClass->GetName();
                BlueprintName.RemoveFromStart(TEXT("BP_")); // Remove "BP_" prefix
                BlueprintName.RemoveFromEnd(TEXT("_C"));    // Remove "_C" suffix
                BlueprintName += "_WeaponData";

                WeaponData = NewObject<UWeaponData>(this, UWeaponData::StaticClass(), *BlueprintName);
                
                if (WeaponData != nullptr)
                {
                    WeaponData->SetFlags(RF_Public | RF_Transactional);
                    WeaponData->MarkPackageDirty();
                }
                else
                {
                    LOG_INVALID(WeaponData);
                }
            }
        }
    }
}

// Sets default values for this component's properties
UWeaponMasteryComponent::UWeaponMasteryComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
    // off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    // Create a new instance of weapon data.
    CreateWeaponDataInstance();

    ConfigureDataAssets();
     
    // Initialize component data related to weapon mastery.
    InitializeWeaponMasterComponentDatas();
}

#pragma endregion

#pragma region PostEditChangeProperty
// This function is called whenever a property is changed in the editor for an instance of UWeaponMasteryComponent.
void UWeaponMasteryComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    // Call the parent class implementation of PostEditChangeProperty to ensure base functionality is maintained.
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Ensure WeaponData is correctly initialized
    CreateWeaponDataInstance();

    ConfigureDataAssets();

    // Check if the changed property belongs to the FWeaponMastery struct.
    if (PropertyChangedEvent.Property != nullptr && PropertyChangedEvent.Property->GetOwnerStruct() == FWeaponMastery::StaticStruct())
    {
        // Get the name of the changed property.
        FName PropertyName = PropertyChangedEvent.Property->GetFName();

        // Check if the WeaponAttainmentType property was changed.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, WeaponAttainmentType))
        {
            // Update weapon state and attainment data properties based on the current WeaponAttainmentType.
            EAttainmentType NewAttainmentType = WeaponMastery.WeaponAttainmentType;
            UpdateWeaponState(NewAttainmentType);
            UpdateAttainmentData(NewAttainmentType);

            // Update WeaponData if available
            if (WeaponData != nullptr)
            {
                // Update the WeaponAttainmentType in WeaponData and convert it to a string for display.
                WeaponData->UpdateWeaponAttainmentType(NewAttainmentType);
                FString WeaponAttainmentTypeString = GetWeaponAttainmentTypeAsString(NewAttainmentType);
                WeaponData->UpdateWeaponAttainmentTypeAsString(WeaponAttainmentTypeString);

                EWeaponState CurrentWeaponState = WeaponMastery.WeaponStateType;
                WeaponData->UpdateWeaponStateType(CurrentWeaponState);
                FString WeaponStateTypeString = GetWeaponStateTypeAsString(CurrentWeaponState);
                WeaponData->UpdateWeaponStateTypeAsString(WeaponStateTypeString);
            }
            else
            {
                // Log an error if WeaponData is not valid.
                LOG_INVALID(WeaponData);
            }
        }       

        // If the WeaponStateType property was changed...
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, WeaponStateType))
        {
            // Update weapon state data based on the new WeaponStateType.
            EWeaponState NewWeaponState = WeaponMastery.WeaponStateType;
            UpdateWeaponStateData(NewWeaponState);

            // Update WeaponData if available.
            if (WeaponData != nullptr)
            {
                // Update the WeaponStateType in WeaponData and convert it to a string for display.
                WeaponData->UpdateWeaponStateType(NewWeaponState);
                FString NewWeaponStateTypeString = GetWeaponStateTypeAsString(NewWeaponState);
                WeaponData->UpdateWeaponStateTypeAsString(NewWeaponStateTypeString);
            }
            else
            {
                // Log an error if WeaponData is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the changed property is WeaponType.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, WeaponType))
        {
            // Get the new weapon type and convert it to a string.
            EWeaponType NewWeaponType = WeaponMastery.WeaponType;
            FString WeaponTypeString = GetWeaponTypeAsString(NewWeaponType);

            // Update the weapon type string in the WeaponDetails struct.
            WeaponDetails.WeaponTypeName = WeaponTypeString;

            // Update weapon type data based on the new weapon type.
            UpdateWeaponTypeData(NewWeaponType);

            // Update WeaponData if available.
            if (WeaponData != nullptr)
            {
                // Update the weapon type in WeaponData and update its string representation.
                WeaponData->UpdateWeaponType(NewWeaponType);
                WeaponData->UpdateWeaponTypeAsString(WeaponTypeString);
            }
            else
            {
                // Log error if WeaponData is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the changed property is RarityEffectProcess.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, RarityEffectProcess))
        {
            // Check the current rarity effect process.
            if (WeaponMastery.RarityEffectProcess == ERarityEffectProcess::Apply)
            {     
                // If the rarity effect process is Apply, set the weapon rarity type to Common and update associated data.
                ERarityType SelectedRarityCommon = ERarityType::Common;
                WeaponMastery.WeaponRarityType = SelectedRarityCommon;
                UpdateWeaponRarity(SelectedRarityCommon);

                // Update WeaponData if available.
                if (WeaponData != nullptr)
                {
                    // Update weapon rarity type and associated strings in WeaponData.
                    WeaponData->UpdateWeaponRarityType(SelectedRarityCommon);
                    FString WeaponRarityTypeString = GetWeaponRarityTypeAsString(SelectedRarityCommon);
                    ERarityEffectProcess ApplyRarityEffectProcess = ERarityEffectProcess::Apply;
                    FString WeaponRarityEffectProcessString = GetWeaponRarityEffectProcessAsString(ApplyRarityEffectProcess);
                    WeaponData->UpdateRarityEffectProcess(ApplyRarityEffectProcess);
                    WeaponData->UpdateRarityEffectProcessAsString(WeaponRarityEffectProcessString);
                    WeaponData->UpdateWeaponRarityTypeAsString(WeaponRarityTypeString);

                    // Update weapon rarity type and associated string in WeaponDetails.
                    WeaponDetails.Rarity = WeaponRarityTypeString;
                }
                else
                {
                    // Log error if WeaponData is not valid.
                    LOG_INVALID(WeaponData);
                }                
            }
            else if (WeaponMastery.RarityEffectProcess == ERarityEffectProcess::Select)
            {
                // If the rarity effect process is Select, set the weapon rarity type to Select and update associated data.
                ERarityType RarityTypeSelect = ERarityType::Select;
                WeaponMastery.WeaponRarityType = RarityTypeSelect;
                UpdateWeaponRarity(RarityTypeSelect);

                // Update WeaponData if available.
                if (WeaponData != nullptr)
                {
                    // Update weapon rarity type and associated strings in WeaponData.
                    WeaponData->UpdateWeaponRarityType(RarityTypeSelect);
                    FString WeaponRaritySelectString = GetWeaponRarityTypeAsString(RarityTypeSelect);
                    ERarityEffectProcess SelectRarityEffectProcess = ERarityEffectProcess::Select;
                    FString WeaponRarityEffectProcessString = GetWeaponRarityEffectProcessAsString(SelectRarityEffectProcess);
                    WeaponData->UpdateRarityEffectProcessAsString(WeaponRarityEffectProcessString);
                    WeaponData->UpdateWeaponRarityTypeAsString(WeaponRaritySelectString);

                    // Update weapon rarity type and associated string in WeaponDetails.
                    WeaponDetails.Rarity = WeaponRaritySelectString;
                }
                else
                {
                    // Log error if WeaponData is not valid.
                    LOG_INVALID(WeaponData);
                }                         
            }
            else if (WeaponMastery.RarityEffectProcess == ERarityEffectProcess::Remove)
            {              
                // If the rarity effect process is Remove, set the weapon rarity type to Select and update associated data.
                ERarityType RemoveRarityType = ERarityType::Select;
                WeaponMastery.WeaponRarityType = RemoveRarityType;
                UpdateWeaponRarity(RemoveRarityType);

                // Update WeaponData if available.
                if (WeaponData != nullptr)
                {
                    // Update weapon rarity type and associated strings in WeaponData.
                    WeaponData->UpdateWeaponRarityType(RemoveRarityType);
                    FString WeaponRaritySelectString = GetWeaponRarityTypeAsString(RemoveRarityType);
                    FString RemoveRarityEffectProcessString = TEXT("Removed");
                    WeaponData->UpdateRarityEffectProcessAsString(RemoveRarityEffectProcessString);
                    WeaponData->UpdateWeaponRarityTypeAsString(RemoveRarityEffectProcessString);

                    // Update weapon rarity type and associated string in WeaponDetails.
                    WeaponDetails.Rarity = WeaponRaritySelectString;
                }
                else
                {
                    // Log error if WeaponData is not valid.
                    LOG_INVALID(WeaponData);
                }             
            }
            // Update the rarity effect process data.
            UpdateRarityEffectProcessData(WeaponMastery.RarityEffectProcess);
        }

        // Check if the changed property is WeaponRarityType.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, WeaponRarityType))
        {
            // Get the new rarity type from WeaponMastery.
            ERarityType NewRarityType = WeaponMastery.WeaponRarityType;

            // Update weapon rarity data.
            UpdateWeaponRarity(NewRarityType);

            // Update Weapon rarity data.
            if (WeaponData != nullptr)
            {
                // Update weapon rarity type and its string representation in WeaponData.
                WeaponData->UpdateWeaponRarityType(NewRarityType);
                FString RarityTypeString = GetWeaponRarityTypeAsString(NewRarityType);
                WeaponData->UpdateWeaponRarityTypeAsString(RarityTypeString);

                // Update weapon rarity type and associated string in WeaponDetails.
                WeaponDetails.Rarity = RarityTypeString;
               
            }
            else
            {
                // Log error if WeaponData is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, WeaponDamageType))
        {
            // Define a float variable to represent zero
            float Zero = 0.0f;

            // Check the current value of the DamageType property
            if (WeaponMastery.WeaponDamageType == EWeaponDamageType::Select)
            {
                // If DamageType is 'Select':
                // Set damage-related properties to zero
                EWeaponDamageType SelectDamageType    = EWeaponDamageType::Select;
                FString SelectDamageTypeString  = GetWeaponDamageTypeAsString(SelectDamageType);
                WeaponMastery.Damage            = Zero;
                WeaponMastery.RandomDamageMin   = Zero;
                WeaponMastery.RandomDamageMax   = Zero;

                // Update damage in WeaponDetails with the selected damage type
                WeaponDetails.Damage = SelectDamageTypeString;
     
                // Update WeaponData with the selected damage type and zero values
                if (WeaponData != nullptr)
                {
                    // Update the damage type of the weapon in the data object to indicate it's set to the selected damage type.
                    WeaponData->UpdateDamageType(SelectDamageType);

                    // Update the string representation of the damage type property in the data object with the string value associated with the selected damage type.
                    WeaponData->UpdateDamageTypeAsString(SelectDamageTypeString);

                    // Set the base damage of the weapon to zero in the data object, indicating it has no base damage.
                    WeaponData->UpdateDamage(Zero);

                    // Update the string representation of the base damage property in the data object with the string value associated with the selected damage type.
                    WeaponData->UpdateDamageAsString(SelectDamageTypeString);

                    // Set both the minimum and maximum random damage values to zero in the data object, indicating no random damage.
                    WeaponData->UpdateRandomDamage(Zero, Zero);

                    // Update the string representation of the random damage properties in the data object with the string value associated with the selected damage type.
                    WeaponData->UpdateRandomDamageAsString(SelectDamageTypeString);

                }
                else
                {
                    // Log an error if WeaponData is not valid
                    LOG_INVALID(WeaponData);
                }
            }
            else if (WeaponMastery.WeaponDamageType == EWeaponDamageType::SingleDamage)
            {
                // If DamageType is 'SingleDamage':
                // Set damage-related properties to zero
                EWeaponDamageType SingleDamageType = EWeaponDamageType::SingleDamage;
                FString SingleDamageTypeString = GetWeaponDamageTypeAsString(SingleDamageType);
                FString ZeroString = TEXT("0");
                WeaponMastery.Damage = Zero;
                WeaponMastery.RandomDamageMin = Zero;
                WeaponMastery.RandomDamageMax = Zero;

                // Update Damage in WeaponDetails with the single damage type
                WeaponDetails.Damage = SingleDamageTypeString;

                // Update WeaponData with the single damage type and zero values
                if (WeaponData != nullptr)
                {
                    // Update the damage type of the weapon in the data object to indicate it's now set to single damage.
                    WeaponData->UpdateDamageType(SingleDamageType);

                    // Update the string representation of the damage type property in the data object with the string value "Single Damage".
                    WeaponData->UpdateDamageTypeAsString(SingleDamageTypeString);

                    // Set the base damage of the weapon to zero in the data object, indicating it has no base damage.
                    WeaponData->UpdateDamage(Zero);

                    // Update the string representation of the base damage property in the data object with the string value "0".
                    WeaponData->UpdateDamageAsString(ZeroString);

                    // Set both the minimum and maximum random damage values to zero in the data object, indicating no random damage.
                    WeaponData->UpdateRandomDamage(Zero, Zero);

                    // Update the string representation of the random damage properties in the data object with the string value associated with single damage.
                    WeaponData->UpdateRandomDamageAsString(SingleDamageTypeString);

                }
                else
                {
                    // Log an error if WeaponData is not valid
                    //LOG_INVALID(WeaponData);
              
                    UE_LOG
                    (
                        LogTemp,
                        Error,
                        TEXT("WeaponData is not valid in WeaponMasteryComponentClass. Function: %s, File: %s, Line: %d"),
                        TEXT(__FUNCTION__),
                        TEXT(__FILE__),
                        __LINE__
                    );
                }
            }
            else if (WeaponMastery.WeaponDamageType == EWeaponDamageType::RandomDamage)
            {
                // If DamageType is 'RandomDamage':
                // Set damage-related properties to zero
                EWeaponDamageType RandomDamageType    = EWeaponDamageType::RandomDamage;
                FString RandomDamageTypeString  = GetWeaponDamageTypeAsString(RandomDamageType);
                FString ZeroString = TEXT("0, 0");

                WeaponMastery.Damage          = Zero;
                WeaponMastery.RandomDamageMin = Zero;
                WeaponMastery.RandomDamageMax = Zero;

                // Update WeaponDetails with the random damage type
                WeaponDetails.Damage = RandomDamageTypeString;

                // Update WeaponData with the random damage type and zero values
                if (WeaponData != nullptr)
                {
                    // Update the damage type of the weapon in the data object to indicate it's now set to random damage.
                    WeaponData->UpdateDamageType(RandomDamageType);

                    // Update the string representation of the damage type property in the data object with the string value "Random Damage".
                    WeaponData->UpdateDamageTypeAsString(RandomDamageTypeString);

                    // Set the base damage of the weapon to zero in the data object, indicating it has no base damage.
                    WeaponData->UpdateDamage(Zero);

                    // Update the string representation of the base damage property in the data object with the string value associated with random damage.
                    WeaponData->UpdateDamageAsString(RandomDamageTypeString);

                    // Set both the minimum and maximum random damage values to zero in the data object, indicating no random damage.
                    WeaponData->UpdateRandomDamage(Zero, Zero);

                    // Update the string representation of the random damage properties in the data object with the string value "0, 0".
                    WeaponData->UpdateRandomDamageAsString(ZeroString);
                }
                else
                {
                    // Log an error if WeaponData is not valid
                    //LOG_INVALID(WeaponData); 
                    UE_LOG
                    (
                        LogTemp,
                        Error,
                        TEXT("WeaponData is not valid in WeaponMasteryComponentClass. Function: %s, File: %s, Line: %d"),
                        TEXT(__FUNCTION__),
                        TEXT(__FILE__),
                        __LINE__
                    );
                }
            }
        }

        // Check if the property being changed is the 'Damage' property of the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, Damage))
        {
            // If the damage type is set to 'SingleDamage', proceed with updating the damage.
            if (WeaponMastery.WeaponDamageType == EWeaponDamageType::SingleDamage)
            {
                // Retrieve the damage value from the 'WeaponMastery' struct.
                int32 WeaponDamage = WeaponMastery.Damage;

                // Convert the integer damage value to a string for display purposes.
                FString WeaponSingleTypeDamage = FString::FromInt(WeaponDamage);

                // Update the 'Damage' property in the 'WeaponDetails' struct with the new damage value.
                WeaponDetails.Damage = WeaponSingleTypeDamage;

                // If the 'WeaponData' object is valid, update the damage property in the data object.
                if (WeaponData != nullptr)
                {
                    // Update the damage value in the 'WeaponData' object.
                    WeaponData->UpdateDamage(WeaponDamage);  

                    // Update the string representation of the damage property in the 'WeaponData' object.
                    WeaponData->UpdateDamageAsString(WeaponSingleTypeDamage);
                }
                else
                {
                    // Log an error message if the 'WeaponData' object is invalid.
                    UE_LOG
                    (
                        LogTemp,
                        Error,
                        TEXT("WeaponData is not valid in WeaponMasteryComponentClass. Function: %s, File: %s, Line: %d"),
                        TEXT(__FUNCTION__),
                        TEXT(__FILE__),
                        __LINE__
                    );
                }
            }
        }

        // Check if either the 'RandomDamageMin' or 'RandomDamageMax' property is being changed in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, RandomDamageMin) || 
            PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, RandomDamageMax))
        {
            // If the damage type is set to 'RandomDamage', proceed with updating the random damage range.
            if (WeaponMastery.WeaponDamageType == EWeaponDamageType::RandomDamage)
            {
                if (WeaponMastery.RandomDamageMin >= WeaponMastery.RandomDamageMax)
                {
                    WeaponMastery.RandomDamageMax = WeaponMastery.RandomDamageMin;
                    WeaponMastery.RandomDamageMax++;
                }

                // Create a string representation of the random damage range.
                FString WeaponRandomTypeDamage = FString::Printf
                (TEXT("%d - %d"), WeaponMastery.RandomDamageMin, WeaponMastery.RandomDamageMax);

                // Update the 'Damage' property in the 'WeaponDetails' struct with the new random damage range.
                WeaponDetails.Damage = WeaponRandomTypeDamage;

                // If the 'WeaponData' object is valid, update the random damage properties in the data object.
                if (WeaponData != nullptr)
                {
                    // Retrieve the random damage minimum and maximum values from the 'WeaponMastery' struct.
                    int32 RandomDamageMin = WeaponMastery.RandomDamageMin;
                    int32 RandomDamageMax = WeaponMastery.RandomDamageMax;

                    // Update the random damage range in the 'WeaponData' object.
                    WeaponData->UpdateRandomDamage(RandomDamageMin, RandomDamageMax);

                    // Update the string representation of the random damage range in the 'WeaponData' object.
                    WeaponData->UpdateRandomDamageAsString(WeaponRandomTypeDamage);
                }
                else
                {
                    // Log an error message if the 'WeaponData' object is invalid.
                    LOG_INVALID(WeaponData);
                }
            }
        }

        // Check if the 'Penetration' property is being changed in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, Penetration))
        {
            // Convert the penetration value to a string representation.
            FString PenetrationString = GetWeaponPenetrationValueAsString(WeaponMastery.Penetration);

            // Update the 'Penetration' property in the 'WeaponDetails' struct with the new value.
            WeaponDetails.Penetration = PenetrationString;

            // If the 'WeaponData' object is valid, update the penetration properties in the data object.
            if (WeaponData != nullptr)
            {
                // Retrieve the penetration value from the 'WeaponMastery' struct.
                float PenetrationValue = WeaponMastery.Penetration;

                // Update the penetration value in the 'WeaponData' object.
                WeaponData->UpdatePenetration(PenetrationValue);

                // Update the string representation of the penetration value in the 'WeaponData' object.
                WeaponData->UpdatePenetrationAsString(PenetrationString);
            }
            else
            {
                // Log an error message if the 'WeaponData' object is invalid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the 'Range' property is being changed in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, Range))
        {
            // Convert the range value to a string representation.
            FString WeaponRangeString = GetWeaponRangeValueAsString(WeaponMastery.Range);

            // Update the 'Range' property in the 'WeaponDetails' struct with the new value.
            WeaponDetails.Range = WeaponRangeString;

            // If the 'WeaponData' object is valid, update the range properties in the data object.
            if (WeaponData != nullptr)
            {
                // Retrieve the range value from the 'WeaponMastery' struct.
                float RangeValue = WeaponMastery.Range;

                // Update the range value in the 'WeaponData' object.
                WeaponData->UpdateRange(RangeValue);

                // Update the string representation of the range value in the 'WeaponData' object.
                WeaponData->UpdateRangeAsString(WeaponRangeString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the 'Weight' property is being changed in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, Weight))
        {
            // Convert the weight value to a string representation.
            FString WeaponWeightString = GetWeaponWeightAsString(WeaponMastery.Weight);

            // Update the 'Weight' property in the 'WeaponDetails' struct with the new value.
            WeaponDetails.Weight = WeaponWeightString;

            // If the 'WeaponData' object is valid, update the weight properties in the data object.
            if (WeaponData != nullptr)
            {
                // Retrieve the weight value from the 'WeaponMastery' struct.
                float WeightValue = WeaponMastery.Weight;

                // Update the weight value in the 'WeaponData' object.
                WeaponData->UpdateWeight(WeightValue);

                // Update the string representation of the weight value in the 'WeaponData' object.
                WeaponData->UpdateWeightAsString(WeaponWeightString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the 'Durability' property is being changed in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, Durability))
        {
            // Convert the durability value to a string representation.
            FString WeaponDurabilityString = GetWeaponDurabilityAsString(WeaponMastery.Durability);

            // Update the 'Durability' property in the 'WeaponDetails' struct with the new value.
            WeaponDetails.Durability = WeaponDurabilityString;

            // If the 'WeaponData' object is valid, update the durability properties in the data object.
            if (WeaponData != nullptr)
            {
                // Retrieve the durability value from the 'WeaponMastery' struct.
                float DurabilityValue = WeaponMastery.Durability;

                // Update the durability value in the 'WeaponData' object.
                WeaponData->UpdateDurability(DurabilityValue);

                // Update the string representation of the durability value in the 'WeaponData' object.
                WeaponData->UpdateDurabilityAsString(WeaponDurabilityString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the 'Price' property is being changed in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, Price))
        {
            // Convert the price value to a string representation.
            FString WeaponPriceString = GetWeaponPriceAsString(WeaponMastery.Price);

            // Update the 'Price' property in the 'WeaponDetails' struct with the new value.
            WeaponDetails.Price = WeaponPriceString;

            // If the 'WeaponData' object is valid, update the price properties in the data object.
            if (WeaponData != nullptr)
            {
                // Retrieve the price value from the 'WeaponMastery' struct.
                float PriceValue = WeaponMastery.Price;

                // Update the price value in the 'WeaponData' object.
                WeaponData->UpdatePrice(PriceValue);

                // Update the string representation of the price value in the 'WeaponData' object.
                WeaponData->UpdatePriceAsString(WeaponPriceString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }
        #pragma region Long-Range Properties

        // Check if the property being changed is 'LongRangeWeaponType' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, LongRangeWeaponType))
        {
            // Update various properties related to the long-range weapon type in 'WeaponMastery'.

            // Update the ammunition type based on the current long-range weapon type.
            UpdateAmmunitionType (WeaponMastery.LongRangeWeaponType);

            // Update the fire type based on the current long-range weapon type.
            UpdateFireType (WeaponMastery.LongRangeWeaponType);

            // Update the fire mode based on the current long-range weapon type.
            UpdateFireMode (WeaponMastery.LongRangeWeaponType);

            // Update the weapon details with the corresponding values.

            // Get the name of the long-range weapon type as a string and set it in 'WeaponDetails'.
            WeaponDetails.Name              = GetLongRangeWeaponNameAsString(WeaponMastery.LongRangeWeaponType);

            // Get the fire type as a string and set it in 'WeaponDetails'.
            WeaponDetails.FireType          = GetWeaponFireTypeAsString(WeaponMastery.FireType);

            // Get the accuracy as a string and set it in 'WeaponDetails'.
            WeaponDetails.Accuracy          = GetWeaponAccuracyAsString(WeaponMastery.Accuracy);

            // Get the magazine capacity as a string and set it in 'WeaponDetails'.
            WeaponDetails.MagazineCapacity  = GetWeaponMagazineCapacityAsString(WeaponMastery.MagazineCapacity);

            // Get the fire rate as a string and set it in 'WeaponDetails'.
            WeaponDetails.FireRate          = GetWeaponFireRateAsString(WeaponMastery.FireRate);

            // Get the reload time as a string and set it in 'WeaponDetails'.
            WeaponDetails.ReloadTime        = GetWeaponReloadTimeAsString(WeaponMastery.ReloadTime);

            // Update additional data related to the long-range weapon type.
            UpdateLongRangeWeaponTypeData   (WeaponMastery.LongRangeWeaponType);

            // Check if 'WeaponData' is not null to avoid null pointer dereference.
            if (WeaponData != nullptr)
            {
                // Retrieve the long-range weapon type from 'WeaponMastery'.
                ELongRangeWeaponType LongRangeWeaponType = WeaponMastery.LongRangeWeaponType;

                // Update the long-range weapon type in 'WeaponData'.
                WeaponData->UpdateLongRangeWeaponType(LongRangeWeaponType);

                // Get the name of the long-range weapon type as a string.
                FString LongRangeWeaponTypeString = GetLongRangeWeaponNameAsString(LongRangeWeaponType);

                // Update the string representation of the long-range weapon type in 'WeaponData'.
                WeaponData->UpdateLongRangeWeaponTypeAsString(LongRangeWeaponTypeString);

                // Set a warning string for the short-range weapon type in 'WeaponData'.
                FString LongRangeWeaponTypeWarningString = TEXT("Long-Range Weapon Type");
                WeaponData->UpdateShortRangeWeaponTypeAsString(LongRangeWeaponTypeWarningString);

                // Retrieve and update the ammunition type in 'WeaponData'.
                EAmmunitionType AmmunitionType = WeaponMastery.AmmunitionType;
                WeaponData->UpdateAmmunitionType(AmmunitionType);
                FString AmmunitionTypeString = GetAmmunitionTypeAsString(AmmunitionType);
                WeaponData->UpdateAmmunitionTypeAsString(AmmunitionTypeString);
           
                // Retrieve and update the fire type in 'WeaponData'.
                EFireType FireType = WeaponMastery.FireType;
                WeaponData->UpdateFireType(FireType);
                FString FireTypeString = GetWeaponFireTypeAsString(FireType);
                WeaponData->UpdateFireTypeAsString(FireTypeString);

                // Retrieve and update the fire mode type in 'WeaponData'.
                EFireModeType FireModeType = WeaponMastery.FireModeType;
                WeaponData->UpdateFireModeType(FireModeType);
                FString FireModeString = GetWeaponFireModeAsString(FireModeType);
                WeaponData->UpdateFireModeTypeAsString(FireModeString);

                // Update the weapon name in 'WeaponData'.
                FString WeaponNameString = GetLongRangeWeaponNameAsString(WeaponMastery.LongRangeWeaponType);
                WeaponData->UpdateWeaponNameAsString(WeaponNameString);

                // Retrieve and update the accuracy in 'WeaponData'.
                float Accuracy = WeaponMastery.Accuracy;
                WeaponData->UpdateAccuracy(Accuracy);
                FString AccuracyString = GetWeaponAccuracyAsString(Accuracy);
                WeaponData->UpdateAccuracyAsString(AccuracyString);

                // Retrieve and update the magazine capacity in 'WeaponData'.
                int32 MagazineCapacity = WeaponMastery.MagazineCapacity;
                WeaponData->UpdateMagazineCapacity(MagazineCapacity);
                FString MagazineString = GetWeaponMagazineCapacityAsString(MagazineCapacity);
                WeaponData->UpdateMagazineCapacityAsString(MagazineString);

                // Retrieve and update the fire rate in 'WeaponData'.
                float FireRate = WeaponMastery.FireRate;
                WeaponData->UpdateFireRate(FireRate);
                FString FireRateString = GetWeaponFireRateAsString(FireRate);
                WeaponData->UpdateFireRateAsString(FireRateString);

                // Retrieve and update the reload time in 'WeaponData'.
                float ReloadTime = WeaponMastery.ReloadTime;
                WeaponData->UpdateReloadTime(ReloadTime);
                FString ReloadTimeString = GetWeaponReloadTimeAsString(ReloadTime);
                WeaponData->UpdateReloadTimeAsString(ReloadTimeString);

                FString Select = TEXT("Select");
                WeaponData->UpdateFullAutomaticAsString(Select);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }


        // Check if the property being changed is 'FireType' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, FireType))
        {
            // Check if "WeaponData" is not null to avoid null pointer dereference.
            if (WeaponData != nullptr)
            {
                // Retrieve the current fire type from "WeaponMastery".
                EFireType FireType = WeaponMastery.FireType;

                // Update the fire type 'WeaponData'.
                WeaponData->UpdateFireType(FireType);

                // Get the fire type as a string representation.
                FString FireTypeString = GetWeaponFireTypeAsString(FireType);

                // Update the string representation of the fire type in 'WeaponData'
                WeaponData->UpdateFireTypeAsString(FireTypeString);


                // Set the fire type string in 'WeaponDetails' structure.
                WeaponDetails.FireType = FireTypeString;
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the property being changed is 'bFullAutomatic' in the 'FWeaponMastery' struct. 
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, bFullAutomatic))
        {
            // Determine if the weapon type is a full-auto or burst weapon by checking against specific weapon types.
            bool IsFullAutoOrBurstWeapon =
                WeaponMastery.LongRangeWeaponType == ELongRangeWeaponType::AssaultRifle ||
                WeaponMastery.LongRangeWeaponType == ELongRangeWeaponType::Submachine ||
                WeaponMastery.LongRangeWeaponType == ELongRangeWeaponType::PDW ||
                WeaponMastery.LongRangeWeaponType == ELongRangeWeaponType::LightMachine ||
                WeaponMastery.LongRangeWeaponType == ELongRangeWeaponType::HeavyMachine;

            // If the weapon type supports full automatic or semi-automatic modes.
            if (IsFullAutoOrBurstWeapon)
            {
                // If the weapon is set to full automatic mode.
                if (WeaponMastery.bFullAutomatic)
                {
                    // Set the fire mode type to 'Automatic'.
                    WeaponMastery.FireModeType  = EFireModeType::Automatic;
                    FString FullAutomatic       = TEXT("Full-Automatic");
                    WeaponDetails.FireMode      = FullAutomatic;

                    // Check if 'WeaponData' is valid.
                    if (WeaponData != nullptr)
                    {
                        // Update the fire mode type and its string representation in 'WeaponData'.
                        EFireModeType FireModeType = WeaponMastery.FireModeType;
                        WeaponData->UpdateFireModeType(FireModeType);
                        FString FireModeTypeString = GetWeaponFireModeAsString(FireModeType);
                        WeaponData->UpdateFireModeTypeAsString(FireModeTypeString);
                    }
                    else
                    {
                        // Log an error message if 'WeaponData' is not valid.
                        LOG_INVALID(WeaponData);
                    }
                }
                // If the weapon is not set to full automatic mode (i.e., semi-automatic mode).
                else
                {
                    // Set the fire mode type to 'SemiAutomatic'.
                    WeaponMastery.FireModeType  = EFireModeType::SemiAutomatic;
                    FString SemiAutomatic       = TEXT("Semi-Automatic");
                    WeaponDetails.FireMode      = SemiAutomatic;

                    // Check if 'WeaponData' is valid.
                    if (WeaponData != nullptr)
                    {
                        // Update the fire mode type and its string representation in 'WeaponData'.
                        EFireModeType FireModeType = WeaponMastery.FireModeType;
                        WeaponData->UpdateFireModeType(FireModeType);
                        FString FireModeTypeString = GetWeaponFireModeAsString(FireModeType);
                        WeaponData->UpdateFireModeTypeAsString(FireModeTypeString);
                    }
                    else
                    {
                        // Log an error message if 'WeaponData' is not valid.
                        LOG_INVALID(WeaponData);
                    }
                }
            }
            // If the weapon type is a single-shot type.
            else
            {
                // Set the fire mode type to 'SingleShot'.
                WeaponMastery.FireModeType  = EFireModeType::SingleShot;
                FString SingleMode          = TEXT("Single-Mode");
                WeaponDetails.FireMode      = SingleMode;

                // Check if 'WeaponData' is valid.
                if (WeaponData != nullptr)
                {
                    // Update the fire mode type and its string representation in 'WeaponData'.
                    EFireModeType FireModeType = WeaponMastery.FireModeType;
                    WeaponData->UpdateFireModeType(FireModeType);
                    FString FireModeTypeString = GetWeaponFireModeAsString(FireModeType);
                    WeaponData->UpdateFireModeTypeAsString(FireModeTypeString);
                }
                else
                {
                    // Log an error message if 'WeaponData' is not valid.
                    LOG_INVALID(WeaponData);
                }
            }
            // Call to update fire mode type data after setting the fire mode.
            UpdateFireModeTypeData(WeaponMastery.FireModeType);
        }

        // Check if the property being changed is 'Accuracy' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, Accuracy))
        {
            // Convert the accuracy value to a string format.
            FString WeaponAccuracy = GetWeaponAccuracyAsString(WeaponMastery.Accuracy);

            // Update the 'Accuracy' field in 'WeaponDetails' with the converted string.
            WeaponDetails.Accuracy = WeaponAccuracy;

            // Check if 'WeaponData' is valid (not a null pointer).
            if (WeaponData != nullptr)
            {
                // Retrieve the accuracy value from 'WeaponMastery'.
                float Accuracy = WeaponMastery.Accuracy;

                // Update the 'Accuracy' value in 'WeaponData'.
                WeaponData->UpdateAccuracy(Accuracy);

                // Convert the accuracy value to a string again.
                FString AccuracyString = GetWeaponAccuracyAsString(Accuracy);

                // Update the 'Accuracy' string representation in 'WeaponData'.
                WeaponData->UpdateAccuracyAsString(AccuracyString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the property being changed is 'MagazineCapacity' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, MagazineCapacity))
        {
            // Convert the magazine capacity value to a string format.
            FString WeaponMagazineCapacity = GetWeaponMagazineCapacityAsString(WeaponMastery.MagazineCapacity);

            // Update the 'MagazineCapacity' field in 'WeaponDetails' with the converted string.
            WeaponDetails.MagazineCapacity = WeaponMagazineCapacity;

            // Check if 'WeaponData' is valid (not a null pointer).
            if (WeaponData != nullptr)
            {
                // Retrieve the magazine capacity value from 'WeaponMastery'.
                int32 MagazineCapacity = WeaponMastery.MagazineCapacity;

                // Update the 'MagazineCapacity' value in 'WeaponData'.
                WeaponData->UpdateMagazineCapacity(MagazineCapacity);

                // Convert the magazine capacity value to a string again.
                FString MagazineString = GetWeaponMagazineCapacityAsString(MagazineCapacity);

                // Update the 'MagazineCapacity' string representation in 'WeaponData'.
                WeaponData->UpdateMagazineCapacityAsString(MagazineString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the property being changed is 'FireRate' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, FireRate))
        {
            // Convert the fire rate value to a string format.
            FString WeaponFireRate = GetWeaponFireRateAsString(WeaponMastery.FireRate);

            // Update the 'FireRate' field in 'WeaponDetails' with the converted string.
            WeaponDetails.FireRate = WeaponFireRate;

            // Check if 'WeaponData' is valid (not a null pointer).
            if (WeaponData != nullptr)
            {
                // Retrieve the fire rate value from 'WeaponMastery'.
                float FireRate = WeaponMastery.FireRate;

                // Update the 'FireRate' value in 'WeaponData'.
                WeaponData->UpdateFireRate(FireRate);

                // Convert the fire rate value to a string again.
                FString FireRateString = GetWeaponFireRateAsString(FireRate);

                // Update the 'FireRate' string representation in 'WeaponData'.
                WeaponData->UpdateFireRateAsString(FireRateString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the property being changed is 'ReloadTime' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, ReloadTime))
        {
            if (WeaponData != nullptr)
            {
                // Check if 'WeaponData' is valid (not a null pointer).
                float ReloadTime = WeaponMastery.ReloadTime;

                // Update the 'ReloadTime' value in 'WeaponData'.
                WeaponData->UpdateReloadTime(ReloadTime);

                // Convert the reload time value to a string.
                FString ReloadTimeString = GetWeaponReloadTimeAsString(ReloadTime);

                // Update the string representation of 'ReloadTime' in 'WeaponData'.
                WeaponData->UpdateReloadTimeAsString(ReloadTimeString);

                // Update the 'ReloadTime' field in 'WeaponDetails' with the converted string.
                WeaponDetails.ReloadTime = ReloadTimeString;
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }
#pragma endregion
        #pragma region Short-Range Properties
        // Check if the property being changed is 'ShortRangeWeaponType' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, ShortRangeWeaponType))
        {
            // Retrieve the name of the short-range weapon type as a string.
            FString ShortRangeWeaponName    = GetShortRangeWeaponNameAsString(WeaponMastery.ShortRangeWeaponType);

            // Update the 'Name' field in 'WeaponDetails' with the short-range weapon name.
            WeaponDetails.Name              = ShortRangeWeaponName;
            
            // Set the 'AmmunitionTypeName' field in 'WeaponDetails' to "Not Adjustable".
            FString NotAdjustable = TEXT("Not Adjustable");
            WeaponDetails.AmmunitionTypeName = NotAdjustable;

            // Update the short-range weapon type data.
            UpdateShortRangeWeaponTypeData  (WeaponMastery.ShortRangeWeaponType);

            // Check if 'WeaponData' is valid (not a null pointer).
            if (WeaponData != nullptr)
            {
                // Retrieve the short-range weapon type from 'WeaponMastery'.
                EShortRangeWeaponType ShortRangeWeaponType = WeaponMastery.ShortRangeWeaponType;

                // Update the short-range weapon type in 'WeaponData'.
                WeaponData->UpdateShortRangeWeaponType(ShortRangeWeaponType);

                // Convert the short-range weapon type to a string.
                FString ShortRangeWeaponTypeString = GetShortRangeWeaponNameAsString(ShortRangeWeaponType);

                // Update the string representation of the short-range weapon type in 'WeaponData'.
                WeaponData->UpdateShortRangeWeaponTypeAsString(ShortRangeWeaponTypeString);
            }
            else
            {
                // Log an error message if 'WeaponData' is not valid.
                LOG_INVALID(WeaponData);
            }
        }

        // Check if the property being changed is 'AttackSpeed' in the 'FWeaponMastery' struct.
        if (PropertyName == GET_MEMBER_NAME_CHECKED(FWeaponMastery, AttackSpeed))
        {
            // Check if the weapon type is 'ShortRange'.
            if (WeaponMastery.WeaponType == EWeaponType::ShortRange)
            {
                // Retrieve the attack speed as a string.
                FString WeaponAttackSpeed = GetWeaponAttackSpeedAsString(WeaponMastery.AttackSpeed);

                // Update the 'AttackSpeed' field in 'WeaponDetails' with the attack speed.
                WeaponDetails.AttackSpeed = WeaponAttackSpeed;

                // Check if 'WeaponData' is valid (not a null pointer).
                if (WeaponData != nullptr)
                {
                    // Retrieve the attack speed from 'WeaponMastery'.
                    float AttackSpeed = WeaponMastery.AttackSpeed;

                    // Update the attack speed in 'WeaponData'.
                    WeaponData->UpdateAttackSpeed(AttackSpeed);

                    // Convert the attack speed to a string.
                    FString AttackSpeedString = GetWeaponAttackSpeedAsString(AttackSpeed);

                    // Update the string representation of the attack speed in 'WeaponData'.
                    WeaponData->UpdateAttackSpeedAsString(AttackSpeedString);
                }
                else
                {
                    // Log an error message if 'WeaponData' is not valid.
                    LOG_INVALID(WeaponData);
                }
            }
            // If the weapon type is 'LongRange'.
            else if (WeaponMastery.WeaponType == EWeaponType::LongRange)
            {
                // Set the attack speed to 'Not Adjustable' for long-range weapons.
                FString ShortRangeWeapon    = TEXT("Not Adjustable");
                WeaponDetails.AttackSpeed   = ShortRangeWeapon;

                // Check if 'WeaponData' is valid (not a null pointer).
                if (WeaponData != nullptr)
                {
                    // Retrieve the attack speed from 'WeaponMastery'.
                    float AttackSpeed = WeaponMastery.AttackSpeed;

                    // Update the attack speed in 'WeaponData'.
                    WeaponData->UpdateAttackSpeed(AttackSpeed);

                    // Convert the attack speed to a string.
                    FString AttackSpeedString = GetWeaponAttackSpeedAsString(AttackSpeed);

                    // Update the string representation of the attack speed in 'WeaponData'.
                    WeaponData->UpdateAttackSpeedAsString(AttackSpeedString);
                }
                else
                {
                    // Log an error message if 'WeaponData' is not valid.
                    LOG_INVALID(WeaponData);
                }
            }
        }
        #pragma endregion
    }
}

void UWeaponMasteryComponent::PostLoad()
{
    Super::PostLoad();

    // Ensure WeaponData is correctly initialized
    CreateWeaponDataInstance();

    ConfigureDataAssets();
}

void UWeaponMasteryComponent::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);

    if (Ar.IsLoading())
    {
        // Ensure WeaponData is correctly initialized when loading from disk
        CreateWeaponDataInstance();
        ConfigureDataAssets();
    }
}

void UWeaponMasteryComponent::ConfigureDataAssets()
{
    if (AttainmentData == nullptr)
    {
        UpdateAttainmentData(WeaponMastery.WeaponAttainmentType);
    }
    if (RarityData == nullptr)
    {
        UpdateRarityData(WeaponMastery.WeaponRarityType);
    }

    if (WeaponStateData == nullptr)
    {
        UpdateWeaponStateData(WeaponMastery.WeaponStateType);
    }
    if (WeaponTypeData == nullptr)
    {
        UpdateWeaponTypeData(WeaponMastery.WeaponType);
    }
    if (LongRangeWeaponTypeData == nullptr)
    {
        UpdateLongRangeWeaponTypeData(WeaponMastery.LongRangeWeaponType);
    }
    if (AmmunitionTypeData == nullptr)
    {
        UpdateAmmunitionTypeData(WeaponMastery.AmmunitionType);
    }

    if (FireTypeData == nullptr)
    {
        UpdateFireTypeData(WeaponMastery.FireType);
    }

    if (FireModeData == nullptr)
    {
        UpdateFireModeTypeData(WeaponMastery.FireModeType);
    }

    if (ShortRangeWeaponTypeData == nullptr)
    {
        UpdateShortRangeWeaponTypeData(WeaponMastery.ShortRangeWeaponType);
    } 
}

#pragma endregion
#pragma region WeaponModule
void UWeaponMasteryComponent::SetWeaponModule(TObjectPtr<AWeaponModule> InWeaponModule)
{
    WeaponModule = InWeaponModule;
    if (WeaponModule == nullptr)
    {
        if (GEngine)
        {
            // Log a message to the screen if the WeaponModule is set to nullptr
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Weapon Module is null"));
        }
        else
        {
            // Log to output log if GEngine is not available
            UE_LOG(LogTemp, Warning, TEXT("Weapon Module is null and GEngine is not available"));
        }
    }
    else
    {
        if (GEngine)
        {
            // Log a message to the screen if the WeaponModule is successfully set
            FString ModuleName = WeaponModule->GetName();
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Weapon Module %s set successfully"), *ModuleName));
        }
        else
        {
            // Log to output log if GEngine is not available
            UE_LOG(LogTemp, Log, TEXT("Weapon Module set successfully"));
        }

        // Example of further actions you might want to take when the WeaponModule is set
        // WeaponModule->Initialize();
    }
}
#pragma endregion
#pragma region DataProps

/*
     * UpdateAttainmentData Function:
     * -----------------------------
     * This function is responsible for updating the attainment data associated with the weapon mastery component.
     * It loads the attainment data blueprint object from a specified asset path and sets the attainment type based on the input parameter.
     * If the loading process succeeds and the attainment data object is valid, it updates the attainment type.
     * If the loading process fails or the attainment data object is invalid, it logs an error message for debugging purposes.
     *
     * Parameters:
     *  - InAttainmentType: The type of attainment to set for the weapon mastery component.
     *        - Select: Choose this option when selecting an attainment.
     *        - None: No specific attainment type selected.
     *        - Pickable: Attainment can be picked up.
     *        - Purchasable: Attainment can be purchased.
     *        - Loot-able: Attainment can be looted.
     *        - Craft-able: Attainment can be crafted.
     *        - Tradeable: Attainment can be traded.
     *        - Reward: Attainment is a reward.
     *        - Upgradable: Attainment can be upgraded.
     *
     * Return Type: void
*/
void UWeaponMasteryComponent::UpdateAttainmentData(EAttainmentType InAttainmentType)
{
    if (!AttainmentData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString BlueprintName = OwnerClass->GetName();
        BlueprintName.RemoveFromStart(TEXT("BP_")); // Remove "BP_" prefix
        BlueprintName.RemoveFromEnd(TEXT("_C"));    // Remove "_C" suffix
        FString InstanceName = BlueprintName + TEXT("_AttainmentDataInstance");

        AttainmentData = NewObject<UAttainmentData>(this, UAttainmentData::StaticClass(), *InstanceName);
        if (!AttainmentData)
        {
            LOG_INVALID(AttainmentData);
            return;
        }

        AttainmentData->SetFlags(RF_Public | RF_Transactional);
        AttainmentData->MarkPackageDirty();
    }

    if (AttainmentData)
    {
        AttainmentData->SetAttainmentType(InAttainmentType);
    }
    else
    {
        LOG_INVALID(AttainmentData);
    }
}



/*
     * UpdateRarityEffectProcessData Function:
     * ----------------------------------------
     * This function updates the rarity effect process data associated with the weapon mastery component.
     * It loads the rarity data blueprint object from a specified asset path and sets the rarity effect process based on the input parameter.
     * If the loading process succeeds and the rarity data object is valid, it updates the rarity effect process.
     * If the loading process fails or the rarity data object is invalid, it logs an error message for debugging purposes.
     *
     * Parameters:
     *  - InRarityEffectProcess: The rarity effect process to set for the weapon mastery component.
     *        - Select: Choose this option when selecting a rarity effect process.
     *        - Apply: Apply a rarity effect process.
     *        - Remove: Remove a rarity effect process.
     *
     * Return Type: void
 */
void UWeaponMasteryComponent::UpdateRarityEffectProcessData(ERarityEffectProcess InRarityEffectProcess)
{
    if (!RarityData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString BlueprintName = OwnerClass->GetName();
        BlueprintName.RemoveFromStart(TEXT("BP_"));
        BlueprintName.RemoveFromEnd(TEXT("_C"));
        FString InstanceName = BlueprintName + TEXT("_RarityDataInstance");

        RarityData = NewObject<URarityData>(this, URarityData::StaticClass(), *InstanceName);
        if (!RarityData)
        {
            LOG_INVALID(RarityData);
            return;
        }

        RarityData->SetFlags(RF_Public | RF_Transactional);
        RarityData->MarkPackageDirty();
    }

    if (RarityData)
    {
        RarityData->SetRarityEffectProcess(InRarityEffectProcess);
    }
    else
    {
        LOG_INVALID(RarityData);
    }
}



/*
 * UpdateRarityData Function:
 * --------------------------
 * This function updates the rarity data associated with the weapon mastery component.
 * It loads the rarity data blueprint object from a specified asset path and sets the rarity type based on the input parameter.
 * If the loading process succeeds and the rarity data object is valid, it updates the rarity type.
 * If the loading process fails or the rarity data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InRarityType: The rarity type to set for the weapon mastery component.
 *        - Select: Default value indicating no specific rarity.
 *        - Common: Indicating common rarity.
 *        - Rare: Indicating rare rarity.
 *        - Epic: Indicating epic rarity.
 *        - Legendary: Indicating legendary rarity.
 *        - Mystic: Indicating mystical rarity.
 *        - Unique: Indicating unique rarity.
 *
 * Return Type: void
*/
void UWeaponMasteryComponent::UpdateRarityData(ERarityType InRarityType)
{
    if (!RarityData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString InstanceName = OwnerClass->GetName();
        InstanceName.RemoveFromStart(TEXT("BP_"));
        InstanceName.RemoveFromEnd(TEXT("_C"));
        InstanceName += TEXT("_RarityDataInstance");

        RarityData = NewObject<URarityData>(this, *InstanceName);
        if (!RarityData)
        {
            LOG_INVALID(RarityData);
            return;
        }

        RarityData->SetFlags(RF_Public | RF_Transactional);
        RarityData->MarkPackageDirty();
    }

    if (RarityData)
    {
        RarityData->SetRarityType(InRarityType);
    }
    else
    {
        LOG_INVALID(RarityData);
    }
}




/*
 * UpdateWeaponStateData Function:
 * -------------------------------
 * This function updates the weapon state data associated with the weapon mastery component.
 * It loads the weapon state data blueprint object from a specified asset path and sets the weapon state based on the input parameter.
 * If the loading process succeeds and the weapon state data object is valid, it updates the weapon state.
 * If the loading process fails or the weapon state data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InWeaponState: The state to set for the weapon in the weapon mastery component.
 *        - Select: Default value indicating no specific state.
 *        - Initial: Initial state of the weapon.
 *        - Equipped: Weapon is equipped as the primary weapon.
 *        - EquippedSecondary: Weapon is equipped as a secondary weapon.
 *        - Dropped: Weapon is dropped on the ground.
 *        - Tradeable: Weapon is tradeable.
 *        - Purchasable: Weapon is available for purchase.
 *        - Loot-able: Weapon is available as loot.
 *        - Craft-able: Weapon is available for crafting.
 *        - Upgradable: Weapon can be upgraded.
 *        - Reward: Weapon can be obtained as a reward.
 *        - Broken: Weapon is broken and cannot be used.
 *        - InInventory: Weapon is in the player's inventory.
 *        - InStorage: Weapon is stored in a safe place.
 *        - MAX: Default maximum value.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateWeaponStateData(EWeaponState InWeaponState)
{
    if (!WeaponStateData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString InstanceName = OwnerClass->GetName();
        InstanceName.RemoveFromStart(TEXT("BP_"));
        InstanceName.RemoveFromEnd(TEXT("_C"));
        InstanceName += TEXT("_WeaponStateDataInstance");

        WeaponStateData = NewObject<UWeaponStateData>(this, *InstanceName);
        if (!WeaponStateData)
        {
            LOG_INVALID(WeaponStateData);
            return;
        }

        WeaponStateData->SetFlags(RF_Public | RF_Transactional);
        WeaponStateData->MarkPackageDirty();
    }

    if (WeaponStateData)
    {
        WeaponStateData->SetWeaponState(InWeaponState);
    }
    else
    {
        LOG_INVALID(WeaponStateData);
    }
}


/*
 * UpdateWeaponTypeData Function:
 * ------------------------------
 * This function updates the weapon type data associated with the weapon mastery component.
 * It loads the weapon type data blueprint object from a specified asset path and sets the weapon type based on the input parameter.
 * If the loading process succeeds and the weapon type data object is valid, it updates the weapon type.
 * If the loading process fails or the weapon type data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InWeaponType: The type to set for the weapon in the weapon mastery component.
 *        - Select: Default value indicating no specific weapon type.
 *        - Unarmed: No weapon, using fists.
 *        - ShortRange: Close combat weapons.
 *        - LongRange: Long range weapons.
 *        - Throwable: Weapons that can be thrown.
 *        - Explosive: Explosive weapons.
 *        - Heavy: Heavy weapons with significant firepower.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateWeaponTypeData(EWeaponType InWeaponType)
{
    if (!WeaponTypeData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString InstanceName = OwnerClass->GetName();
        InstanceName.RemoveFromStart(TEXT("BP_"));
        InstanceName.RemoveFromEnd(TEXT("_C"));
        InstanceName += TEXT("_WeaponTypeDataInstance");

        WeaponTypeData = NewObject<UWeaponTypeData>(this, *InstanceName);
        if (!WeaponTypeData)
        {
            LOG_INVALID(WeaponTypeData);
            return;
        }

        WeaponTypeData->SetFlags(RF_Public | RF_Transactional);
        WeaponTypeData->MarkPackageDirty();
    }

    if (WeaponTypeData)
    {
        WeaponTypeData->SetWeaponType(InWeaponType);
    }
    else
    {
        LOG_INVALID(WeaponTypeData);
    }
}




/*
 * UpdateLongRangeWeaponTypeData Function:
 * ---------------------------------------
 * This function updates the long-range weapon type data associated with the weapon mastery component.
 * It loads the long-range weapon type data blueprint object from a specified asset path and sets the long-range weapon type based on the input parameter.
 * If the loading process succeeds and the long-range weapon type data object is valid, it updates the long-range weapon type.
 * If the loading process fails or the long-range weapon type data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InLongRangeWeaponType: The type of long-range weapon to set for the weapon mastery component.
 *        - Select: Default type for unspecified ranged weapons.
 *        - Pistol: Handguns.
 *        - AssaultRifle: Assault rifles.
 *        - Sniper: Sniper rifles.
 *        - BattleRifle: Battle rifles.
 *        - Submachine: Submachine guns.
 *        - PDW: Personal Defense Weapons (PDWs).
 *        - LightMachine: Light machine guns.
 *        - HeavyMachine: Heavy machine guns.
 *        - Shotgun: Shotguns.
 *        - RocketLauncher: Rocket launchers.
 *        - GrenadeLauncher: Grenade launchers.
 *        - Bow: Bows.
 *        - Crossbow: Crossbows.
 *        - HiddenBlade: Hidden blade weapons (e.g., knives).
 *        - ThrowingKnife: Throwing knives.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateLongRangeWeaponTypeData(ELongRangeWeaponType InLongRangeWeaponType)
{
    if (!LongRangeWeaponTypeData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString InstanceName = OwnerClass->GetName();
        InstanceName.RemoveFromStart(TEXT("BP_"));
        InstanceName.RemoveFromEnd(TEXT("_C"));
        InstanceName += TEXT("_LongRangeWeaponTypeDataInstance");

        LongRangeWeaponTypeData = NewObject<ULongRangeWeaponTypeData>(this, *InstanceName);
        if (!LongRangeWeaponTypeData)
        {
            LOG_INVALID(LongRangeWeaponTypeData);
            return;
        }

        LongRangeWeaponTypeData->SetFlags(RF_Public | RF_Transactional);
        LongRangeWeaponTypeData->MarkPackageDirty();
    }

    if (LongRangeWeaponTypeData)
    {
        LongRangeWeaponTypeData->SetLongRangeWeaponType(InLongRangeWeaponType);
    }
    else
    {
        LOG_INVALID(LongRangeWeaponTypeData);
    }
}



/*
 * UpdateAmmunitionTypeData Function:
 * ----------------------------------
 * This function updates the ammunition type data associated with the weapon mastery component.
 * It loads the ammunition type data blueprint object from a specified asset path and sets the ammunition type based on the input parameter.
 * If the loading process succeeds and the ammunition type data object is valid, it updates the ammunition type.
 * If the loading process fails or the ammunition type data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InAmmunitionType: The type of ammunition to set for the weapon mastery component.
 *        - Select: Default type for unspecified ammunition.
 *        - EAT_9mm: 9mm ammunition.
 *        - EAT_45ACP: .45 ACP ammunition.
 *        - EAT_556mm: 5.56mm ammunition.
 *        - EAT_762mm: 7.62mm ammunition.
 *        - EAT_50Caliber: .50 caliber ammunition.
 *        - EAT_12Gauge: 12 gauge ammunition.
 *        - EAT_40mm: 40mm grenade.
 *        - EAT_Rocket: Rockets for launchers.
 *        - EAT_Grenade: General grenade.
 *        - EAT_Arrow: Arrows for bows.
 *        - EAT_Bolt: Bolts for crossbows.
 *        - EAT_Cartridge: General cartridge type.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateAmmunitionTypeData(EAmmunitionType InAmmunitionType)
{
    if (!AmmunitionTypeData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString BlueprintName = OwnerClass->GetName();
        BlueprintName.RemoveFromStart(TEXT("BP_"));
        BlueprintName.RemoveFromEnd(TEXT("_C"));

        FString InstanceName = BlueprintName + TEXT("_AmmunitionTypeDataInstance");

        AmmunitionTypeData = NewObject<UAmmunitionTypeData>(this, *InstanceName);
        if (!AmmunitionTypeData)
        {
            LOG_INVALID(AmmunitionTypeData);
            return;
        }

        AmmunitionTypeData->SetFlags(RF_Public | RF_Transactional);
        AmmunitionTypeData->MarkPackageDirty();
    }

    if (AmmunitionTypeData)
    {
        AmmunitionTypeData->SetAmmunitionType(InAmmunitionType);
    }
    else
    {
        LOG_INVALID(AmmunitionTypeData);
    }
}




/**
 * UpdateFireTypeData Function:
 * ----------------------------
 * This function updates the fire type data associated with the weapon mastery component.
 * It loads the fire type data blueprint object from a specified asset path and sets the fire type based on the input parameter.
 * If the loading process succeeds and the fire type data object is valid, it updates the fire type.
 * If the loading process fails or the fire type data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InFireType: The type of fire to set for the weapon mastery component.
 *        - Select: A default option for unspecified fire types.
 *        - Projectile: Represents fire types that use projectiles (e.g., bullets, arrows).
 *        - HitScan: Represents fire types that use the hit scan method for instant hit detection.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateFireTypeData(EFireType InFireType)
{
    if (!FireTypeData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString InstanceName = OwnerClass->GetName();
        InstanceName.RemoveFromStart(TEXT("BP_"));
        InstanceName.RemoveFromEnd(TEXT("_C"));
        InstanceName += TEXT("_FireTypeDataInstance");

        FireTypeData = NewObject<UFireTypeData>(this, *InstanceName);
        if (!FireTypeData)
        {
            LOG_INVALID(FireTypeData);
            return;
        }

        FireTypeData->SetFlags(RF_Public | RF_Transactional);
        FireTypeData->MarkPackageDirty();
    }

    if (FireTypeData)
    {
        FireTypeData->SetFireType(InFireType);
    }
    else
    {
        LOG_INVALID(FireTypeData);
    }
}


/**
 * UpdateFireModeTypeData Function:
 * --------------------------------
 * This function updates the fire mode type data associated with the weapon mastery component.
 * It loads the fire mode type data blueprint object from a specified asset path and sets the fire mode type based on the input parameter.
 * If the loading process succeeds and the fire mode type data object is valid, it updates the fire mode type.
 * If the loading process fails or the fire mode type data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InFireModeType: The type of fire mode to set for the weapon mastery component.
 *        - Select: Default option for unspecified fire mode.
 *        - SingleShot: Represents a single fire mode.
 *        - SemiAutomatic: Represents a semi-automatic fire mode.
 *        - Automatic: Represents a fully automatic fire mode.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateFireModeTypeData(EFireModeType InFireModeType)
{
    if (!FireModeData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString InstanceName = OwnerClass->GetName();
        InstanceName.RemoveFromStart(TEXT("BP_"));
        InstanceName.RemoveFromEnd(TEXT("_C"));
        InstanceName += TEXT("_FireModeDataInstance");

        FireModeData = NewObject<UFireModeData>(this, *InstanceName);
        if (!FireModeData)
        {
            LOG_INVALID(FireModeData);
            return;
        }

        FireModeData->SetFlags(RF_Public | RF_Transactional);
        FireModeData->MarkPackageDirty();
    }

    if (FireModeData)
    {
        FireModeData->SetFireModeType(InFireModeType);
    }
    else
    {
        LOG_INVALID(FireModeData);
    }
}

/**
 * UpdateShortRangeWeaponTypeData Function:
 * ----------------------------------------
 * This function updates the short-range weapon type data associated with the weapon mastery component.
 * It loads the short-range weapon type data blueprint object from a specified asset path and sets the short-range weapon type based on the input parameter.
 * If the loading process succeeds and the short-range weapon type data object is valid, it updates the short-range weapon type.
 * If the loading process fails or the short-range weapon type data object is invalid, it logs an error message for debugging purposes.
 *
 * Parameters:
 *  - InShortRangeWeaponType: The type of short-range melee weapon to set for the weapon mastery component.
 *        - Select: Default option for unspecified melee weapons.
 *        - Sword: Represents a sword melee weapon.
 *        - Dagger: Represents a dagger melee weapon.
 *        - Mace: Represents a mace melee weapon.
 *        - GreatHammer: Represents a great hammer melee weapon.
 *        - Axe: Represents an axe melee weapon.
 *        - GreatAxe: Represents a great axe melee weapon.
 *        - Spear: Represents a spear melee weapon.
 *        - Sickle: Represents a sickle melee weapon.
 *        - HiddenBlade: Represents a hidden blade melee weapon.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateShortRangeWeaponTypeData(EShortRangeWeaponType InShortRangeWeaponType)
{
    if (!ShortRangeWeaponTypeData)
    {
        AActor* Owner = GetOwner();
        if (!Owner)
        {
            LOG_INVALID(Owner);
            return;
        }

        UClass* OwnerClass = Owner->GetClass();
        if (!OwnerClass)
        {
            LOG_INVALID(OwnerClass);
            return;
        }

        FString InstanceName = OwnerClass->GetName();
        InstanceName.RemoveFromStart(TEXT("BP_"));
        InstanceName.RemoveFromEnd(TEXT("_C"));
        InstanceName += TEXT("_ShortRangeWeaponTypeDataInstance");

        ShortRangeWeaponTypeData = NewObject<UShortRangeWeaponTypeData>(this, *InstanceName);
        if (!ShortRangeWeaponTypeData)
        {
            LOG_INVALID(ShortRangeWeaponTypeData);
            return;
        }

        ShortRangeWeaponTypeData->SetFlags(RF_Public | RF_Transactional);
        ShortRangeWeaponTypeData->MarkPackageDirty();
    }

    if (ShortRangeWeaponTypeData)
    {
        ShortRangeWeaponTypeData->SetShortRangeWeaponType(InShortRangeWeaponType);
    }
    else
    {
        LOG_INVALID(ShortRangeWeaponTypeData);
    }
}


#pragma endregion
#pragma region WeaponMastery
void UWeaponMasteryComponent::SetWeaponType(EWeaponType InWeaponType)
{
    // Set the weapon type in the WeaponMastery component to the provided InWeaponType.
    WeaponMastery.WeaponType = InWeaponType;

    // Get the string representation of the weapon type and assign it to WeaponDetails.WeaponTypeName.
    WeaponDetails.WeaponTypeName = GetWeaponTypeAsString(InWeaponType);

    // Define a constant string representing "Not Adjustable" for clarity and ease of maintenance.
    FString NotAdjustable = TEXT("Not Adjustable");

    // Check if the provided weapon type is ShortRange.
    if (InWeaponType == EWeaponType::ShortRange)
    {
        // Set various attributes of WeaponDetails to "Not Adjustable" for short-range weapons.
        WeaponDetails.FireType          = NotAdjustable; // Type of fire (e.g., automatic, semi-automatic).
        WeaponDetails.FireMode          = NotAdjustable; // Mode of fire (e.g., single-shot, burst).
        WeaponDetails.MagazineCapacity  = NotAdjustable; // Number of rounds a magazine can hold.
        WeaponDetails.FireRate          = NotAdjustable; // Rate of fire (rounds fired per minute).
        WeaponDetails.ReloadTime        = NotAdjustable; // Time taken to reload the weapon.
        WeaponDetails.Accuracy          = NotAdjustable; // Accuracy of the weapon.
    }
    // If the provided weapon type is LongRange.
    else if (InWeaponType == EWeaponType::LongRange)
    {
        // Set the attack speed of the weapon to "Not Adjustable" for long-range weapons.
        WeaponDetails.AttackSpeed = NotAdjustable; // Speed at which the weapon's projectiles travel.
    }
}

/**
 * Initializes the data of the WeaponMasteryComponent.
 * This function resets all attributes of WeaponMasteryComponent to their default values.
 * Additionally, if WeaponData is valid, it updates the corresponding strings of weapon attributes.
 */
void UWeaponMasteryComponent::InitializeWeaponMasterComponentDatas()
{
    // Set default values for all weapon attributes to zero.
    float Zero = 0.0f;
    WeaponMastery.WeaponAttainmentType  = EAttainmentType::Select;
    WeaponMastery.WeaponStateType       = EWeaponState::Select;
    WeaponMastery.RarityEffectProcess   = ERarityEffectProcess::Select;
    WeaponMastery.WeaponRarityType      = ERarityType::Select;
    WeaponMastery.WeaponDamageType            = EWeaponDamageType::Select;
    WeaponMastery.Damage                = Zero;
    WeaponMastery.RandomDamageMin       = Zero;
    WeaponMastery.RandomDamageMax       = Zero;
    WeaponMastery.Penetration           = Zero;
    WeaponMastery.Range                 = Zero;
    WeaponMastery.LongRangeWeaponType   = ELongRangeWeaponType::Select;
    WeaponMastery.AmmunitionType        = EAmmunitionType::Select;
    WeaponMastery.FireType              = EFireType::Select;
    WeaponMastery.bFullAutomatic        = false;
    WeaponMastery.FireModeType          = EFireModeType::Select;
    WeaponMastery.Accuracy              = Zero;
    WeaponMastery.MagazineCapacity      = Zero;
    WeaponMastery.FireRate              = Zero;
    WeaponMastery.ReloadTime            = Zero;
    WeaponMastery.ShortRangeWeaponType  = EShortRangeWeaponType::Select;
    WeaponMastery.AttackSpeed           = Zero;
    WeaponMastery.Weight                = Zero;
    WeaponMastery.Durability            = Zero;
    WeaponMastery.Price                 = Zero;

    // If WeaponData is not null, update its attributes with default values and corresponding string representations.
    if (WeaponData != nullptr)
    {       
        // Get string representations of default values for each weapon attribute.
        FString WeaponAttainmentTypeString      = GetWeaponAttainmentTypeAsString(WeaponMastery.WeaponAttainmentType);
        FString WeaponStateTypeString           = GetWeaponStateTypeAsString(WeaponMastery.WeaponStateType);
        FString WeaponTypeString                = GetWeaponTypeAsString(WeaponMastery.WeaponType);
        FString WeaponRarityEffectProcessString = GetWeaponRarityEffectProcessAsString(WeaponMastery.RarityEffectProcess);
        FString WeaponRarityTypeString          = GetWeaponRarityTypeAsString(WeaponMastery.WeaponRarityType);
        FString WeaponDamageTypeString          = GetWeaponDamageTypeAsString(WeaponMastery.WeaponDamageType);
        FString WeaponSingleDamageString        = GetWeaponDamageAsString(WeaponMastery.Damage);
        FString WeaponRandomDamageString        = GetWeaponRandomDamageAsString(WeaponMastery.RandomDamageMin, WeaponMastery.RandomDamageMax);
        FString WeaponPenetrationString         = GetWeaponPenetrationValueAsString(WeaponMastery.Penetration);
        FString WeaponRangeString               = GetWeaponRangeValueAsString(WeaponMastery.Range);
        FString WeaponLongRangeWeaponTypeString = GetLongRangeWeaponNameAsString(WeaponMastery.LongRangeWeaponType);
        FString WeaponAmmunitionTypeString      = GetAmmunitionTypeAsString(WeaponMastery.AmmunitionType);
        FString WeaponFireTypeString            = GetWeaponFireTypeAsString(WeaponMastery.FireType);
        FString WeaponIsFullAutomaticString     = GetWeaponFullAutomaticAsString(WeaponMastery.bFullAutomatic);
        FString WeaponFireModeString            = GetWeaponFireModeAsString(WeaponMastery.FireModeType);
        FString WeaponAccuracyString            = GetWeaponAccuracyAsString(WeaponMastery.Accuracy);
        FString WeaponMagazineCapacityString    = GetWeaponMagazineCapacityAsString(WeaponMastery.MagazineCapacity);
        FString WeaponFireRateString            = GetWeaponFireRateAsString(WeaponMastery.FireRate);
        FString WeaponReloadTimeString          = GetWeaponReloadTimeAsString(WeaponMastery.ReloadTime);
        FString WeaponShortRangeWeaponTypeString = GetShortRangeWeaponNameAsString(WeaponMastery.ShortRangeWeaponType);
        FString WeaponAttackSpeedString         = GetWeaponAttackSpeedAsString(WeaponMastery.AttackSpeed);
        FString WeaponWeightString              = GetWeaponWeightAsString(WeaponMastery.Weight);
        FString WeaponDurabilityString          = GetWeaponDurabilityAsString(WeaponMastery.Durability);
        FString WeaponPrice                     = GetWeaponPriceAsString(WeaponMastery.Price);

        // Update WeaponData's attributes with default values.
        WeaponData->UpdateWeaponAttainmentType(EAttainmentType::Select);
        WeaponData->UpdateWeaponStateType(EWeaponState::Select);
        WeaponData->UpdateRarityEffectProcess(ERarityEffectProcess::Select);
        WeaponData->UpdateWeaponRarityType(ERarityType::Select);
        WeaponData->UpdateDamageType(EWeaponDamageType::Select);
        WeaponData->UpdateDamage(Zero);
        WeaponData->UpdateRandomDamage(Zero, Zero);
        WeaponData->UpdatePenetration(Zero);
        WeaponData->UpdateRange(Zero);
        WeaponData->UpdateLongRangeWeaponType(ELongRangeWeaponType::Select);
        WeaponData->UpdateAmmunitionType(EAmmunitionType::Select);
        WeaponData->UpdateFireType(EFireType::Select);
        WeaponData->UpdateFullAutomatic(false);
        WeaponData->UpdateFireModeType(EFireModeType::Select);
        WeaponData->UpdateAccuracy(Zero);
        WeaponData->UpdateMagazineCapacity(Zero);
        WeaponData->UpdateFireRate(Zero);
        WeaponData->UpdateReloadTime(Zero);
        WeaponData->UpdateShortRangeWeaponType(EShortRangeWeaponType::Select);
        WeaponData->UpdateAttackSpeed(Zero);
        WeaponData->UpdateWeight(Zero);
        WeaponData->UpdateDurability(Zero);
        WeaponData->UpdatePrice(Zero);

        // Update WeaponData's attributes with their corresponding string representations.
        WeaponData->UpdateWeaponAttainmentTypeAsString  (WeaponAttainmentTypeString);
        WeaponData->UpdateWeaponStateTypeAsString       (WeaponStateTypeString);
        WeaponData->UpdateWeaponTypeAsString            (WeaponTypeString);
        WeaponData->UpdateRarityEffectProcessAsString   (WeaponRarityEffectProcessString);
        WeaponData->UpdateWeaponRarityTypeAsString      (WeaponRarityTypeString);
        WeaponData->UpdateDamageTypeAsString            (WeaponDamageTypeString);
        WeaponData->UpdateDamageAsString                (WeaponSingleDamageString);
        WeaponData->UpdateRandomDamageAsString          (WeaponRandomDamageString);
        WeaponData->UpdatePenetrationAsString           (WeaponPenetrationString);
        WeaponData->UpdateRangeAsString                 (WeaponRangeString);
        WeaponData->UpdateLongRangeWeaponTypeAsString   (WeaponLongRangeWeaponTypeString);
        WeaponData->UpdateAmmunitionTypeAsString        (WeaponAmmunitionTypeString);
        WeaponData->UpdateFireTypeAsString              (WeaponFireTypeString);
        WeaponData->UpdateFullAutomaticAsString         (WeaponIsFullAutomaticString);
        WeaponData->UpdateFireModeTypeAsString          (WeaponFireModeString);
        WeaponData->UpdateAccuracyAsString              (WeaponAccuracyString);
        WeaponData->UpdateMagazineCapacityAsString      (WeaponMagazineCapacityString);
        WeaponData->UpdateFireRateAsString              (WeaponFireRateString);
        WeaponData->UpdateReloadTimeAsString            (WeaponReloadTimeString);
        WeaponData->UpdateShortRangeWeaponTypeAsString  (WeaponShortRangeWeaponTypeString);
        WeaponData->UpdateAttackSpeedAsString           (WeaponAttackSpeedString);
        WeaponData->UpdateWeightAsString                (WeaponWeightString);
        WeaponData->UpdateDurabilityAsString            (WeaponDurabilityString);
        WeaponData->UpdatePriceAsString                 (WeaponPrice);
        WeaponData->UpdateWeaponNameAsString            (TEXT("Not Adjusted!"));
    }
}

/**
 * Initializes the data of the WeaponDetails struct.
 * This function sets various attributes of WeaponDetails based on WeaponMastery's data.
 * If WeaponData is not null, it updates WeaponData's attributes with corresponding values.
 */
void UWeaponMasteryComponent::InitializeWeaponData()
{
    // Set the rarity of the weapon based on WeaponMastery's WeaponRarityType.
    WeaponDetails.Rarity  = GetWeaponRarityTypeAsString(WeaponMastery.WeaponRarityType);
 
    // Determine the type-specific attributes based on WeaponMastery's WeaponType.
    if (WeaponMastery.WeaponType == EWeaponType::LongRange)
    {
        WeaponDetails.WeaponTypeName = GetWeaponTypeAsString(EWeaponType::LongRange);
        WeaponDetails.Name = GetLongRangeWeaponNameAsString(WeaponMastery.LongRangeWeaponType);
    }
    else if (WeaponMastery.WeaponType == EWeaponType::ShortRange)
    {
        WeaponDetails.WeaponTypeName = GetWeaponTypeAsString(EWeaponType::ShortRange);
        WeaponDetails.Name = GetShortRangeWeaponNameAsString(WeaponMastery.ShortRangeWeaponType);
    }

    // Update WeaponDetails with various weapon attributes from WeaponMastery.
    WeaponDetails.AmmunitionTypeName    = GetAmmunitionTypeAsString         (WeaponMastery.AmmunitionType);
    WeaponDetails.Damage                = GetWeaponDamageAsString           (WeaponMastery.Damage);
    WeaponDetails.Penetration           = GetWeaponPenetrationValueAsString (WeaponMastery.Penetration);
    WeaponDetails.Range                 = GetWeaponRangeValueAsString       (WeaponMastery.Range);
    WeaponDetails.Accuracy              = GetWeaponAccuracyAsString         (WeaponMastery.Accuracy);
    WeaponDetails.FireMode              = GetWeaponFireModeAsString         (WeaponMastery.FireModeType);
    WeaponDetails.MagazineCapacity      = GetWeaponMagazineCapacityAsString (WeaponMastery.MagazineCapacity);
    WeaponDetails.FireRate              = GetWeaponFireRateAsString         (WeaponMastery.FireRate);
    WeaponDetails.ReloadTime            = GetWeaponReloadTimeAsString       (WeaponMastery.ReloadTime);
    WeaponDetails.AttackSpeed           = GetWeaponAttackSpeedAsString      (WeaponMastery.AttackSpeed);
    WeaponDetails.Durability            = GetWeaponDurabilityAsString       (WeaponMastery.Durability);
    WeaponDetails.Price                 = GetWeaponPriceAsString            (WeaponMastery.Price);
    WeaponDetails.Weight                = GetWeaponWeightAsString           (WeaponMastery.Price);

    // If WeaponData is not null, update its attributes with corresponding values.
    if (WeaponData != nullptr)
    {
        // Get string representations of various weapon attributes for WeaponData update.
        FString AttainmentType          = GetWeaponAttainmentTypeAsString       (WeaponMastery.WeaponAttainmentType);
        FString WeaponStateType         = GetWeaponStateTypeAsString            (WeaponMastery.WeaponStateType);
        FString WeaponType              = GetWeaponTypeAsString                 (WeaponMastery.WeaponType);
        FString RarityEffectProcess     = GetWeaponRarityEffectProcessAsString  (WeaponMastery.RarityEffectProcess);
        FString WeaponRarityType        = GetWeaponRarityTypeAsString           (WeaponMastery.WeaponRarityType);
        FString Penetration             = GetWeaponPenetrationValueAsString     (WeaponMastery.Penetration);
        FString Range                   = GetWeaponRangeValueAsString           (WeaponMastery.Range);
        FString Weight                  = GetWeaponWeightAsString               (WeaponMastery.Weight);
        FString Durability              = GetWeaponDurabilityAsString           (WeaponMastery.Durability);
        FString Price                   = GetWeaponPriceAsString                (WeaponMastery.Price);
        FString LongRangeWeaponType     = GetLongRangeWeaponNameAsString        (WeaponMastery.LongRangeWeaponType);
        FString ShortRangeWeaponType    = GetShortRangeWeaponNameAsString       (WeaponMastery.ShortRangeWeaponType);
        FString AmmunitionType          = GetAmmunitionTypeAsString         (WeaponMastery.AmmunitionType);
        FString FireType                = GetWeaponFireTypeAsString         (WeaponMastery.FireType);
        FString FullAutomatic           = GetWeaponFullAutomaticAsString        (WeaponMastery.bFullAutomatic);
        FString FireMode                = GetWeaponFireModeAsString             (WeaponMastery.FireModeType);
        FString Accuracy                = GetWeaponAccuracyAsString             (WeaponMastery.Accuracy);
        FString MagazineCapacity        = GetWeaponMagazineCapacityAsString     (WeaponMastery.MagazineCapacity);
        FString FireRate                = GetWeaponFireRateAsString             (WeaponMastery.FireRate);
        FString ReloadTime              = GetWeaponReloadTimeAsString           (WeaponMastery.ReloadTime);
        FString AttackSpeed             = GetWeaponAttackSpeedAsString          (WeaponMastery.AttackSpeed);
        FString WeaponName              = TEXT("Un-known");

        // Update WeaponData with weapon attributes and their string representations.
        WeaponData->UpdateWeaponAttainmentTypeAsString(AttainmentType);
        WeaponData->UpdateWeaponStateTypeAsString(WeaponStateType);
        WeaponData->UpdateWeaponTypeAsString(WeaponType);
        WeaponData->UpdateRarityEffectProcessAsString(RarityEffectProcess);
        WeaponData->UpdateWeaponRarityTypeAsString(WeaponRarityType);

        // Determine the type of damage and update WeaponData accordingly.
        bool bSingleDamage = WeaponMastery.WeaponDamageType == EWeaponDamageType::SingleDamage;
        bool bRandomDamage = WeaponMastery.WeaponDamageType == EWeaponDamageType::RandomDamage;
        bool bSelectDamage = WeaponMastery.WeaponDamageType == EWeaponDamageType::Select;
        if (bSingleDamage)
        {
            // Update WeaponData with single damage value.
            FString Damage = GetWeaponDamageAsString(WeaponMastery.Damage);
            WeaponData->UpdateDamageTypeAsString(Damage);
        }
        else if (bRandomDamage)
        {
            // Update WeaponData with random damage range.
            FString WeaponRandomTypeDamage = FString::Printf
            (TEXT("%d - %d"), WeaponMastery.RandomDamageMin, WeaponMastery.RandomDamageMax);

            WeaponDetails.Damage = WeaponRandomTypeDamage;
            WeaponData->UpdateRandomDamageAsString(WeaponRandomTypeDamage);
        }
        else if (bSelectDamage)
        {
            // Update WeaponData with 'NO DATA' for damage.
            FString SelectDamage = TEXT("NO DATA");
            WeaponData->UpdateDamageAsString(SelectDamage);
            WeaponData->UpdateRandomDamageAsString(SelectDamage);
        }
        WeaponData->UpdatePenetrationAsString(Penetration);
        WeaponData->UpdateRangeAsString(Range);
        WeaponData->UpdateWeightAsString(Weight);
        WeaponData->UpdateDurabilityAsString(Durability);
        WeaponData->UpdatePriceAsString(Price);
        WeaponData->UpdateLongRangeWeaponTypeAsString(LongRangeWeaponType);
        WeaponData->UpdateShortRangeWeaponTypeAsString(ShortRangeWeaponType);
        WeaponData->UpdateAmmunitionTypeAsString(AmmunitionType);
        WeaponData->UpdateFireTypeAsString(FireType);
        WeaponData->UpdateFullAutomaticAsString(FullAutomatic);
        WeaponData->UpdateFireModeTypeAsString(FireMode);
        WeaponData->UpdateAccuracyAsString(Accuracy);
        WeaponData->UpdateMagazineCapacityAsString(MagazineCapacity);
        WeaponData->UpdateFireRateAsString(FireRate);
        WeaponData->UpdateReloadTimeAsString(ReloadTime);
        WeaponData->UpdateAttackSpeedAsString(AttackSpeed);


        // Determine the name of the weapon based on its type and update WeaponData.
        if (WeaponMastery.WeaponType == EWeaponType::LongRange)
        {
            ELongRangeWeaponType CurrentLongRangeWeaponType = WeaponMastery.LongRangeWeaponType;

            WeaponName = GetLongRangeWeaponNameAsString(CurrentLongRangeWeaponType);
        }
        else if (WeaponMastery.WeaponType == EWeaponType::ShortRange)
        {
            EShortRangeWeaponType CurrentShortRangeWeaponType = WeaponMastery.ShortRangeWeaponType;

            WeaponName = GetShortRangeWeaponNameAsString(CurrentShortRangeWeaponType);
        }
        WeaponData->UpdateWeaponNameAsString(WeaponName);
    }
    else
    {
        // Log an error if WeaponData is not valid.
        UE_LOG
        (
            LogTemp,
            Error,
            TEXT("WeaponData is not valid in WeaponMasteryComponentClass. Function: %s, File: %s, Line: %d"),
            TEXT(__FUNCTION__),
            TEXT(__FILE__),
            __LINE__
        );
    }
}

/**
 * Updates the state of the weapon based on the provided attainment type.
 * This function maps the attainment type to a corresponding weapon state.
 * If the provided attainment type does not match any known types, it sets the weapon state to 'Select'.
 */
void UWeaponMasteryComponent::UpdateWeaponState(EAttainmentType	InAttainmentType)
{
    // Update WeaponStateType based on WeaponAttainmentType
    switch (InAttainmentType)
    {
    case EAttainmentType::Pickable:
        // Set weapon state to 'Initial' when it's pickable.
        WeaponMastery.WeaponStateType = EWeaponState::Initial;
        break;
    case EAttainmentType::Purchasable:
        // Set weapon state to 'Purchasable' when it's purchasable.
        WeaponMastery.WeaponStateType = EWeaponState::Purchasable;
        break;
    case EAttainmentType::Lootable:
        // Set weapon state to 'Lootable' when it's lootable.
        WeaponMastery.WeaponStateType = EWeaponState::Lootable;
        break;
    case EAttainmentType::Craftable:
        // Set weapon state to 'Craftable' when it's craftable.
        WeaponMastery.WeaponStateType = EWeaponState::Craftable;
        break;
    case EAttainmentType::Tradeable:
        // Set weapon state to 'Tradeable' when it's tradeable.
        WeaponMastery.WeaponStateType = EWeaponState::Tradeable;
        break;
    case EAttainmentType::Reward:
        // Set weapon state to 'Reward' when it's a reward.
        WeaponMastery.WeaponStateType = EWeaponState::Reward;
        break;
    case EAttainmentType::Upgradable:
        // Set weapon state to 'Upgradable' when it's upgradable.
        WeaponMastery.WeaponStateType = EWeaponState::Upgradable;
        break;
    case EAttainmentType::Select:
    case EAttainmentType::None:
        // Set weapon state to 'Select' when the attainment type is 'Select' or 'None'.
        WeaponMastery.WeaponStateType = EWeaponState::Select;
        break;
    default:
        // Set weapon state to 'Select' by default if the attainment type is unknown.
        WeaponMastery.WeaponStateType = EWeaponState::Select;
        break;
    }
}

/**
 * Updates the weapon rarity based on the provided rarity type.
 * This function applies rarity effects to the weapon mesh if applicable.
 * It also includes a delayed reset operation to prevent rapid changes.
 */
void UWeaponMasteryComponent::UpdateWeaponRarity(ERarityType InRarityType)
{
    // Check if weapon rarity process is already ongoing; if yes, return.
    if (bWeaponRarityProcess == true)
    {
        return;
    }
    // Update rarity data based on the provided rarity type.
    UpdateRarityData(InRarityType);

    // Check if WeaponModule is valid; if not, log an error and return.
    if (WeaponModule == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("WeaponModule is nullptr!"));
        return;
    }

    // Initialize MaterialInterface to nullptr.
    UMaterialInterface* MaterialInterface = nullptr;

    // Check if rarity effect process is not 'Apply' or if the rarity type is 'Select'.
    if (WeaponMastery.RarityEffectProcess != ERarityEffectProcess::Apply || InRarityType == ERarityType::Select)
    {
        // Clear any existing overlay material and return.
        WeaponModule->GetWeaponMesh()->SetOverlayMaterial(MaterialInterface);
        WeaponModule->GetWeaponMesh()->MarkRenderStateDirty();
        return;
    }
    else
    {
        // Get overlay material based on the provided rarity type.
        MaterialInterface = GetOverlayMaterialByRarity(InRarityType);
    }

    // If MaterialInterface is still nullptr, log an error and return.
    if (MaterialInterface == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to obtain overlay material for rarity type: %d"), static_cast<int32>(InRarityType));
        if (GEngine)
        {
            // Display an on-screen debug message for the error.
            GEngine->AddOnScreenDebugMessage
            (
                -1, 5.0f,
                FColor::Red,
                TEXT("Failed to obtain overlay material | Function: UpdateWeaponRarity[WeaponMasteryComponent.cpp]")
            );
        }
        return;
    }

    // Apply the obtained overlay material to the weapon mesh.
    if (WeaponModule->GetWeaponMesh() != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("OverlayMaterial Applied!."));
        UE_LOG(LogTemp, Warning, TEXT("MaterialInterface: %s"), *MaterialInterface->GetName());
        WeaponModule->GetWeaponMesh()->SetOverlayMaterial(MaterialInterface);

        // Force a manual update of the weapon mesh to ensure changes are applied immediately.
        WeaponModule->GetWeaponMesh()->MarkRenderStateDirty();
    }
    else
    {
        // Log an error if the weapon mesh is nullptr.
        UE_LOG(LogTemp, Error, TEXT("WeaponMesh is nullptr! Cannot apply overlay material."));
    }

    // Set the weapon rarity process flag to true to indicate ongoing process.
    bWeaponRarityProcess = true;

    // Define a lambda function for delayed operation to reset the weapon rarity process flag.
    auto DelayedReset = [this]()
    {
        FPlatformProcess::Sleep(0.5f);

        // Asynchronously reset the updating weapon rarity operation.
        AsyncTask
        (ENamedThreads::GameThread, [this]()
            {
                ResetUpdatingWeaponRarityOperation();
            }
        );
    };

    // Detach a thread to execute the delayed reset operation.
    std::thread(DelayedReset).detach();

    //TODO: This method has been temporarily used to eliminate the problem of the nested loop and to fix the issue of materials not being added. 
    // We will continue with this method until the exact source of the problem is resolved.
}

/**
 * GetOverlayMaterialByRarity Function:
 * -------------------------------------
 * This function retrieves the overlay material based on the specified rarity type.
 * It returns the corresponding material interface based on the rarity type provided as input.
 * If the rarity type is recognized and a corresponding material is found, it returns the material interface.
 * If the rarity type is not recognized or the corresponding material is not found, it logs an error or warning message accordingly.
 *
 * Parameters:
 *  - InRarityType: The rarity type for which to retrieve the overlay material.
 *
 * Return Type:
 *  - UMaterialInterface*: Pointer to the overlay material interface corresponding to the provided rarity type.
 *                        Returns nullptr if the rarity type is unrecognized or the material is not found.
 */
UMaterialInterface* UWeaponMasteryComponent::GetOverlayMaterialByRarity(ERarityType InRarityType)
{
    UMaterialInterface* MaterialInterface = nullptr;

    // Switch statement to handle different rarity types and retrieve corresponding materials.
    switch (InRarityType)
    {
    case ERarityType::Select:
        // No specific material for 'Select' rarity type. Return nullptr.
        return nullptr;
    case ERarityType::Common:
        // Retrieve the common material from the weapon effects object.
        MaterialInterface = Cast<UMaterialInterface>(WeaponEffectAndProps.GetCommonMaterial());
        break;
    case ERarityType::Rare:
        // Retrieve the rare material from the weapon effects object.
        MaterialInterface = Cast<UMaterialInterface>(WeaponEffectAndProps.GetRareMaterial());
        break;
    case ERarityType::Epic:
        // Retrieve the epic material from the weapon effects object.
        MaterialInterface = Cast<UMaterialInterface>(WeaponEffectAndProps.GetEpicMaterial());
        break;
    case ERarityType::Legendary:
        // Retrieve the legendary material from the weapon effects object.
        MaterialInterface = Cast<UMaterialInterface>(WeaponEffectAndProps.GetLegendaryMaterial());
        break;
    case ERarityType::Mystic:
        // Retrieve the mystic material from the weapon effects object.
        MaterialInterface = Cast<UMaterialInterface>(WeaponEffectAndProps.GetMysticMaterial());
        break;
    case ERarityType::Unique:
        // Retrieve the unique material from the weapon effects object.
        MaterialInterface = Cast<UMaterialInterface>(WeaponEffectAndProps.GetUniqueMaterial());
        break;
    default:
        // Log a warning message if the rarity type is not recognized.
        UE_LOG(LogTemp, Warning, TEXT("Rarity type not recognized: %d. Using default material."), static_cast<int32>(InRarityType));
        return nullptr;
    }

    // Check if the material interface pointer is valid.
    if (!MaterialInterface)
    {
        // Log an error message if failed to cast material for the specified rarity type.
        UE_LOG(LogTemp, Error, TEXT("Failed to cast material for rarity type: %d"), static_cast<int32>(InRarityType));
    }
    else
    {
        // Log a success message if successfully cast material for the specified rarity type.
        UE_LOG(LogTemp, Log, TEXT("Successful to cast material for rarity type: %d"), static_cast<int32>(InRarityType));
    }

    // Return the retrieved material interface.
    return MaterialInterface;
}

void UWeaponMasteryComponent::ResetUpdatingWeaponRarityOperation()
{
    UE_LOG(LogTemp, Warning, TEXT("ResetUpdatingWeaponRarityOperation!"));
    bWeaponRarityProcess = false;
}

/**
 * UpdateAmmunitionType Function:
 * ------------------------------
 * This function handles the update of ammunition type based on the specified long-range weapon type.
 * It switches between different long-range weapon types and assigns corresponding ammunition types to the weapon mastery.
 * After updating the ammunition type, it calls the UpdateAmmunitionTypeData function to reflect the changes.
 *
 * Parameters:
 *  - InLongRangeWeaponType: The long-range weapon type for which to update the ammunition type.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateAmmunitionType(ELongRangeWeaponType InLongRangeWeaponType)
{
    // Check if the weapon type is not long-range
    if (WeaponMastery.WeaponType != EWeaponType::LongRange)
    {
        // If the weapon type is not long-range, set the ammunition type to 'Select'
        WeaponMastery.AmmunitionType = EAmmunitionType::Select;
        // Set the ammunition type name to "Not Adjustable"
        WeaponDetails.AmmunitionTypeName = TEXT("Not Adjustable");

        // Exit the function as no further action is needed for non-long-range weapons
        return;
    }

    // Switch between different long-range weapon types
    switch (InLongRangeWeaponType)
    {
    case ELongRangeWeaponType::Pistol:
        // Assign handgun ammunition type
        WeaponMastery.AmmunitionType = EAmmunitionType::EAT_9mm;
        WeaponDetails.AmmunitionTypeName = TEXT("9mm");
        break;

    case ELongRangeWeaponType::AssaultRifle:
    case ELongRangeWeaponType::BattleRifle:
        // Assign rifle ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_556mm;
        WeaponDetails.AmmunitionTypeName    = TEXT("5.56mm");
        break;

    case ELongRangeWeaponType::Sniper:
        // Assign sniper rifle ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_762mm;
        WeaponDetails.AmmunitionTypeName    = TEXT("7.62mm");
        break;

    case ELongRangeWeaponType::Submachine:
    case ELongRangeWeaponType::PDW:
        // Assign submachine gun ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_9mm;
        WeaponDetails.AmmunitionTypeName    = TEXT("9mm");
        break;

    case ELongRangeWeaponType::LightMachine:
    case ELongRangeWeaponType::HeavyMachine:
        // Assign machine gun ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_762mm; // Example, adjust as needed
        WeaponDetails.AmmunitionTypeName    = TEXT("7.62mm");

        break;

    case ELongRangeWeaponType::Shotgun:
        // Assign shotgun ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_12Gauge;
        WeaponDetails.AmmunitionTypeName    = TEXT("12 Gauge");

        break;

    case ELongRangeWeaponType::RocketLauncher:
        // Assign rocket launcher ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_Rocket;
        WeaponDetails.AmmunitionTypeName    = TEXT("Rocket");
        break;

    case ELongRangeWeaponType::GrenadeLauncher:
        // Assign grenade launcher ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_Grenade;
        WeaponDetails.AmmunitionTypeName    = TEXT("Grenade");
        break;

    case ELongRangeWeaponType::Bow:
        // Assign bow ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_Arrow;
        WeaponDetails.AmmunitionTypeName    = TEXT("Arrow");
        break;

    case ELongRangeWeaponType::Crossbow:
        // Assign crossbow ammunition type
        WeaponMastery.AmmunitionType        = EAmmunitionType::EAT_Bolt;
        WeaponDetails.AmmunitionTypeName    = TEXT("Bolt");
        break;

    case ELongRangeWeaponType::HiddenBlade:
    case ELongRangeWeaponType::ThrowingKnife:
        // Assign hidden blade/throwing knife ammunition type (if applicable)
        // You may not need ammunition types for these
        WeaponDetails.AmmunitionTypeName    = TEXT("");
        break;

    default:
        // Handle default case if necessary
        break;
    }

    // Update the ammunition type data to reflect the changes
    UpdateAmmunitionTypeData(WeaponMastery.AmmunitionType);
}

/**
 * UpdateFireType Function:
 * ------------------------
 * This function updates the fire type of a weapon based on its long-range weapon type.
 * It maps each long-range weapon type to its corresponding fire type and updates the WeaponMastery structure accordingly.
 * After updating the fire type, it calls the UpdateFireTypeData function to ensure that the changes are reflected in the data.
 *
 * Parameters:
 *  - InLongRangeWeaponType: The long-range weapon type for which to update the fire type.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateFireType(ELongRangeWeaponType InLongRangeWeaponType)
{
    // Map each long range weapon type to its corresponding fire type
    switch (InLongRangeWeaponType)
    {
        // Handguns
        case ELongRangeWeaponType::Pistol:
            WeaponMastery.FireType = EFireType::HitScan;
            break;
        case ELongRangeWeaponType::HiddenBlade:
            WeaponMastery.FireType = EFireType::Projectile;
            break;

            // Rifles and submachine guns
        case ELongRangeWeaponType::Sniper:
        case ELongRangeWeaponType::BattleRifle:
        case ELongRangeWeaponType::Submachine:
        case ELongRangeWeaponType::PDW:
        case ELongRangeWeaponType::LightMachine:
            WeaponMastery.FireType = EFireType::HitScan;
            break;

            // Shotguns, launchers, and bows
        case ELongRangeWeaponType::AssaultRifle:
        case ELongRangeWeaponType::HeavyMachine:
        case ELongRangeWeaponType::Shotgun:
        case ELongRangeWeaponType::RocketLauncher:
        case ELongRangeWeaponType::GrenadeLauncher:
        case ELongRangeWeaponType::Bow:
        case ELongRangeWeaponType::Crossbow:
            WeaponMastery.FireType = EFireType::Projectile;
            break;

            // Throwables and hidden weapons
        case ELongRangeWeaponType::ThrowingKnife:
            WeaponMastery.FireType = EFireType::Projectile;
            break;

            // Default case for unspecified weapon types
        case ELongRangeWeaponType::Select:
        default:
            WeaponMastery.FireType = EFireType::Select;
            break;
    }
    // Update the fire type data to reflect the changes
    UpdateFireTypeData(WeaponMastery.FireType);
}

/**
 * UpdateFireMode Function:
 * ------------------------
 * This function updates the fire mode of a weapon based on its long-range weapon type.
 * It maps each long-range weapon type to its corresponding fire mode and updates the WeaponDetails and WeaponMastery structures accordingly.
 * After updating the fire mode, it calls the UpdateFireModeTypeData function to ensure that the changes are reflected in the data.
 *
 * Parameters:
 *  - InLongRangeWeaponType: The long-range weapon type for which to update the fire mode.
 *
 * Return Type: void
 */
void UWeaponMasteryComponent::UpdateFireMode(ELongRangeWeaponType InLongRangeWeaponType)
{
    switch (InLongRangeWeaponType)
    {
    // Handguns
    case ELongRangeWeaponType::Pistol:
    case ELongRangeWeaponType::HiddenBlade:
        WeaponDetails.FireMode = TEXT("Single-Shot");
        WeaponMastery.FireModeType = EFireModeType::SingleShot;
        break;

    // Rifles and submachine guns
    case ELongRangeWeaponType::Sniper:
    case ELongRangeWeaponType::BattleRifle:
        WeaponDetails.FireMode = TEXT("Single-Shot");
        WeaponMastery.FireModeType = EFireModeType::SingleShot;
        break;

     // Rifles and submachine guns
    case ELongRangeWeaponType::AssaultRifle:
    case ELongRangeWeaponType::Submachine:
    case ELongRangeWeaponType::PDW:
    case ELongRangeWeaponType::LightMachine:
        if (WeaponMastery.bFullAutomatic)
        {
            WeaponDetails.FireMode = TEXT("Full-Automatic");
            WeaponMastery.FireModeType = EFireModeType::Automatic;
        }
        else
        {
            WeaponDetails.FireMode = TEXT("Semi-Automatic");
            WeaponMastery.FireModeType = EFireModeType::SemiAutomatic;
        }
        break;
    // Heavy machine guns
    case ELongRangeWeaponType::HeavyMachine:
        WeaponDetails.FireMode = TEXT("Full-Automatic");
        WeaponMastery.FireModeType = EFireModeType::Automatic;
        break;

    // Shotguns, launchers, and bows
    case ELongRangeWeaponType::Shotgun:
    case ELongRangeWeaponType::RocketLauncher:
    case ELongRangeWeaponType::GrenadeLauncher:
    case ELongRangeWeaponType::Bow:
    case ELongRangeWeaponType::Crossbow:
        WeaponDetails.FireMode = TEXT("Single-Shot");
        WeaponMastery.FireModeType = EFireModeType::SingleShot;
        break;

    // Throwables and hidden weapons
    case ELongRangeWeaponType::ThrowingKnife:
        WeaponDetails.FireMode = TEXT("Single-Shot");
        WeaponMastery.FireModeType = EFireModeType::SingleShot;
        break;

    // Default case for unspecified weapon types
    case ELongRangeWeaponType::Select:
    default:
        WeaponDetails.FireMode = TEXT("Default");
        WeaponMastery.FireType = EFireType::Select;
        break;
    }
    // Update the fire mode type data to reflect the changes
    UpdateFireModeTypeData(WeaponMastery.FireModeType);
}

/**
 * GetWeaponAttainmentTypeAsString Function:
 * -----------------------------------------
 * This function returns a string representation of the given attainment type.
 *
 * Parameters:
 *  - InAttainmentType: The attainment type to convert to string. (EAttainmentType)
 *
 * Return Type: FString
 */

FString UWeaponMasteryComponent::GetWeaponAttainmentTypeAsString(EAttainmentType InAttainmentType)
{
    switch (InAttainmentType)
    {
    case EAttainmentType::Select:
        return FString("Selection");
    case EAttainmentType::None:
        return FString("No acquisition method");
    case EAttainmentType::Pickable:
        return FString("Can be picked up");
    case EAttainmentType::Purchasable:
        return FString("Can be purchased");
    case EAttainmentType::Lootable:
        return FString("Can be obtained as loot");
    case EAttainmentType::Craftable:
        return FString("Can be crafted");
    case EAttainmentType::Tradeable:
        return FString("Can be traded");
    case EAttainmentType::Reward:
        return FString("Can be obtained as a reward");
    case EAttainmentType::Upgradable:
        return FString("Can be upgraded");
    default:
        return FString("Unknown");
    }
}

/**
 * GetWeaponStateTypeAsString Function:
 * -------------------------------------
 * This function returns a string representation of the given weapon state.
 *
 * Parameters:
 *  - InWeaponState: The weapon state to convert to string. (EWeaponState)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponStateTypeAsString(EWeaponState InWeaponState)
{
    switch (InWeaponState)
    {
    case EWeaponState::Select:
        return FString("Select");
    case EWeaponState::Initial:
        return FString("Initial");
    case EWeaponState::Equipped:
        return FString("Equipped");
    case EWeaponState::EquippedSecondary:
        return FString("Equipped Secondary");
    case EWeaponState::Dropped:
        return FString("Dropped");
    case EWeaponState::Tradeable:
        return FString("Tradeable");
    case EWeaponState::Purchasable:
        return FString("Purchasable");
    case EWeaponState::Lootable:
        return FString("Loot-able");
    case EWeaponState::Craftable:
        return FString("Craft-able");
    case EWeaponState::Upgradable:
        return FString("Upgradable");
    case EWeaponState::Reward:
        return FString("Reward");
    case EWeaponState::Broken:
        return FString("Broken");
    case EWeaponState::InInventory:
        return FString("In Inventory");
    case EWeaponState::InStorage:
        return FString("In Storage");
    default:
        return FString("Unknown");
    }
}

/**
 * GetWeaponRarityEffectProcessAsString Function:
 * ------------------------------------------------
 * This function returns a string representation of the given rarity effect process.
 *
 * Parameters:
 *  - InRarityEffectProcess: The rarity effect process to convert to string. (ERarityEffectProcess)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponRarityEffectProcessAsString(ERarityEffectProcess InRarityEffectProcess)
{
    switch (InRarityEffectProcess)
    {
    case ERarityEffectProcess::Select:
        return FString("Select");
        break;
    case ERarityEffectProcess::Apply:
        return FString("Applied");
        break;
    case ERarityEffectProcess::Remove:
        return FString("Removed");
        break;
    }
    return FString();
}

/**
 * GetWeaponRarityTypeAsString Function:
 * --------------------------------------
 * This function returns a string representation of the given rarity type.
 *
 * Parameters:
 *  - InRarityType: The rarity type to convert to string. (ERarityType)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponRarityTypeAsString(ERarityType InRarityType)
{
    switch (InRarityType)
    {
    case ERarityType::Select:
        return TEXT("Select");
    case ERarityType::Common:
        return TEXT("Common");
    case ERarityType::Rare:
        return TEXT("Rare");
    case ERarityType::Epic:
        return TEXT("Epic");
    case ERarityType::Legendary:
        return TEXT("Legendary");
    case ERarityType::Mystic:
        return TEXT("Mystic");
    case ERarityType::Unique:
        return TEXT("Unique");
    default:
        return TEXT("Default");
    }
}

/**
 * GetWeaponTypeAsString Function:
 * --------------------------------
 * This function returns a string representation of the given weapon type.
 *
 * Parameters:
 *  - InWeaponType: The weapon type to convert to string. (EWeaponType)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponTypeAsString(EWeaponType InWeaponType)
{
    switch (InWeaponType)
    {
    case EWeaponType::Select:
        return TEXT("Select");
        break;
    case EWeaponType::Unarmed:
        return TEXT("Unarmed");
        break;
    case EWeaponType::ShortRange:
        return TEXT("Short Range Weapon");
        break;
    case EWeaponType::LongRange:
        return TEXT("Long Range Weapon");
        break;
    case EWeaponType::Throwable:
        return TEXT("Throwable");
        break;
    case EWeaponType::Explosive:
        return TEXT("Explosive");
        break;
    case EWeaponType::Heavy:
        return TEXT("Heavy Weapon");
        break;
    }
    return FString();
}


/**
 * GetLongRangeWeaponNameAsString Function:
 * ----------------------------------------
 * This function returns a string representation of the given long-range weapon type.
 *
 * Parameters:
 *  - InLongRangeWeaponType: The long-range weapon type to convert to string. (ELongRangeWeaponType)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetLongRangeWeaponNameAsString(ELongRangeWeaponType InLongRangeWeaponType)
{
        switch (InLongRangeWeaponType)
    {
        // Default type
    case ELongRangeWeaponType::Select:
        return TEXT("Select");
        break;

        // Handguns
    case ELongRangeWeaponType::Pistol:
        return TEXT("Pistol");
        break;

        // Rifles
    case ELongRangeWeaponType::AssaultRifle:
        return TEXT("Assault Rifle");
        break;
    case ELongRangeWeaponType::Sniper:
        return TEXT("Sniper Rifle");
        break;
    case ELongRangeWeaponType::BattleRifle:
        return TEXT("Battle Rifle");
        break;

        // Submachine Guns
    case ELongRangeWeaponType::Submachine:
        return TEXT("SubMachine-Gun");
        break;
    case ELongRangeWeaponType::PDW:
        return TEXT("Personal Defense Weapon");
        break;
        // Machine Guns
    case ELongRangeWeaponType::LightMachine:
        return TEXT("LightMachine-Gun");
        break;
    case ELongRangeWeaponType::HeavyMachine:
        return TEXT("HeavyMachine-Gun");
        break;

        // Shotguns
    case ELongRangeWeaponType::Shotgun:
        return TEXT("Shotgun");
        break;

        // Launchers
    case ELongRangeWeaponType::RocketLauncher:
        return TEXT("Rocket Launcher");
        break;
    case ELongRangeWeaponType::GrenadeLauncher:
        return TEXT("Grenade Launcher");
        break;

        // Bows and Crossbows
    case ELongRangeWeaponType::Bow:
        return TEXT("Bow");
        break;
    case ELongRangeWeaponType::Crossbow:
        return TEXT("Cross-Bow");
        break;

        // Throwables and Hidden Weapons
    case ELongRangeWeaponType::HiddenBlade:
        return TEXT("Hidden Blade");
        break;
    case ELongRangeWeaponType::ThrowingKnife:
        return TEXT("Throwing Knife");
        break;

        // Default case for unspecified weapon types
    default:
        return TEXT("Default");
        break;
    }   
}

/**
 * GetShortRangeWeaponNameAsString Function:
 * ----------------------------------------
 * This function returns a string representation of the given short-range weapon type.
 *
 * Parameters:
 *  - InShortRangeWeaponType: The short-range weapon type to convert to string. (EShortRangeWeaponType)
 *
 * Return Type: FString
 */

FString UWeaponMasteryComponent::GetShortRangeWeaponNameAsString(EShortRangeWeaponType InShortRangeWeaponType)
{
    switch (InShortRangeWeaponType)
    {
        // Default type
    case EShortRangeWeaponType::Select:
        return TEXT("Select");
        break;

        // Bladed Weapons
    case EShortRangeWeaponType::Sword:
        return TEXT("Sword");
        break;
    case EShortRangeWeaponType::Dagger:
        return TEXT("Dagger");
        break;

        // Blunt Weapons
    case EShortRangeWeaponType::Mace:
        return TEXT("Mace");
        break;
    case EShortRangeWeaponType::GreatHammer:
        return TEXT("Great Hammer");
        break;

        // Axes
    case EShortRangeWeaponType::Axe:
        return TEXT("Axe");
        break;
    case EShortRangeWeaponType::GreatAxe:
        return TEXT("Great Axe");
        break;

        // Polearms
    case EShortRangeWeaponType::Spear:
        return TEXT("Spear");
        break;
    case EShortRangeWeaponType::Sickle:
        return TEXT("Sickle");
        break;

        // Special Weapons
    case EShortRangeWeaponType::HiddenBlade:
        return TEXT("Hidden Blade");
        break;

        // Default case for unspecified weapon types
    default:
        return TEXT("Default");
        break;
    }
}

/**
 * GetAmmunitionTypeAsString Function:
 * -----------------------------------
 * This function returns a string representation of the given ammunition type.
 *
 * Parameters:
 *  - InAmmunitionType: The ammunition type to convert to string. (EAmmunitionType)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetAmmunitionTypeAsString(EAmmunitionType InAmmunitionType)
{
    switch (InAmmunitionType)
    {
        // Default type
    case EAmmunitionType::Select:
        return TEXT("Select");
        break;

        // Handgun Ammunition
    case EAmmunitionType::EAT_9mm:
        return TEXT("9mm");
        break;
    case EAmmunitionType::EAT_45ACP:
        return TEXT(".45 ACP");
        break;

        // Rifle Ammunition
    case EAmmunitionType::EAT_556mm:
        return TEXT("5.56mm");
        break;
    case EAmmunitionType::EAT_762mm:
        return TEXT("7.62mm");
        break;
    case EAmmunitionType::EAT_50Caliber:
        return TEXT(".50 Caliber");
        break;

        // Shotgun Ammunition
    case EAmmunitionType::EAT_12Gauge:
        return TEXT("12 Gauge");
        break;

        // Explosive Ammunition
    case EAmmunitionType::EAT_40mm:
        return TEXT("40mm Grenade");
        break;
    case EAmmunitionType::EAT_Rocket:
        return TEXT("Rocket");
        break;
    case EAmmunitionType::EAT_Grenade:
        return TEXT("Grenade");
        break;

        // Archery Ammunition
    case EAmmunitionType::EAT_Arrow:
        return TEXT("Arrow");
        break;
    case EAmmunitionType::EAT_Bolt:
        return TEXT("Bolt");
        break;

        // General Ammunition
    case EAmmunitionType::EAT_Cartridge:
        return TEXT("Cartridge");
        break;

        // Default case for unspecified ammunition types
    default:
        return TEXT("Default");
        break;
    }
}

/**
 * GetWeaponDamageTypeAsString Function:
 * -------------------------------------
 * This function returns a string representation of the given damage type.
 *
 * Parameters:
 *  - InDamageType: The damage type to convert to string. (EWeaponDamageType)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponDamageTypeAsString(EWeaponDamageType InDamageType)
{
    switch (InDamageType)
    {
    case EWeaponDamageType::Select:
        return TEXT("Select Damage Type");
        break;
    case EWeaponDamageType::SingleDamage:
        return TEXT("Single Damage Type");
        break;
    case EWeaponDamageType::RandomDamage:
        return TEXT("Random Damage Type");
        break;
    }
    return FString("Unknown");
}

/**
 * GetWeaponDamageAsString Function:
 * ---------------------------------
 * This function returns a string representation of the given damage value.
 *
 * Parameters:
 *  - InDamage: The damage value to convert to string. (int32)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponDamageAsString(int32 InDamage)
{
    return FString::FromInt(InDamage);
}

/**
 * GetWeaponRandomDamageAsString Function:
 * ---------------------------------------
 * This function returns a string representation of the given range of random damage values.
 *
 * Parameters:
 *  - InRandomDamageMin: The minimum random damage value. (int32)
 *  - InRandomDamageMax: The maximum random damage value. (int32)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponRandomDamageAsString(int32 InRandomDamageMin, int32 InRandomDamageMax)
{
    return FString::Printf(TEXT("Min: %.0f, Max: %.0f"), InRandomDamageMin, InRandomDamageMax);
}

/**
 * GetWeaponPenetrationValueAsString Function:
 * -------------------------------------------
 * This function returns a string representation of the given penetration value.
 *
 * Parameters:
 *  - InPenetrationValue: The penetration value to convert to string. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponPenetrationValueAsString(float InPenetrationValue)
{
    return FString::Printf(TEXT("%.1f"), InPenetrationValue);
}

/**
 * GetWeaponRangeValueAsString Function:
 * --------------------------------------
 * This function returns a string representation of the given range value.
 *
 * Parameters:
 *  - InRangeValue: The range value to convert to string. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponRangeValueAsString(float InRangeValue)
{
    return FString::Printf(TEXT("%.0f"), InRangeValue);
}

/**
 * GetWeaponAccuracyAsString Function:
 * ------------------------------------
 * This function returns a string representation of the given accuracy value.
 *
 * Parameters:
 *  - InAccuracy: The accuracy value to convert to string. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponAccuracyAsString(float InAccuracy)
{
    return FString::Printf(TEXT("%.0f"), InAccuracy);
}

/**
 * GetWeaponFireTypeAsString Function:
 * -----------------------------------
 * This function returns a string representation of the given fire type.
 *
 * Parameters:
 *  - InFireType: The fire type to convert to string. (EFireType)
 *
 * Return Type: FString
 */

FString UWeaponMasteryComponent::GetWeaponFireTypeAsString(EFireType InFireType)
{
    switch (InFireType)
    {
    case EFireType::Select:
        return TEXT("Select");
        break;

    case EFireType::HitScan:
        return TEXT("Hit-Scan");
        break;
    case EFireType::Projectile:
        return TEXT("Projectile");
        break;

    default:
        return TEXT("Unknow");
        break;
    }
}

/**
 * GetWeaponFullAutomaticAsString Function:
 * ----------------------------------------
 * This function returns a string representation of the firing mode (full-automatic or semi-automatic) based on the weapon type and the provided boolean indicating whether the weapon is set to full-automatic mode.
 * For certain weapon types where the firing mode cannot be adjusted (such as short-range weapons, explosives, etc.), it returns "Not Adjustable".
 * For other weapon types where the firing mode can be adjusted, it returns "Automatic" if the weapon is set to full-automatic mode, and "Semi-Automatic" otherwise.
 *
 * Parameters:
 *  - bInAutomatic: A boolean indicating whether the weapon is set to full-automatic mode. (bool)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponFullAutomaticAsString(bool bInAutomatic)
{
    EWeaponType WeaponType = WeaponMastery.WeaponType;
    
    bool bIsNotAdjustable =
        WeaponType == EWeaponType::ShortRange   ||
        WeaponType == EWeaponType::Explosive    ||
        WeaponType == EWeaponType::Select       || 
        WeaponType == EWeaponType::Throwable    || 
        WeaponType == EWeaponType::Unarmed      ;

    if (bIsNotAdjustable)
    {
        return FString("Not Adjustable");
    }
    else
    {
        if (bInAutomatic)
        {
            return TEXT("Automatic");
        }
        else
        {
            return TEXT("Semi-Automatic");
        }
    }
}

/**
 * GetWeaponFireModeAsString Function:
 * -----------------------------------
 * This function returns a string representation of the fire mode based on the provided fire mode type enumeration.
 *
 * Parameters:
 *  - InFireModeType: The fire mode type enumeration indicating the type of fire mode. (EFireModeType)
 *
 * Return Type: FString
 */

FString UWeaponMasteryComponent::GetWeaponFireModeAsString(EFireModeType InFireModeType)
{
    switch (InFireModeType)
    {
    case EFireModeType::Select:
        return TEXT("Select");
        break;
    case EFireModeType::SingleShot:
        return TEXT("Single-Shot");
        break;
    case EFireModeType::SemiAutomatic:
        return TEXT("Semi-Automatic");
        break;
    case EFireModeType::Automatic:
        return TEXT("Full-Automatic");
        break;
    }
    return FString();
}

/**
 * GetWeaponMagazineCapacityAsString Function:
 * -------------------------------------------
 * This function returns a string representation of the magazine capacity of a weapon.
 *
 * Parameters:
 *  - InMagazineCapacity: The magazine capacity of the weapon. (int32)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponMagazineCapacityAsString(int32 InMagazineCapacity)
{
    return FString::FromInt(InMagazineCapacity);
}

/**
 * GetWeaponFireRateAsString Function:
 * -----------------------------------
 * This function returns a string representation of the fire rate of a weapon.
 *
 * Parameters:
 *  - InFireRate: The fire rate of the weapon. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponFireRateAsString(float InFireRate)
{
    return FString::Printf(TEXT("%.2fs"), InFireRate);
}

/**
 * GetWeaponReloadTimeAsString Function:
 * -------------------------------------
 * This function returns a string representation of the reload time of a weapon.
 *
 * Parameters:
 *  - InReloadTime: The reload time of the weapon. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponReloadTimeAsString(float InReloadTime)
{
    return FString::Printf(TEXT("%.1f"), InReloadTime);
}

/**
 * GetWeaponAttackSpeedAsString Function:
 * --------------------------------------
 * This function returns a string representation of the attack speed of a weapon.
 *
 * Parameters:
 *  - InAttackSpeed: The attack speed of the weapon. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponAttackSpeedAsString(float InAttackSpeed)
{
    return FString::Printf(TEXT("%.0f"), InAttackSpeed);
}


/**
 * GetWeaponWeightAsString Function:
 * ---------------------------------
 * This function returns a string representation of the weight of a weapon.
 *
 * Parameters:
 *  - InWeight: The weight of the weapon. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponWeightAsString(float InWeight)
{
    return FString::Printf(TEXT("%.0f gr"), InWeight);
}

/**
 * GetWeaponDurabilityAsString Function:
 * -------------------------------------
 * This function returns a string representation of the durability of a weapon.
 *
 * Parameters:
 *  - InDurability: The durability of the weapon. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponDurabilityAsString(float InDurability)
{
    return FString::Printf(TEXT("%.0f"), InDurability);
}

/**
 * GetWeaponPriceAsString Function:
 * --------------------------------
 * This function returns a string representation of the price of a weapon.
 *
 * Parameters:
 *  - InPrice: The price of the weapon. (float)
 *
 * Return Type: FString
 */
FString UWeaponMasteryComponent::GetWeaponPriceAsString(float InPrice)
{
    return FString::Printf(TEXT("%.0f Credits"), InPrice);
}
#pragma endregion