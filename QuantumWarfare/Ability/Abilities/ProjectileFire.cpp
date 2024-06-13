// @2023 All rights reversed by Reverse-Alpha Studios


#include "ProjectileFire.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include <QuantumWarfare/QWGameplayTags.h>
#include <QuantumWarfare/Actor/Projectiles/ProjectileModule.h>
#include <QuantumWarfare/Actor/Projectiles/ProjectileRocket.h>
#include <QuantumWarfare/Interaction/CombatInterface.h>

void UProjectileFire::ActivateAbility
(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileFire::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (bIsServer == false) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		if (ProjectileClass != nullptr)
		{
			AProjectileModule* Projectile = GetWorld()->SpawnActorDeferred<AProjectileModule>
			(
				ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
				
			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);

			TArray<TWeakObjectPtr<AActor>> Actors;
			Actors.Add(Projectile);
			EffectContextHandle.AddActors(Actors);

			FHitResult HitResult;
			HitResult.Location = ProjectileTargetLocation;
			EffectContextHandle.AddHitResult(HitResult);

			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

			const FQWGameplayTags GameplayTags = FQWGameplayTags::Get();

			for (auto& Pair : DamageTypes)
			{
				const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
			}

			Projectile->DamageEffectSpecHandle = SpecHandle;
			Projectile->FinishSpawning(SpawnTransform);			
		}
		//else if (ProjectileRocketClass != nullptr)
		//{
		//	AProjectileRocket* ProjectileRocket = GetWorld()->SpawnActorDeferred<AProjectileRocket>
		//		(
		//			ProjectileRocketClass,
		//			SpawnTransform,
		//			GetOwningActorFromActorInfo(),
		//			Cast<APawn>(GetOwningActorFromActorInfo()),
		//			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		//		);
		//	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		//	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		//	ProjectileRocket->DamageEffectSpecHandle = SpecHandle;
		//	ProjectileRocket->FinishSpawning(SpawnTransform);
		//}
		//else
		//{
		//	// DO NOTHING YET
		//}
	}
}