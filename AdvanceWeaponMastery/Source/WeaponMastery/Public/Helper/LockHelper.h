/**
 * Copyright (c) [2024] Reverse-Alpha-Studios
 * This plugin is designed for use with the Unreal Engine system.
 */
#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WEAPONMASTERY_API LockHelper
{
public:
	LockHelper();
	~LockHelper();

public:
	static bool LockFile(const FString& FilePath);
	static bool UnlockFile(const FString& FilePath);

private:
	static IFileHandle* FileHandle;
};
