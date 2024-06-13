// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "NewTypes/TeamTypes.h"
#include "TeamPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ATeamPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

protected:

private:
	UPROPERTY(EditAnywhere)
	ETeam Team;

public:
	FORCEINLINE ETeam GetTeamFromTeamPlayerStart() const { return Team; }
};
