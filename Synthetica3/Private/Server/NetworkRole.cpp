// ©2023 Reverse - A Company. All rights reserved.


#include "Server/NetworkRole.h"

#include "Components/TextBlock.h"

void UNetworkRole::SetDisplayText(FString TextToDisplay)
{
    if (DisplayText)
    {
        DisplayText->SetText(FText::FromString(TextToDisplay));
    }
}

void UNetworkRole::ShowPlayerNetRole(APawn* InPawn)
{
    ENetRole RemoteRole = InPawn->GetRemoteRole();

    FString Role;
    switch (RemoteRole)
    {
    case ENetRole::ROLE_Authority:
        Role = FString("Authority");
        break;
    case ENetRole::ROLE_AutonomousProxy:
        Role = FString("AutonomousProxy");
        break;
    case ENetRole::ROLE_SimulatedProxy:
        Role = FString("SimulatedProxy");
        break;
    case ENetRole::ROLE_None:
        Role = FString("ROLE_None");
        break;
    }

    FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
    SetDisplayText(RemoteRoleString);
}

void UNetworkRole::NativeDestruct()
{
    RemoveFromParent();
    Super::NativeDestruct();
}

void UNetworkRole::OpenMenu()
{

}
