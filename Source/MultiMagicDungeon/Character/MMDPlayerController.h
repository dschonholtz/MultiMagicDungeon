#pragma once

#include "GameFramework/PlayerController.h"
#include "MMDPlayerController.generated.h"

/**
 * Exists on server and owning client only.
 * Handles input routing, camera, and HUD lifecycle.
 * Phase 1+: Enhanced Input mappings wired here.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMMDPlayerController();
};
