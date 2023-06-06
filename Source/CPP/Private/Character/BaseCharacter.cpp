


#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	// spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 400.0f;
	// FVector: x y 40.0f
	SpringArmComponent->AddLocalOffset(FVector(0.0f, 0.0f, 40.0f));
	// follow camera
	// ucameracomponent
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// boolean
	bUseControllerRotationYaw = false;
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Add Mapping Context
	// Local Player
	// PlayerController
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	// Guard
	if (PlayerController == nullptr)
		return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem 
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(PlayerController->GetLocalPlayer());

	// Input mapping context
	if (Subsystem == nullptr)
		return;

	Subsystem->AddMappingContext(InputMappingContext, 0);
}





void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced input component
	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// I A Look -> Event Function -> Bind
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	
	const FVector2D LookValue = Value.Get<FVector2D>();
	
	// == 0
	if(LookValue.X != 0.0)
		AddControllerYawInput(LookValue.X);

	if (LookValue.Y != 0.0)
		AddControllerPitchInput(LookValue.Y);
}