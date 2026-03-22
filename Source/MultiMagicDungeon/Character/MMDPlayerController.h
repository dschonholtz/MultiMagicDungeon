#pragma once

#include "GameFramework/PlayerController.h"
#include "MMDPlayerController.generated.h"

class UInputMappingContext;

/**
 * Registers the default Enhanced Input mapping context on the local player
 * and sets game-only input mode so keyboard events reach the pawn.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMMDPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
};
