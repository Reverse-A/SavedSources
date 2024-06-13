// @2023 All rights reversed by Reverse-Alpha Studios


#include "ExecCalculation_Damage.h"
#include "AbilitySystemComponent.h"

#include <QuantumWarfare/QWGameplayTags.h>

#include <QuantumWarfare/Ability/AttributeSystem.h>
#include <QuantumWarfare/Ability/Data/CharacterClassInformation.h>
#include <QuantumWarfare/Ability/BlueprintLibrary/PlayerAbilitySystemLibrary.h>
#include <QuantumWarfare/Interaction/CombatInterface.h>
#include <QuantumWarfare/Ability/AbilitySystemTypes.h>

struct QWDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);

	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	/*Resistance*/
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BluntResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PierceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CutResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PenetrationResistance);


	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	QWDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, Armor,					Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, BlockChance,				Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, CriticalHitResistance,	Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, ArmorPenetration,			Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, CriticalHitChance,		Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, CriticalHitDamage,		Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, PhysicalResistance,		Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, BluntResistance,			Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, PierceResistance,			Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, CutResistance,			Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSystem, PenetrationResistance,	Target, false);

		const FQWGameplayTags& Tags = FQWGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor,					ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance,			BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance,	CriticalHitResistanceDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration,		ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance,		CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage,		CriticalHitDamageDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical,				PhysicalResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Blunt,					BluntResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Pierce,				PierceResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Cut,					CutResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Penetration,			PenetrationResistanceDef);
		
	}
};

static const QWDamageStatics& DamageStatics()
{
	static QWDamageStatics DStatics;
	return DStatics;
}

UExecCalculation_Damage::UExecCalculation_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().BluntResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PierceResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CutResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PenetrationResistanceDef);
}

void UExecCalculation_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;




#pragma region Damage
	const float Zero = 0.0f;
	const float Multiple = 2.0f;
	const float Hundred = 100.0f;

	// Get damage set by caller magnitude
	float Damage = Zero;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FQWGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf
		(
			QWDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag),
			TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalculation_Damage"),
			*ResistanceTag.ToString()
		);
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = QWDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

		float Resistance = Zero;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, Zero, Hundred);

		DamageTypeValue *= (Hundred - Resistance) / Hundred;

		Damage += DamageTypeValue;
	}

#pragma region BlockChance
	float TargetBlockChance = Zero;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, Zero);

	// Simulate chance of attack being blocked
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UPlayerAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	if (bBlocked)
	{
		float RandomBlockDamageReducer = FMath::RandRange(0.10f, 0.50f);
		// If blocked calculate the Damage result 
		Damage = (Damage * RandomBlockDamageReducer);
	}

#pragma region Armor
	const UCharacterClassInformation* CharacterClassInformation = UPlayerAbilitySystemLibrary::GetCharacterClassInformation(SourceAvatar);

	float TargetArmor = Zero;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, Zero);
	const FRealCurve* EffectiveArmorCurve = CharacterClassInformation->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(SourceCombatInterface->GetPlayerLevel());

#pragma region Armor Penetration
	float SourceArmorPenetration = Zero;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, Zero);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInformation->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	const float EffectiveArmor = TargetArmor * (Hundred - SourceArmorPenetration * ArmorPenetrationCoefficient) / Hundred;

	Damage *= (Hundred - EffectiveArmor * EffectiveArmorCoefficient) / Hundred;


#pragma region Critical-Hit
	// CriticalHitChance 
	float SourceCriticalHitChance = Zero;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, Zero);

	// CriticalHitResistance
	float TargetCriticalHitResistance = Zero;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, Zero);

	// CriticalHitDamage
	float SourceCriticalHitDamage = Zero;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, Zero);

	// Find CriticalHit resistance curve
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInformation->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	// Calculate effective Critical Hit chance after factoring in resistance
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(Zero, Hundred) < EffectiveCriticalHitChance;
	UPlayerAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	Damage = bCriticalHit ? Multiple * Damage + SourceCriticalHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData
	(
		UAttributeSystem::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		Damage
	);

	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
