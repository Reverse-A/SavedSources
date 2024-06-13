// ©2023 Reverse-A. All rights reserved.

#pragma once

UENUM(BlueprintType)
enum class ERotateStatement : uint8
{
	ERS_Left UMETA(DisplayName = "Turning Left"),
	ERS_Right UMETA(DisplayName = "Turning Right"),
	ERS_None UMETA(DisplayName = "Turning None"),

	ERS_MAX UMETA(DisplayName = "DEFAULT MAX")
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAIS_Initialize		UMETA(DisplayName = "Initialize"),
	EAIS_Idle			UMETA(DisplayName = "Idle"),
	EAIS_Patrolling		UMETA(DisplayName = "Patrolling"),
	EAIS_AlertState		UMETA(DisplayName = "Alert"),
	EAIS_ChaseState		UMETA(DisplayName = "Chasing"),
	EAIS_AttackState	UMETA(DisplayName = "AttackState"),
	EAIS_Injured		UMETA(DisplayName = "Injured"),

	EAIS_Death UMETA(DisplayName = "Death")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Null	  UMETA(DisplayName = "Null"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_ThrowingGrenade UMETA(DisplayName = "ThrowingGrenade"),


	ECS_MAX UMETA(DisplayName = "DEFAULT MAX")
};