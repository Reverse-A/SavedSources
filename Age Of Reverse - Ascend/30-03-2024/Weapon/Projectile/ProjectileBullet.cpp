// // @2023 All rights reversed by Reverse-Alpha Studios


#include "ProjectileBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Ascend/Weapon/RangedWeapon.h"
static FName NAME_ProjectileMovementComponent(TEXT("ProjectileMovementComponent"));

AProjectileBullet::AProjectileBullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(NAME_ProjectileMovementComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed	= ProjectileInitialSpeed;
	ProjectileMovementComponent->MaxSpeed		= ProjectileMaxSpeed;
	
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);

}
