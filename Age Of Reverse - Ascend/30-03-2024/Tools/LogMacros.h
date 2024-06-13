#pragma once

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#define LW(Format)						UE_LOG(LogTemp, Warning, TEXT(Format));
#define LWF(FloatValue)			UE_LOG(LogTemp, Warning, TEXT("%f"), FloatValue);
#define LW_TF(Format, ...)	UE_LOG(LogTemp, Warning, TEXT(Format), __VA_ARGS__)
#define LOG_SCREEN(Message, ...) \
    if (GEngine) \
    { \
        FString Msg = FString::Printf(TEXT(Message), ##__VA_ARGS__); \
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, Msg); \
    }

#define LINETRACE(World, StartPoint, EndPoint, Color) DrawDebugLine(World, StartPoint, EndPoint, Color);

#define LOG_TO_DESKTOP(Message) \
do { \
    FString CleanFileName = FPaths::GetCleanFilename(FString(__FILE__)); \
    FString ErrorMessage = FString::Printf(TEXT("%s %s/%s\n"), TEXT(Message), *CleanFileName, TEXT(__FUNCTION__)); \
    FString DesktopDir = "C:/Users/xerbe/Desktop"; \
    FString FilePath = FPaths::Combine(DesktopDir, TEXT("ErrorLog.txt")); \
    FFileHelper::SaveStringToFile(ErrorMessage, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append); \
} while (false)

///*The method of logging errors for later correction*/
//void DebugLogToDestop(FString InsertErrorMessage)
//{
//    FString ErrorMessage = InsertErrorMessage;
//
//    FString DesktopDir = "C:/Users/xerbe/Desktop";
//
//    FString FilePath = FPaths::Combine(DesktopDir, TEXT("DebugLog.txt"));
//
//    FFileHelper::SaveStringToFile(ErrorMessage, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//}