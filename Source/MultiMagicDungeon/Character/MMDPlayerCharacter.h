#pragma once

#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MMDPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class AMMDPlayerState;

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

	// Mouse look sensitivity applied to the Mouse2D input modifier (default 0.5 to avoid
	// raw delta values feeling too fast; tune per-Blueprint subclass as needed).
	UPROPERTY(EditDefaultsOnly, Category = "MMD|Input")
	float MouseSensitivity = 0.5f;

	// Enhanced Input (created programmatically in constructor — no editor assets)
	UPROPERTY()
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY()
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	void EnhancedMove(const FInputActionValue& Value);
	void EnhancedLook(const FInputActionValue& Value);
};
