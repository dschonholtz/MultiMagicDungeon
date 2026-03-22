#include "AbilitySystem/MMDSpellCastAbility.h"
#include "AbilitySystem/MMDAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SpellSystem/MMDSpellChainComponent.h"
#include "SpellSystem/MMDSpellExecutor.h"
#include "SpellSystem/MMDSpellProjectile.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "MMDLog.h"

UMMDSpellCastAbility::UMMDSpellCastAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UMMDSpellCastAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if (!ASC || !AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Read chain from the Avatar's SpellChainComponent
	UMMDSpellChainComponent* ChainComp = AvatarActor->FindComponentByClass<UMMDSpellChainComponent>();
	if (!ChainComp)
	{
		UE_LOG(LogMMD, Warning, TEXT("SpellCast: no SpellChainComponent on avatar"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FMMDSpellChain& Chain = ChainComp->GetPendingCast();
	if (!Chain.IsValid())
	{
		UE_LOG(LogMMD, Warning, TEXT("SpellCast: invalid pending chain"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Check mana
	float Cost = ComputeManaCost(Chain);
	const UMMDAttributeSet* Attrs = ASC->GetSet<UMMDAttributeSet>();
	if (Attrs && Attrs->GetMana() < Cost)
	{
		UE_LOG(LogMMD, Warning, TEXT("SpellCast: not enough mana (have %.0f, need %.0f)"),
			Attrs->GetMana(), Cost);
		ChainComp->ClearPendingCast();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Build initial context from caster
	FMMDSpellContext InitCtx;
	InitCtx.Origin = AvatarActor->GetActorLocation();
	InitCtx.Direction = AvatarActor->GetActorForwardVector();

	if (APawn* Pawn = Cast<APawn>(AvatarActor))
	{
		if (AController* PC = Pawn->GetController())
		{
			InitCtx.Direction = PC->GetControlRotation().Vector();
		}
	}

	// Execute
	FMMDSpellResult Result = MMDSpellExecution::Execute(Chain, InitCtx);
	ApplyManaCost(ASC, Cost);

	switch (Result.Type)
	{
	case EMMDSpellResultType::Projectile:
		SpawnProjectile(Result, AvatarActor);
		break;
	case EMMDSpellResultType::AoE:
		ApplyAoE(Result, AvatarActor);
		break;
	default:
		UE_LOG(LogMMD, Warning, TEXT("SpellCast: no result produced"));
		break;
	}

	UE_LOG(LogMMD, Log, TEXT("SpellCast: executed (len=%d, cost=%.0f)"),
		Chain.Primitives.Num(), Cost);

	ChainComp->ClearPendingCast();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

float UMMDSpellCastAbility::ComputeManaCost(const FMMDSpellChain& Chain) const
{
	return Chain.Primitives.Num() * ManaCostPerPrimitive;
}

void UMMDSpellCastAbility::ApplyManaCost(UAbilitySystemComponent* ASC, float Cost)
{
	UGameplayEffect* CostGE = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("SpellManaCost"));
	CostGE->DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo& Mod = CostGE->Modifiers.AddDefaulted_GetRef();
	Mod.Attribute = UMMDAttributeSet::GetManaAttribute();
	Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-Cost));
	Mod.ModifierOp = EGameplayModOp::Additive;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	ASC->ApplyGameplayEffectToSelf(CostGE, 1.f, Ctx);
}

void UMMDSpellCastAbility::SpawnProjectile(const FMMDSpellResult& Result, AActor* AvatarActor)
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector SpawnLoc = Result.Origin + Result.Direction * 100.f;
	const FRotator SpawnRot = Result.Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AvatarActor;
	SpawnParams.Instigator = Cast<APawn>(AvatarActor);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMMDSpellProjectile* Proj = World->SpawnActor<AMMDSpellProjectile>(
		AMMDSpellProjectile::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);

	if (Proj)
	{
		Proj->InitProjectile(Result.Element, Result.Shape, Result.Power);
	}
}

void UMMDSpellCastAbility::ApplyAoE(const FMMDSpellResult& Result, AActor* AvatarActor)
{
	UWorld* World = GetWorld();
	if (!World) return;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Result.Radius);
	World->OverlapMultiByChannel(Overlaps, Result.Origin, FQuat::Identity, ECC_Pawn, SphereShape);

	// Debug visualization of AoE radius
	DrawDebugSphere(World, Result.Origin, Result.Radius, 16, FColor::Orange, false, 1.5f);

	float Damage = 15.f * Result.Power;

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.GetActor();
		if (!HitActor || HitActor == AvatarActor) continue;

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
		{
			UGameplayEffect* DamageGE = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("AoEDamage"));
			DamageGE->DurationPolicy = EGameplayEffectDurationType::Instant;

			FGameplayModifierInfo& Mod = DamageGE->Modifiers.AddDefaulted_GetRef();
			Mod.Attribute = UMMDAttributeSet::GetHealthAttribute();
			Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-Damage));
			Mod.ModifierOp = EGameplayModOp::Additive;

			FGameplayEffectContextHandle Ctx = TargetASC->MakeEffectContext();
			Ctx.AddInstigator(AvatarActor, AvatarActor);
			TargetASC->ApplyGameplayEffectToSelf(DamageGE, 1.f, Ctx);

			UE_LOG(LogMMD, Log, TEXT("AoE hit %s for %.0f damage"), *HitActor->GetName(), Damage);
		}
	}
}
