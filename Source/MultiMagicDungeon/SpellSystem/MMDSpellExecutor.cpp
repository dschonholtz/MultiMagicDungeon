#include "SpellSystem/MMDSpellExecutor.h"
#include "SpellSystem/MMDSpellTags.h"
#include "MMDLog.h"

FMMDSpellResult MMDSpellExecution::Execute(const FMMDSpellChain& Chain, const FMMDSpellContext& InitialContext)
{
	FMMDSpellResult Result;

	if (!Chain.IsValid())
	{
		UE_LOG(LogMMD, Warning, TEXT("SpellExecutor: invalid chain (len=%d)"), Chain.Primitives.Num());
		return Result;
	}

	FMMDSpellContext Ctx = InitialContext;

	for (const FMMDSpellPrimitive& Prim : Chain.Primitives)
	{
		switch (Prim.Type)
		{
		case EMMDSpellPrimitiveType::Summon:
			Ctx.Element = Prim.Parameter;
			UE_LOG(LogMMD, Log, TEXT("SpellExecutor: SUMMON %s"), *Prim.Parameter.ToString());
			break;

		case EMMDSpellPrimitiveType::Shape:
			Ctx.Shape = Prim.Parameter;
			UE_LOG(LogMMD, Log, TEXT("SpellExecutor: SHAPE %s"), *Prim.Parameter.ToString());
			break;

		case EMMDSpellPrimitiveType::Launch:
			Result.Type = EMMDSpellResultType::Projectile;
			Result.Element = Ctx.Element;
			Result.Shape = Ctx.Shape;
			Result.Origin = Ctx.Origin;
			Result.Direction = Ctx.Direction;
			Result.Power = Ctx.Power;
			UE_LOG(LogMMD, Log, TEXT("SpellExecutor: LAUNCH → Projectile"));
			break;

		case EMMDSpellPrimitiveType::Detonate:
			Result.Type = EMMDSpellResultType::AoE;
			Result.Element = Ctx.Element;
			Result.Shape = Ctx.Shape;
			Result.Origin = Ctx.Origin;
			Result.Power = Ctx.Power;
			Result.Radius = 300.f * Ctx.Power;
			UE_LOG(LogMMD, Log, TEXT("SpellExecutor: DETONATE → AoE radius=%.0f"), Result.Radius);
			break;
		}
	}

	return Result;
}
