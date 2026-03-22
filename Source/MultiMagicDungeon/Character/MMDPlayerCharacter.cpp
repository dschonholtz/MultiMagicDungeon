#include "Character/MMDPlayerCharacter.h"
#include "Core/MMDPlayerState.h"
#include "AbilitySystem/MMDAbilitySystemComponent.h"
#include "AbilitySystem/MMDSpellCastAbility.h"
#include "SpellSystem/MMDSpellChainComponent.h"
#include "SpellSystem/MMDSpellTags.h"
#include "SpellSystem/MMDSpellTypes.h"
#include "MMDLog.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

AMMDPlayerCharacter::AMMDPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Third-person camera rig
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Character faces movement direction, not controller yaw
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;

	// Spell chain component (local-only, no replication)
	SpellChainComp = CreateDefaultSubobject<UMMDSpellChainComponent>(TEXT("SpellChainComp"));

	// Load input assets from Content Browser
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveFinder(
		TEXT("/Game/Input/Actions/IA_Move"));
	MoveAction = MoveFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> LookFinder(
		TEXT("/Game/Input/Actions/IA_Look"));
	LookAction = LookFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpFinder(
		TEXT("/Game/Input/Actions/IA_Jump"));
	JumpAction = JumpFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> SummonFinder(
		TEXT("/Game/Input/Actions/IA_SpellSummon"));
	SpellSummonAction = SummonFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> ShapeFinder(
		TEXT("/Game/Input/Actions/IA_SpellShape"));
	SpellShapeAction = ShapeFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> LaunchFinder(
		TEXT("/Game/Input/Actions/IA_SpellLaunch"));
	SpellLaunchAction = LaunchFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> DetonateFinder(
		TEXT("/Game/Input/Actions/IA_SpellDetonate"));
	SpellDetonateAction = DetonateFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> CancelFinder(
		TEXT("/Game/Input/Actions/IA_SpellCancel"));
	SpellCancelAction = CancelFinder.Object;
}

void AMMDPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
}

void AMMDPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AMMDPlayerCharacter::InitAbilityActorInfo()
{
	AMMDPlayerState* PS = GetMMDPlayerState();
	if (!PS) return;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return;

	ASC->InitAbilityActorInfo(PS, this);
	UE_LOG(LogMMD, Log, TEXT("InitAbilityActorInfo: Owner=%s Avatar=%s"),
		*GetNameSafe(PS), *GetNameSafe(this));
}

UAbilitySystemComponent* AMMDPlayerCharacter::GetAbilitySystemComponent() const
{
	AMMDPlayerState* PS = GetMMDPlayerState();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

void AMMDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		UE_LOG(LogMMD, Error, TEXT("SIPC: EnhancedInputComponent cast failed"));
		return;
	}

	// Movement
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMMDPlayerCharacter::Move);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMMDPlayerCharacter::Look);
	EIC->BindAction(JumpAction, ETriggerEvent::Started,   this, &ACharacter::Jump);
	EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	// Spell chain building
	if (SpellSummonAction)
		EIC->BindAction(SpellSummonAction, ETriggerEvent::Started, this, &AMMDPlayerCharacter::OnSpellSummon);
	if (SpellShapeAction)
		EIC->BindAction(SpellShapeAction, ETriggerEvent::Started, this, &AMMDPlayerCharacter::OnSpellShape);
	if (SpellLaunchAction)
		EIC->BindAction(SpellLaunchAction, ETriggerEvent::Started, this, &AMMDPlayerCharacter::OnSpellLaunch);
	if (SpellDetonateAction)
		EIC->BindAction(SpellDetonateAction, ETriggerEvent::Started, this, &AMMDPlayerCharacter::OnSpellDetonate);
	if (SpellCancelAction)
		EIC->BindAction(SpellCancelAction, ETriggerEvent::Started, this, &AMMDPlayerCharacter::OnSpellCancel);
}

void AMMDPlayerCharacter::OnSpellSummon(const FInputActionValue& Value)
{
	if (!SpellChainComp) return;
	SpellChainComp->AddPrimitive(FMMDSpellPrimitive(EMMDSpellPrimitiveType::Summon, TAG_MMD_Element_Fire));
	UE_LOG(LogMMD, Log, TEXT("Spell: +SUMMON Fire (chain len=%d)"), SpellChainComp->GetCurrentChain().Primitives.Num());
}

void AMMDPlayerCharacter::OnSpellShape(const FInputActionValue& Value)
{
	if (!SpellChainComp) return;
	SpellChainComp->AddPrimitive(FMMDSpellPrimitive(EMMDSpellPrimitiveType::Shape, TAG_MMD_Shape_Sphere));
	UE_LOG(LogMMD, Log, TEXT("Spell: +SHAPE Sphere (chain len=%d)"), SpellChainComp->GetCurrentChain().Primitives.Num());
}

void AMMDPlayerCharacter::OnSpellLaunch(const FInputActionValue& Value)
{
	CastCurrentChain(Value, EMMDSpellPrimitiveType::Launch);
}

void AMMDPlayerCharacter::OnSpellDetonate(const FInputActionValue& Value)
{
	CastCurrentChain(Value, EMMDSpellPrimitiveType::Detonate);
}

void AMMDPlayerCharacter::OnSpellCancel(const FInputActionValue& Value)
{
	if (!SpellChainComp) return;
	SpellChainComp->CancelChain();
	UE_LOG(LogMMD, Log, TEXT("Spell: chain cancelled"));
}

void AMMDPlayerCharacter::CastCurrentChain(const FInputActionValue& Value, EMMDSpellPrimitiveType TerminalType)
{
	if (!SpellChainComp || !SpellChainComp->IsChainActive()) return;

	FMMDSpellChain Chain = SpellChainComp->FinalizeChain(FMMDSpellPrimitive(TerminalType));
	if (!Chain.IsValid())
	{
		UE_LOG(LogMMD, Warning, TEXT("Spell: invalid chain (len=%d)"), Chain.Primitives.Num());
		return;
	}

	// Send chain to server for execution
	Server_CastSpell(Chain.Primitives);
}

void AMMDPlayerCharacter::Server_CastSpell_Implementation(const TArray<FMMDSpellPrimitive>& Primitives)
{
	if (!SpellChainComp) return;

	FMMDSpellChain Chain;
	Chain.Primitives = Primitives;
	if (!Chain.IsValid()) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogMMD, Warning, TEXT("Server_CastSpell: no ASC"));
		return;
	}

	SpellChainComp->SetPendingCast(Chain);
	ASC->TryActivateAbilityByClass(UMMDSpellCastAbility::StaticClass());
}

void AMMDPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D V = Value.Get<FVector2D>();
	if (!Controller) return;

	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), V.Y);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), V.X);
}

void AMMDPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D V = Value.Get<FVector2D>();
	AddControllerYawInput(V.X);
	AddControllerPitchInput(-V.Y);
}

AMMDPlayerState* AMMDPlayerCharacter::GetMMDPlayerState() const
{
	return GetPlayerState<AMMDPlayerState>();
}
