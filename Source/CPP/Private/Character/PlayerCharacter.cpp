// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "Component/HealthComponent.h"
#include "Widget/PlayerWidget.h"
#include "DataAsset/BaseCharacterData.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/EnhancedInputData.h"

#include "Component/AttackComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"


APlayerCharacter::APlayerCharacter()
{
	// spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->AddLocalOffset(FVector(0.0f, 0.0f, 40.0f));
	// camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*
	* Enhanced Input
	*/
	AddMapingContextForCharacter();

	// Bind Input Actions
	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent && EnhancedInputData)
	{
		EnhancedInputComponent->BindAction(EnhancedInputData->IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(EnhancedInputData->IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(EnhancedInputData->IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::AttackPressed);

		EnhancedInputComponent->BindAction(EnhancedInputData->IA_Sprint, ETriggerEvent::Started, this, &APlayerCharacter::SprintStarted);
		EnhancedInputComponent->BindAction(EnhancedInputData->IA_Sprint, ETriggerEvent::Completed, this, &APlayerCharacter::SprintCompleted);
	}

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Spawn Widget
	PlayerWidget =
		CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (PlayerWidget && HealthComponent)
	{

		PlayerWidget->AddToViewport();
		PlayerWidget->UpdateHealthBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());

		PlayerWidget->HideEnemyStats();
	}
}

void APlayerCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::HandleTakePointDamage(DamagedActor, Damage, InstigatedBy,
		HitLocation, FHitComponent, BoneName, ShotFromDirection,
		DamageType, DamageCauser);

	if (PlayerWidget && HealthComponent)
		PlayerWidget->UpdateHealthBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
}

void APlayerCharacter::HandleDead()
{
	Super::HandleDead();

	if(PlayerWidget)
		PlayerWidget->RemoveFromViewport();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	DisableInput(PlayerController);
}

void APlayerCharacter::I_SetupEnemyStats(FText NameText, float Health, float MaxHealth)
{
	if (PlayerWidget)
	{
		PlayerWidget->ShowEnemyStats();
		PlayerWidget->UpdateNameText_Enemy(NameText);
		PlayerWidget->UpdateHealthBar_Enemy(Health, MaxHealth);
	}

	if (BaseCharacterData)
	{
		ChangeMaxWalkSpeed(BaseCharacterData->FightSpeed);
		SpeedBeforeSprint = BaseCharacterData->FightSpeed;
	}
}

void APlayerCharacter::I_ExitFight()
{
	if (PlayerWidget)
		PlayerWidget->HideEnemyStats();


	ChangeMaxWalkSpeed(BaseSpeed);
	SpeedBeforeSprint = BaseSpeed;
	
}

void APlayerCharacter::I_HandleEnemyHealthUpdated(float Health, float MaxHealth)
{

	if(PlayerWidget)
		PlayerWidget->UpdateHealthBar_Enemy(Health, MaxHealth);

}

#pragma region Enhanced_Input
void APlayerCharacter::AddMapingContextForCharacter()
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

void APlayerCharacter::Look(const FInputActionValue& Value)
{

	const FVector2D LookValue = Value.Get<FVector2D>();

	// == 0
	if (LookValue.X != 0.0)
		AddControllerYawInput(LookValue.X);

	if (LookValue.Y != 0.0)
		AddControllerPitchInput(LookValue.Y);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
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

void APlayerCharacter::AttackPressed()
{
	I_RequestAttack();
}

void APlayerCharacter::SprintStarted()
{
	if (GetCharacterMovement() && BaseCharacterData)
	{
		SpeedBeforeSprint = GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = BaseCharacterData->SprintSpeed;
	}

}

void APlayerCharacter::SprintCompleted()
{
	if(GetCharacterMovement())
		GetCharacterMovement()->MaxWalkSpeed = SpeedBeforeSprint;
}

#pragma endregion