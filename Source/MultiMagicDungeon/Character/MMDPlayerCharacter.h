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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "MMD|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "MMD|Camera")
	TObjectPtr<UCameraComponent> Camera;

	// Loaded from Content/Input/ assets via ConstructorHelpers
	UPROPERTY()
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY()
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	TObjectPtr<UInputAction> JumpAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
