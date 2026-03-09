#pragma once

#include "GameFramework/HUD.h"
#include "MMDHUD.generated.h"

/**
 * Debug HUD — draws all connected players' name, health, and mana using Canvas.
 * No UMG assets required. Replace with proper UMG widgets in Phase 1.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	void DrawPlayerList();
	void DrawCrosshair();
};
