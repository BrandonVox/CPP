


#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/EnhancedInputData.h"

#include "Component/AttackComponent.h"

#include "Kismet/KismetSystemLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
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

	// u attack component
	// actor component
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));

	// boolean
	bUseControllerRotationYaw = false;
	// Character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Z -> Yaw
	GetCharacterMovement()->RotationRate.Yaw = 540.0;
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AddMapingContextForCharacter();

	// Bind Input Actions
	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputData == nullptr)
		return;

	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Attack, ETriggerEvent::Started, this, &ABaseCharacter::AttackPressed);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// attack component
	if(AttackComponent)
		AttackComponent->SetupAttackComponent(BaseCharacterData);
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// line trace

	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = 
		StartLocation + (GetActorForwardVector() * 1000.0f);

	// Hit Results
	TArray<FHitResult> HitResults;

	HittedActors.Empty();

	bool bDoHitSomething = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		StartLocation,
		EndLocation,
		TraceRadius,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true
		);
	// 
	if (bDoHitSomething == false) return;

	
	int HitCount = 0;

	// 1 2 3 4
	for (const FHitResult& Result : HitResults)
	{
		if (HittedActors.Contains(Result.GetActor())) continue;
		
		// Print String
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Cyan,
				Result.BoneName.ToString()
			);

		// Draw Sphere
		UKismetSystemLibrary::DrawDebugSphere(
			this,
			Result.ImpactPoint,
			10.0f
		);
		// add -> emplace
		// 
		HittedActors.Emplace(Result.GetActor());

		HitCount++;
	}
	

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Red,
			FString::Printf(TEXT("Hit Count = %d"), HitCount)
		);
}

void ABaseCharacter::I_PlayAttackMontage(UAnimMontage* AttackMontage)
{
	PlayAnimMontage(AttackMontage);
}

void ABaseCharacter::I_AN_EndAttack()
{
	// attack component
	// bisattacking -> false
	if(AttackComponent)
		AttackComponent->AN_EndAttack();
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::AddMapingContextForCharacter()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController == nullptr)
		return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(PlayerController->GetLocalPlayer());


	if (Subsystem && EnhancedInputData)
		Subsystem->AddMappingContext(EnhancedInputData->InputMappingContext, 0);
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

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D ActionValue = Value.Get<FVector2D>();


	const FRotator MyControllerRotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);
	
	// forward backward
	const FVector ForwardDirection = 
		MyControllerRotation.RotateVector(FVector::ForwardVector);

	if (ActionValue.Y != 0.0)
		AddMovementInput(ForwardDirection, ActionValue.Y);


	// right left
	const FVector RightDirection =
		MyControllerRotation.RotateVector(FVector::RightVector);

	if (ActionValue.X != 0.0)
		AddMovementInput(RightDirection, ActionValue.X);
}

void ABaseCharacter::AttackPressed()
{
	if(AttackComponent)
		AttackComponent->RequestAttack();
}
