#pragma once

#include "GameFramework/Character.h"
#include "MMDPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AMMDPlayerState;

/**
 * The player's physical presence in the world.
 * Movement replicated automatically by CharacterMovementComponent.
 *
 * Does NOT store persistent data (health, mana, spells) — that lives on AMMDPlayerState.
 *
 * Input: legacy bindings via DefaultInput.ini (temporary).
 * TODO Phase 1: replace with Enhanced Input + UInputMappingContext assets.
 */
UCLASS()
class MULTIMAGICDUNGEON_API AMMDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMMDPlayerCharacter();

	AMMDPlayerState* GetMMDPlayerState() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "MMD|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "MMD|Camera")
	TObjectPtr<UCameraComponent> Camera;

	void MoveForward(float Value);
	void MoveRight(float Value);
};
