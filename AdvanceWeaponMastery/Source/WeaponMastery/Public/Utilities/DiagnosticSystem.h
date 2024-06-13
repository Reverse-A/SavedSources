/**
 * Copyright (c) [2024] Reverse-Alpha-Studios
 * This plugin is designed for use with the Unreal Engine system.
 */
#pragma once

#include "CoreMinimal.h"

 // Logs a warning message to the console.
 // Usage: LOG_WARNING("Your warning message here.");
#define LOG_WARNING(Message) UE_LOG(LogTemp, Warning, TEXT(Message))

// Logs an error message to the console.
// Usage: LOG_ERROR("Your error message here.");
#define LOG_ERROR(Message) UE_LOG(LogTemp, Error, TEXT(Message))

// Logs an informational message to the console.
// Usage: LOG_INFO("Your informational message here.");
#define LOG_INFO(Message) UE_LOG(LogTemp, Display, TEXT(Message))

// Logs a verbose message to the console.
// Usage: LOG_VERBOSE("Your verbose message here.");
#define LOG_VERBOSE(Message) UE_LOG(LogTemp, Verbose, TEXT(Message))

// Logs a fatal error message to the console and terminates the program.
// Usage: LOG_FATAL("Your fatal error message here.");
#define LOG_FATAL(Message) UE_LOG(LogTemp, Fatal, TEXT(Message))

// Logs a silent message to the console. This is useful for messages that should not appear unless explicitly configured to do so.
// Usage: LOG_SILENT("Your silent message here.");
#define LOG_SILENT(Message) UE_LOG(LogTemp, Log, TEXT(Message))


// This macro logs a message to the screen using GEngine with the specified color.
// Usage: LOG_GENGINE("Your message here", "ColorName");
// Example: LOG_GENGINE("Hello, world!", "Red");
// Supported colors: Red, Green, Blue, Cyan, Magenta, Yellow, White
#define LOG_GENGINE(Message, Color) \
if (GEngine) \
{ \
    FColor LogColor; \
    if (FString(Color) == "Red") \
    { \
        LogColor = FColor::Red; \
    } \
    else if (FString(Color) == "Green") \
    { \
        LogColor = FColor::Green; \
    } \
    else if (FString(Color) == "Blue") \
    { \
        LogColor = FColor::Blue; \
    } \
    else if (FString(Color) == "Cyan") \
    { \
        LogColor = FColor::Cyan; \
    } \
    else if (FString(Color) == "Magenta") \
    { \
        LogColor = FColor::Magenta; \
    } \
    else if (FString(Color) == "Yellow") \
    { \
        LogColor = FColor::Yellow; \
    } \
    else if (FString(Color) == "White") \
    { \
        LogColor = FColor::White; \
    } \
    else \
    { \
        LogColor = FColor::White; \
    } \
    GEngine->AddOnScreenDebugMessage(-1, 5.f, LogColor, FString(Message)); \
}


// Logs an invalid object message to the screen using DiagnosticSystem.
// Usage: LOG_INVALID(YourObject);
// Example: LOG_INVALID(WeaponData);
#define LOG_INVALID(InvalidObjectInput) \
if (GEngine) \
{ \
    DiagnosticSystem::LogInvalid( \
        TEXT(#InvalidObjectInput), \
        TEXT(__FUNCTION__), \
        TEXT(__FILE__), \
        __LINE__ \
    ); \
}

class WEAPONMASTERY_API DiagnosticSystem
{
public:
	// Get class name from function name
	static FString GetClassName(const FString& FunctionName);

	// Log invalid object
	static void LogInvalid(const FString& InvalidObjectName, const FString& FunctionName, const FString& FileName, int32 LineNumber);
};
