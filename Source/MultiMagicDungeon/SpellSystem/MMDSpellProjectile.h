#pragma once

#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "MMDSpellProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

/**
 * Replicated projectile spawned by spell execution.
 * Server-authoritative: spawned on server, replicates to clients.
 * On hit, server applies damage via GameplayEffect.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDSpellProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMMDSpellProjectile();

	/** Initialize projectile with spell data. Call after spawning. */
	void InitProjectile(const FGameplayTag& InElement, const FGameplayTag& InShape, float InPower);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> VFXComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(Replicated)
	FGameplayTag Element;

	UPROPERTY(Replicated)
	FGameplayTag Shape;

	UPROPERTY(Replicated)
	float Power = 1.f;

	float Damage = 20.f;
};
