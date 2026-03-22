#pragma once

#include "GameFramework/HUD.h"
#include "MMDHUD.generated.h"

/**
 * Debug HUD — draws player stats and active spell chain using Canvas.
 * No UMG assets required. Replace with proper UMG widgets later.
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
	void DrawSpellChain();
};
