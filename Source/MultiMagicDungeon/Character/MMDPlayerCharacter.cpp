#include "Character/MMDPlayerCharacter.h"
#include "Core/MMDPlayerState.h"
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

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMMDPlayerCharacter::Move);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMMDPlayerCharacter::Look);
	EIC->BindAction(JumpAction, ETriggerEvent::Started,   this, &ACharacter::Jump);
	EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	UE_LOG(LogMMD, Warning, TEXT("SIPC: Class=%s Move=%s Bindings=%d"),
		*GetClass()->GetName(), *GetNameSafe(MoveAction),
		EIC->GetActionEventBindings().Num());
}

void AMMDPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D V = Value.Get<FVector2D>();
	UE_LOG(LogMMD, Warning, TEXT("Move: X=%.2f Y=%.2f"), V.X, V.Y);
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
