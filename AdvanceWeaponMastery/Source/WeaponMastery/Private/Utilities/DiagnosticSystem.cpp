#include "Utilities/DiagnosticSystem.h"
#include "Engine/Engine.h"

FString DiagnosticSystem::GetClassName(const FString& FunctionName)
{
    int32 DoubleColonIndex;
    if (FunctionName.FindLastChar(TEXT(':'), DoubleColonIndex))
    {
        return FunctionName.Left(DoubleColonIndex - 1); // Extract class name 
    }
    return TEXT("UnknownClass");
}

void DiagnosticSystem::LogInvalid(const FString& InvalidObjectName, const FString& FunctionName, const FString& FileName, int32 LineNumber)
{
    if (GEngine)
    {
        FString ErrorMessage = FString::Printf(
            TEXT("%s is not valid in %s class. Please Check! Function: %s, File: %s, Line: %d"),
            *InvalidObjectName,
            *GetClassName(FunctionName),
            *FunctionName,
            *FileName,
            LineNumber
        );
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMessage);
    }
}