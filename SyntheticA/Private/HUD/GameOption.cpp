// @2023 All rights reversed by Reverse-Alpha Studios


#include "HUD/GameOption.h"

#include "Characters/BaseCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameUserSettings.h"

#include "Kismet/GameplayStatics.h"

#include "MultiplayerSessionsSubsystem.h"



void UGameOption::MenuSetup()
{
	GameUserSettings = GameUserSettings == nullptr ? Cast<UGameUserSettings>(GEngine->GameUserSettings) : GameUserSettings;
	if (GameUserSettings)
	{
		if (WindowModeText)
		{
			uint8 GetWindowModeIndex = GameUserSettings->GetFullscreenMode();

			WindowMode = EWindowMode::ConvertIntToWindowMode(GetWindowModeIndex);

			FString WindowModeTextToString = UEnum::GetDisplayValueAsText(WindowMode).ToString();
			FString FormattedText = FString::Printf(TEXT("%s"), *WindowModeTextToString, LINE_TERMINATOR);
			WindowModeText->SetText(FText::FromString(FormattedText));
		}

		if (ResolutionText)
		{
			Resolution = GameUserSettings->GetScreenResolution();
			FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
			ResolutionText->SetText(FText::FromString(ResolutionString));
		}

		if (GraphicsText)
		{
			uint8 GetScalabilityLevelIndex = GameUserSettings->GetOverallScalabilityLevel();
			GraphicsIndex = GetScalabilityLevelIndex;
			FString GraphicsString = "";
			SetGraphicsText(GraphicsString);
			GraphicsText->SetText(FText::FromString(GraphicsString));
		}

		if (VsyncText)
		{
			bool bIsVSyncOn = GameUserSettings->IsVSyncEnabled();
			FString VSyncString = "";
			if (bIsVSyncOn)
			{
				VSyncString = "On";
			}
			else
			{
				VSyncString = "Off";
			}
			VsyncText->SetText(FText::FromString(VSyncString));
		}

		if (FrameText)
		{
			FrameRate = GameUserSettings->GetFrameRateLimit(); //uint32(FrameRate)

			FString FrameRateString = FString::Printf(TEXT("%d"), FrameRate);
			FrameText->SetText(FText::FromString(FrameRateString));
		}
	}
	SetIsFocusable(true);
	
/*Single Buttons*/
	if (ApplyButton && !ApplyButton->OnClicked.IsBound())
	{
		ApplyButton->OnClicked.AddDynamic(this, &UGameOption::ApplyButtonClicked);
	}

	if (BackButton && !BackButton->OnClicked.IsBound())
	{
		BackButton->OnClicked.AddDynamic(this, &UGameOption::BackButtonClicked);
	}

	if (QuitGameButton && !QuitGameButton->OnClicked.IsBound())
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UGameOption::QuitGameButtonClicked);
	}


