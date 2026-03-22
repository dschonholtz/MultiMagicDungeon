#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SpellSystem/MMDSpellTypes.h"
#include "MMDPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class AMMDPlayerState;
class UMMDSpellChainComponent;

UCLASS()
class MULTIMAGICDUNGEON_API AMMDPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMMDPlayerCharacter();
	AMMDPlayerState* GetMMDPlayerState() const;

	// IAbilitySystemInterface — delegates to PlayerState's ASC
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UMMDSpellChainComponent* GetSpellChainComponent() const { return SpellChainComp; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilityActorInfo();

	// Movement input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Spell input
	void OnSpellSummon(const FInputActionValue& Value);
	void OnSpellShape(const FInputActionValue& Value);
	void OnSpellLaunch(const FInputActionValue& Value);
	void OnSpellDetonate(const FInputActionValue& Value);
	void OnSpellCancel(const FInputActionValue& Value);

	/** Finalize chain locally, then request server to execute. */
	void CastCurrentChain(const FInputActionValue& Value, EMMDSpellPrimitiveType TerminalType);

	/** Server RPC — receives the finalized chain and activates the ability. */
	UFUNCTION(Server, Reliable)
	void Server_CastSpell(const TArray<FMMDSpellPrimitive>& Primitives);

	UPROPERTY(VisibleAnywhere, Category = "MMD|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "MMD|Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "MMD|Spells")
	TObjectPtr<UMMDSpellChainComponent> SpellChainComp;

	// Input actions — loaded from Content/Input/ assets
	UPROPERTY()
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY()
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY()
	TObjectPtr<UInputAction> SpellSummonAction;

	UPROPERTY()
	TObjectPtr<UInputAction> SpellShapeAction;

	UPROPERTY()
	TObjectPtr<UInputAction> SpellLaunchAction;

	UPROPERTY()
	TObjectPtr<UInputAction> SpellDetonateAction;

	UPROPERTY()
	TObjectPtr<UInputAction> SpellCancelAction;
};
