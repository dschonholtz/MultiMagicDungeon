#include "Character/MMDPlayerCharacter.h"
#include "Core/MMDPlayerState.h"
#include "MMDLog.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

void AMMDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMMDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Legacy input — bound to axes defined in DefaultInput.ini
	// TODO Phase 1: replace with Enhanced Input + UInputMappingContext assets
	PlayerInputComponent->BindAxis("MoveForward", this, &AMMDPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",   this, &AMMDPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",      this, &AMMDPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight",   this, &AMMDPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AMMDPlayerCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
	}
}

void AMMDPlayerCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Value);
	}
}

AMMDPlayerState* AMMDPlayerCharacter::GetMMDPlayerState() const
{
	return GetPlayerState<AMMDPlayerState>();
}
