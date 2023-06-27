// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Widget/PlayerWidget.h"

#include "DataAsset/BaseCharacterData.h"
#include "DataAsset/EnhancedInputData.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Component/HealthComponent.h"
#include "Component/AttackComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// Spring Arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->AddLocalOffset(FVector(0.0f, 0.0f, 40.0f));

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AddMapingContextForCharacter();

	// Bind Input Actions
	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputData == nullptr)
		return;

	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::AttackPressed);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (PlayerWidget && HealthComponent)
	{
		PlayerWidget->AddToViewport();
		PlayerWidget->
			UpdateHealthBar_Player(HealthComponent->Health, HealthComponent->MaxHealth);
		PlayerWidget->HideEnemyStats();

	}
		
}



void APlayerCharacter::I_EnterCombat(float Health_Enemy, float MaxHealth_Enemy)
{
	if (PlayerWidget)
	{
		PlayerWidget->ShowEnemyStats();
		PlayerWidget->UpdateHealthBar_Enemy(Health_Enemy, MaxHealth_Enemy);
	}

	if (BaseCharacterData)
		ChangeMaxWalkSpeed(BaseCharacterData->CombatSpeed);
		
}

void APlayerCharacter::I_HitTarget(float Health_Target, float MaxHealth_Target)
{
	// danh trung target
	// cap nhat mau
	// mau cua enemy
	if (PlayerWidget)
	{
		PlayerWidget->UpdateHealthBar_Enemy(Health_Target, MaxHealth_Target);
	}
}


#pragma region Input

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
#pragma endregion

