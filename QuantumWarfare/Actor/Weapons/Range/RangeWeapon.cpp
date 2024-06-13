// @2023 All rights reversed by Reverse-Alpha Studios


#include "RangeWeapon.h"
#include "Kismet/GameplayStatics.h"

ARangeWeapon::ARangeWeapon()
{
	WeaponProperties.SetWeaponType(EWeaponType::LongRange);
}


// Function created for future use 
void ARangeWeapon::SpawnMuzzleFlashEmitter(FVector MuzzleSocketLocation)
{
    // Check if WeaponMesh and the MuzzleFlash are valid
    if (WeaponMesh && MuzzleFlash)
    {
        // Get the socket location from the WeaponMesh
        FVector MuzzleSocketLocation = WeaponMesh->GetSocketLocation("MuzzleSocket");

        // Spawn the emitter at the socket location
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, MuzzleSocketLocation);
    }
}