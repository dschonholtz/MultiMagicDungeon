#include "Character/MMDPlayerCharacter.h"
#include "Core/MMDPlayerState.h"
#include "MMDLog.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"

AMMDPlayerCharacter::AMMDPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Third-person camera rig
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Character faces movement direction, not controller yaw
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;

	// --- Create Enhanced Input actions programmatically ---
	MoveAction = NewObject<UInputAction>(this, TEXT("IA_Move"));
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	LookAction = NewObject<UInputAction>(this, TEXT("IA_Look"));
	LookAction->ValueType = EInputActionValueType::Axis2D;

	JumpAction = NewObject<UInputAction>(this, TEXT("IA_Jump"));
	JumpAction->ValueType = EInputActionValueType::Boolean;

	// --- Create mapping context and bind keys ---
	InputMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_Default"));

	// W — forward (+Y). Digital key outputs on X axis; Swizzle moves it to Y.
	{
		FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(MoveAction, EKeys::W);
		auto* Swizzle = NewObject<UInputModifierSwizzleAxis>(InputMappingContext);
		Swizzle->Order = EInputAxisSwizzle::YXZ;
		Mapping.Modifiers.Add(Swizzle);
	}

	// S — backward (-Y). Swizzle to Y, then negate.
	{
		FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(MoveAction, EKeys::S);
		auto* Swizzle = NewObject<UInputModifierSwizzleAxis>(InputMappingContext);
		Swizzle->Order = EInputAxisSwizzle::YXZ;
		Mapping.Modifiers.Add(Swizzle);
		auto* Negate = NewObject<UInputModifierNegate>(InputMappingContext);
		Mapping.Modifiers.Add(Negate);
	}

	// D — right (+X). No modifiers; digital key value is already on X.
	{
		InputMappingContext->MapKey(MoveAction, EKeys::D);
	}

	// A — left (-X). Negate the X value.
	{
		FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
		auto* Negate = NewObject<UInputModifierNegate>(InputMappingContext);
		Mapping.Modifiers.Add(Negate);
	}

	// Mouse — look (2D delta as a single axis)
	{
		InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
	}

	// Space — jump
	{
		InputMappingContext->MapKey(JumpAction, EKeys::SpaceBar);
	}
}

void AMMDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocallyControlled()) return;

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AMMDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMMDPlayerCharacter::EnhancedMove);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMMDPlayerCharacter::EnhancedLook);
	EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
}

void AMMDPlayerCharacter::EnhancedMove(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	if (!Controller) return;

	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector ForwardDir = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, MoveVector.Y);
	AddMovementInput(RightDir, MoveVector.X);
}

void AMMDPlayerCharacter::EnhancedLook(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(-LookVector.Y);
}

AMMDPlayerState* AMMDPlayerCharacter::GetMMDPlayerState() const
{
	return GetPlayerState<AMMDPlayerState>();
}
