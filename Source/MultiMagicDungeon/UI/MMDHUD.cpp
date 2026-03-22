#include "UI/MMDHUD.h"
#include "Core/MMDPlayerState.h"
#include "Character/MMDPlayerCharacter.h"
#include "SpellSystem/MMDSpellChainComponent.h"
#include "SpellSystem/MMDSpellTypes.h"
#include "Engine/Canvas.h"
#include "GameFramework/GameStateBase.h"

void AMMDHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawPlayerList();
	DrawSpellChain();
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

void AMMDHUD::DrawSpellChain()
{
	APawn* Pawn = GetOwningPawn();
	AMMDPlayerCharacter* Char = Cast<AMMDPlayerCharacter>(Pawn);
	if (!Char) return;

	UMMDSpellChainComponent* ChainComp = Char->GetSpellChainComponent();
	if (!ChainComp || !ChainComp->IsChainActive()) return;

	const FMMDSpellChain& Chain = ChainComp->GetCurrentChain();

	// Build display string from chain primitives
	FString ChainText;
	for (int32 i = 0; i < Chain.Primitives.Num(); ++i)
	{
		if (i > 0) ChainText += TEXT(" > ");

		const FMMDSpellPrimitive& Prim = Chain.Primitives[i];
		switch (Prim.Type)
		{
		case EMMDSpellPrimitiveType::Summon:
			ChainText += FString::Printf(TEXT("[SUMMON:%s]"),
				*Prim.Parameter.GetTagName().ToString());
			break;
		case EMMDSpellPrimitiveType::Shape:
			ChainText += FString::Printf(TEXT("[SHAPE:%s]"),
				*Prim.Parameter.GetTagName().ToString());
			break;
		case EMMDSpellPrimitiveType::Launch:
			ChainText += TEXT("[LAUNCH]");
			break;
		case EMMDSpellPrimitiveType::Detonate:
			ChainText += TEXT("[DETONATE]");
			break;
		}
	}

	ChainText += TEXT(" > [???]");

	// Draw at bottom center
	const float TextWidth = ChainText.Len() * 8.f;
	const float X = (Canvas->SizeX - TextWidth) * 0.5f;
	const float Y = Canvas->SizeY - 80.f;

	DrawText(ChainText, FLinearColor::Yellow, X, Y);

	// Draw hint text below
	DrawText(TEXT("Q=Summon  E=Shape  LMB=Launch  RMB=Detonate  Esc=Cancel"),
		FLinearColor(0.5f, 0.5f, 0.5f), X - 50.f, Y + 22.f);
}

void AMMDHUD::DrawCrosshair()
{
	const float CX = Canvas->SizeX * 0.5f;
	const float CY = Canvas->SizeY * 0.5f;
	const float Size = 10.f;
	DrawLine(CX - Size, CY, CX + Size, CY, FLinearColor::White, 1.5f);
	DrawLine(CX, CY - Size, CX, CY + Size, FLinearColor::White, 1.5f);
}