/*Switchable Buttons*/
	if (WindowModeIncrease && !WindowModeIncrease->OnClicked.IsBound())
	{
		WindowModeIncrease->OnClicked.AddDynamic(this, &UGameOption::WindowModeIncreaseButtonClicked);
	}

	if (WindowModeDecrease && !WindowModeDecrease->OnClicked.IsBound())
	{
		WindowModeDecrease->OnClicked.AddDynamic(this, &UGameOption::WindowModeDecreaseButtonClicked);
	}

	if (ResolutionIncrease && !ResolutionIncrease->OnClicked.IsBound())
	{
		ResolutionIncrease->OnClicked.AddDynamic(this, &UGameOption::ResolutionIncreaseButtonClicked);
	}

	if (ResolutionDecrease && !ResolutionDecrease->OnClicked.IsBound())
	{
		ResolutionDecrease->OnClicked.AddDynamic(this, &UGameOption::ResolutionDecreaseButtonClicked);
	}
	if (GraphicsIncrease && !GraphicsIncrease->OnClicked.IsBound())
	{
		GraphicsIncrease->OnClicked.AddDynamic(this, &UGameOption::GraphicsIncreaseButtonClicked);
	}

	if (GraphicsDecrease && !GraphicsDecrease->OnClicked.IsBound())
	{
		GraphicsDecrease->OnClicked.AddDynamic(this, &UGameOption::GraphicsDecreaseButtonClicked);
	}

	if (VsyncIncrease && !VsyncIncrease->OnClicked.IsBound())
	{
		VsyncIncrease->OnClicked.AddDynamic(this, &UGameOption::VSyncOnButtonClicked);
	}

	if (VsyncDecrease && !VsyncDecrease->OnClicked.IsBound())
	{
		VsyncDecrease->OnClicked.AddDynamic(this, &UGameOption::VSyncOffButtonClicked);
	}

	if (FrameRateIncrease && !FrameRateIncrease->OnClicked.IsBound())
	{
		FrameRateIncrease->OnClicked.AddDynamic(this, &UGameOption::FrameRateIncreaseButtonClicked);
	}

	if (FrameRateDecrease && !FrameRateDecrease->OnClicked.IsBound())
	{
		FrameRateDecrease->OnClicked.AddDynamic(this, &UGameOption::FrameRateDecreaseButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

		if (MultiplayerSessionsSubsystem && !MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UGameOption::OnDestroySession);
		}
	}
}

void UGameOption::MenuTearDown()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

	if (ApplyButton && ApplyButton->OnClicked.IsBound())
	{
		ApplyButton->OnClicked.RemoveDynamic(this, &UGameOption::ApplyButtonClicked);
	}
	if (BackButton && BackButton->OnClicked.IsBound())
	{
		BackButton->OnClicked.RemoveDynamic(this, &UGameOption::BackButtonClicked);
	}
	if (QuitGameButton && QuitGameButton->OnClicked.IsBound())
	{
		QuitGameButton->OnClicked.RemoveDynamic(this, &UGameOption::QuitGameButtonClicked);
	}
	if (WindowModeIncrease && WindowModeIncrease->OnClicked.IsBound())
	{
		WindowModeIncrease->OnClicked.RemoveDynamic(this, &UGameOption::WindowModeIncreaseButtonClicked);
	}
	if (WindowModeDecrease && WindowModeDecrease->OnClicked.IsBound())
	{
		WindowModeDecrease->OnClicked.RemoveDynamic(this, &UGameOption::WindowModeDecreaseButtonClicked);
	}
	if (ResolutionIncrease && ResolutionIncrease->OnClicked.IsBound())
	{
		ResolutionIncrease->OnClicked.RemoveDynamic(this, &UGameOption::ResolutionIncreaseButtonClicked);
	}
	if (ResolutionDecrease && ResolutionDecrease->OnClicked.IsBound())
	{
		ResolutionDecrease->OnClicked.RemoveDynamic(this, &UGameOption::ResolutionDecreaseButtonClicked);
	}
	if (GraphicsIncrease && GraphicsIncrease->OnClicked.IsBound())
	{
		GraphicsIncrease->OnClicked.RemoveDynamic(this, &UGameOption::GraphicsIncreaseButtonClicked);
	}
	if (GraphicsDecrease && GraphicsDecrease->OnClicked.IsBound())
	{
		GraphicsDecrease->OnClicked.RemoveDynamic(this, &UGameOption::GraphicsDecreaseButtonClicked);
	}
	if (VsyncIncrease && VsyncIncrease->OnClicked.IsBound())
	{
		VsyncIncrease->OnClicked.RemoveDynamic(this, &UGameOption::VSyncOnButtonClicked);
	}
	if (VsyncDecrease && VsyncDecrease->OnClicked.IsBound())
	{
		VsyncDecrease->OnClicked.RemoveDynamic(this, &UGameOption::VSyncOffButtonClicked);
	}
	if (FrameRateIncrease && FrameRateIncrease->OnClicked.IsBound())
	{
		FrameRateIncrease->OnClicked.RemoveDynamic(this, &UGameOption::FrameRateIncreaseButtonClicked);
	}
	if (FrameRateDecrease && FrameRateDecrease->OnClicked.IsBound())
	{
		FrameRateDecrease->OnClicked.RemoveDynamic(this, &UGameOption::FrameRateDecreaseButtonClicked);
	}

	if (MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UGameOption::OnDestroySession);
	}
}


