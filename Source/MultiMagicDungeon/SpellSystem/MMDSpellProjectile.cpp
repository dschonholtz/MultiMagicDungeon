#include "SpellSystem/MMDSpellProjectile.h"
#include "AbilitySystem/MMDAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "MMDLog.h"

AMMDSpellProjectile::AMMDSpellProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);
	InitialLifeSpan = 5.f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	RootComponent = CollisionComp;

	CollisionComp->OnComponentHit.AddDynamic(this, &AMMDSpellProjectile::OnHit);

	VFXComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComp"));
	VFXComp->SetupAttachment(CollisionComp);
	VFXComp->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireVFX(
		TEXT("/Game/SpellVisualizations/NS_MMD_Spell_Fire"));
	if (FireVFX.Succeeded())
	{
		VFXComp->SetAsset(FireVFX.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AMMDSpellProjectile::InitProjectile(const FGameplayTag& InElement, const FGameplayTag& InShape, float InPower)
{
	Element = InElement;
	Shape = InShape;
	Power = InPower;
	Damage = 20.f * Power;

	if (VFXComp)
	{
		VFXComp->Activate();
	}
}

void AMMDSpellProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMMDSpellProjectile, Element);
	DOREPLIFETIME(AMMDSpellProjectile, Shape);
	DOREPLIFETIME(AMMDSpellProjectile, Power);
}

void AMMDSpellProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority()) return;

	// Don't hit the caster
	if (OtherActor && OtherActor == GetInstigator())
	{
		return;
	}

	// Apply damage via GAS if the target has an ASC
	if (OtherActor)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
			EffectContext.AddInstigator(GetInstigator(), this);

			UGameplayEffect* DamageGE = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("SpellDamage"));
			DamageGE->DurationPolicy = EGameplayEffectDurationType::Instant;

			FGameplayModifierInfo& Mod = DamageGE->Modifiers.AddDefaulted_GetRef();
			Mod.Attribute = UMMDAttributeSet::GetHealthAttribute();
			Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-Damage));
			Mod.ModifierOp = EGameplayModOp::Additive;

			TargetASC->ApplyGameplayEffectToSelf(DamageGE, 1.f, EffectContext);
			UE_LOG(LogMMD, Log, TEXT("Projectile hit %s — applied %.0f damage"), *OtherActor->GetName(), Damage);
		}
	}

	Destroy();
}
