#include "UI/MMDHUD.h"
#include "Core/MMDPlayerState.h"
#include "Engine/Canvas.h"
#include "GameFramework/GameStateBase.h"

void AMMDHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawPlayerList();
	DrawCrosshair();
}

void AMMDHUD::DrawPlayerList()
{
	AGameStateBase* GS = GetWorld()->GetGameState();
	if (!GS) return;

	const float StartX = 20.f;
	float Y = 20.f;
	const float LineHeight = 22.f;

	DrawText(TEXT("=== Players ==="), FLinearColor::White, StartX, Y);
	Y += LineHeight;

	for (APlayerState* PS : GS->PlayerArray)
	{
		AMMDPlayerState* MMDPS = Cast<AMMDPlayerState>(PS);
		if (!MMDPS) continue;

		const FString Line = FString::Printf(
			TEXT("%s  HP:%.0f  MP:%.0f"),
			*MMDPS->GetPlayerName(),
			MMDPS->GetHealth(),
			MMDPS->GetMana()
		);

		// Local player in green, others in white
		const bool bIsLocal = (PS == GetOwningPlayerController()->PlayerState);
		DrawText(Line, bIsLocal ? FLinearColor::Green : FLinearColor::White, StartX, Y);
		Y += LineHeight;
	}
}

void AMMDHUD::DrawCrosshair()
{
	const float CX = Canvas->SizeX * 0.5f;
	const float CY = Canvas->SizeY * 0.5f;
	const float Size = 10.f;
	DrawLine(CX - Size, CY, CX + Size, CY, FLinearColor::White, 1.5f);
	DrawLine(CX, CY - Size, CX, CY + Size, FLinearColor::White, 1.5f);
}