bool UGameOption::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}


void UGameOption::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		QuitGameButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();

		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;

			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UGameOption::OnPlayerLeftGame()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
	else
	{
	}
}

#pragma region Apply-Cancel-Quit Buttons
void UGameOption::ApplyButtonClicked()
{
	GameUserSettings = GameUserSettings == nullptr ? Cast<UGameUserSettings>(GEngine->GameUserSettings) : GameUserSettings;

	if (GameUserSettings)
	{
		/*WindowMode*/
		GameUserSettings->SetFullscreenMode(WindowMode);

		/*Resolution*/
		Select_ResolutionScaleByIndex();
		GameUserSettings->SetScreenResolution(Resolution);

		/*Graphics*/
		GameUserSettings->SetOverallScalabilityLevel(GraphicsIndex);

		/*V-Sync*/
		GameUserSettings->SetVSyncEnabled(bVsyncOn);

		/*FrameRate*/
		GameUserSettings->SetFrameRateLimit(FrameRate);

		GameUserSettings->ApplySettings(true);		
	}
}

void UGameOption::BackButtonClicked()
{
	if (GetWorld())
	{
		PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UGameOption::QuitGameButtonClicked()
{
	QuitGameButton->SetIsEnabled(false);
	if (GetWorld())
	{
		APlayerController* FirstPlayerController = GetWorld()->GetFirstPlayerController();
		if (FirstPlayerController)
		{
			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(FirstPlayerController->GetPawn());
			if (BaseCharacter)
			{
				BaseCharacter->Server_LeaveGame();
				BaseCharacter->OnLeftGame.AddDynamic(this, &UGameOption::OnPlayerLeftGame);
				MenuTearDown();
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
			}
			else
			{
				QuitGameButton->SetIsEnabled(true);
			}
		}
	}
}
#pragma endregion

#pragma region Window Mode 
void UGameOption::WindowModeIncreaseButtonClicked()
{
	uint8 MaxWindowModeIndex = 2;
	if (WindowModeIndex != MaxWindowModeIndex)
	{
		WindowModeIndex++;
	}
	WindowMode = EWindowMode::ConvertIntToWindowMode(WindowModeIndex);

	FString WindowModeTextToString = UEnum::GetDisplayValueAsText(WindowMode).ToString();
	FString FormattedText = FString::Printf(TEXT("%s"), *WindowModeTextToString, LINE_TERMINATOR);

	if (WindowModeText)
	{
		WindowModeText->SetText(FText::FromString(FormattedText));
	}
}

void UGameOption::WindowModeDecreaseButtonClicked()
{
	uint8 MinWindowModeIndex = 0;
	if (WindowModeIndex != MinWindowModeIndex)
	{
		WindowModeIndex--;
	}
	WindowMode = EWindowMode::ConvertIntToWindowMode(WindowModeIndex);

	FString WindowModeTextToString = UEnum::GetDisplayValueAsText(WindowMode).ToString();
	FString FormattedText = FString::Printf(TEXT("%s"), *WindowModeTextToString, LINE_TERMINATOR);

	if (WindowModeText)
	{
		WindowModeText->SetText(FText::FromString(FormattedText));
	}
}
#pragma endregion
#pragma region Resolution
void UGameOption::ResolutionIncreaseButtonClicked()
{
	uint8 MaxResolutionIndex = 4;
	if (ResolutionIndex != MaxResolutionIndex)
	{
		ResolutionIndex++;
	}
	Select_ResolutionScaleByIndex();
	if (ResolutionText)
	{
		FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionText->SetText(FText::FromString(ResolutionString));
	}
}

void UGameOption::ResolutionDecreaseButtonClicked()
{
	uint8 MinResolutionIndex = 0;
	if (ResolutionIndex != MinResolutionIndex)
	{
		ResolutionIndex--;
	}
	Select_ResolutionScaleByIndex();
	if (ResolutionText)
	{
		FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionText->SetText(FText::FromString(ResolutionString));
	}
}

void UGameOption::Select_ResolutionScaleByIndex()
{
	if (ResolutionIndex == 0)
	{
		ResolutionX = 1200;
		ResolutionY = 720;	
	}
	else if (ResolutionIndex == 1)
	{
		ResolutionX = 1600;
		ResolutionY = 900;
	}
	else if (ResolutionIndex == 2)
	{
		ResolutionX = 1920;
		ResolutionY = 1080;
	}
	else if (ResolutionIndex == 3)
	{
		ResolutionX = 2560;
		ResolutionY = 1440;
	}
	else if (ResolutionIndex == 4)
	{
		ResolutionX = 3840;
		ResolutionY = 2160;
	}
	Resolution = FIntPoint(ResolutionX, ResolutionY);
}
#pragma endregion
#pragma region Graphics
void UGameOption::GraphicsIncreaseButtonClicked()
{
	uint8 MaxGraphicsIndex = 4;
	if (GraphicsIndex != MaxGraphicsIndex)
	{
		GraphicsIndex++;
	}

	if (ResolutionText)
	{
		FString GraphicsString = "";
		SetGraphicsText(GraphicsString);
		GraphicsText->SetText(FText::FromString(GraphicsString));
	}
}

void UGameOption::GraphicsDecreaseButtonClicked()
{
	uint8 MinGraphicsIndex = 0;
	if (GraphicsIndex != MinGraphicsIndex)
	{
		GraphicsIndex--;
	}

	if (ResolutionText)
	{
		FString GraphicsString = "";
		SetGraphicsText(GraphicsString);
		GraphicsText->SetText(FText::FromString(GraphicsString));
	}
}
void UGameOption::SetGraphicsText(FString& outString)
{
	switch (GraphicsIndex)
	{
	case 0:
		outString = "Low";
		break;
	case 1:
		outString = "Medium";
		break;
	case 2:
		outString = "High";
		break;
	case 3:
		outString = "Epic";
		break;
	case 4:
		outString = "Cinematic";
		break;

	default:
		outString = "";
		break;
	}
}
#pragma endregion
#pragma region V-Sync
void UGameOption::VSyncOnButtonClicked()
{
	bVsyncOn = true;
	if (VsyncText)
	{
		VsyncText->SetText(FText::FromString(TEXT("On")));
	}
}

void UGameOption::VSyncOffButtonClicked()
{
	bVsyncOn = false;
	if (VsyncText)
	{
		VsyncText->SetText(FText::FromString(TEXT("Off")));
	}
}
#pragma endregion

#pragma region FrameRate
void UGameOption::FrameRateIncreaseButtonClicked()
{
	uint32 MaxFrameRate = 240;

	if (FrameRate != MaxFrameRate)
	{
		FrameRate += 5;
	}
	FString FrameRateString = FString::Printf(TEXT("%d"), FrameRate);
	FrameText->SetText(FText::FromString(FrameRateString));
}

void UGameOption::FrameRateDecreaseButtonClicked()
{
	uint32 MinFrameRate = 30;

	if (FrameRate != MinFrameRate)
	{
		FrameRate -= 5;
	}
	FString FrameRateString = FString::Printf(TEXT("%d"), FrameRate);
	FrameText->SetText(FText::FromString(FrameRateString));
}
#pragma endregion