#include "Helper/LockHelper.h"
#include "HAL/PlatformFileManager.h"

IFileHandle* LockHelper::FileHandle = nullptr;

LockHelper::LockHelper()
{

}

LockHelper::~LockHelper()
{

}

bool LockHelper::LockFile(const FString& FilePath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (FileHandle != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("File is already locked: %s"), *FilePath);
		return false;
	}

	FileHandle = PlatformFile.OpenWrite(*FilePath, false, false);

	if (FileHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open file: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("File locked successfully: %s"), *FilePath);
	return true;
}

bool LockHelper::UnlockFile(const FString& FilePath)
{
	if (FileHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("File is not locked%s"), *FilePath);
		return false;
	}

	delete FileHandle;
	FileHandle = nullptr;

	UE_LOG(LogTemp, Log, TEXT("File unlocked successfully: %s"), *FilePath);
	return true;
}
