// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Widget/PlayerWidget.h"
#include "Widget/EndWidget.h"

#include "DataAsset/BaseCharacterData.h"
#include "DataAsset/EnhancedInputData.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"

#include "Component/AttackComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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
	EnhancedInputComponent->BindAction(EnhancedInputData->IA_StrongAttack, ETriggerEvent::Started, this, &APlayerCharacter::StrongAttackPressed);

	EnhancedInputComponent->BindAction(EnhancedInputData->IA_ExitCombat, ETriggerEvent::Started, this, &APlayerCharacter::ExitCombatPressed);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (PlayerWidget && HealthComponent)
	{
		PlayerWidget->AddToViewport();
		PlayerWidget->UpdateHealthBar_Player
			(HealthComponent->Health, HealthComponent->MaxHealth);

		PlayerWidget->UpdateStaminaBar_Player
			(StaminaComponent->Stamina, StaminaComponent->MaxStamina);

		PlayerWidget->HideEnemyStats();

		PlayerWidget->UpdateEliminations(Eliminations);

	}
	// gameplaystatics
	if (BaseCharacterData == nullptr) return;
	BackgroundAudio = UGameplayStatics::SpawnSound2D(this, BaseCharacterData->ThemeSound);

	if(BackgroundAudio)
		BackgroundAudio->SetVolumeMultiplier(BaseCharacterData->BackgroundAudioVolume);
}

void APlayerCharacter::Destroyed()
{
	ShowEndWidget();
	Super::Destroyed();
}

void APlayerCharacter::ShowEndWidget()
{
	auto PlayerController = Cast<APlayerController>(GetController());

	if (EndWidget == nullptr)
		EndWidget = CreateWidget<UEndWidget>(PlayerController, EndWidgetClass);

	if (PlayerController == nullptr) return;
	if (EndWidget == nullptr) return;

	EndWidget->AddToViewport();
	EndWidget->UpdateResultText(FText::FromString(TEXT("Lose")));

	FInputModeUIOnly MyInputMode;
	MyInputMode.SetWidgetToFocus(EndWidget->TakeWidget());

	PlayerController->SetInputMode(MyInputMode);
	PlayerController->SetShowMouseCursor(true);
}

void APlayerCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::HandleTakePointDamage(DamagedActor, Damage, InstigatedBy,
		HitLocation, FHitComponent, BoneName, ShotFromDirection,
		DamageType, DamageCauser);

	if(PlayerWidget && HealthComponent)
		PlayerWidget->UpdateHealthBar_Player
			(HealthComponent->Health, HealthComponent->MaxHealth);
}

void APlayerCharacter::HandleDead()
{
	Super::HandleDead();

	if(PlayerWidget)
		PlayerWidget->RemoveFromParent();

	auto PlayerController = Cast<APlayerController>(GetController());
	DisableInput(PlayerController);
}



void APlayerCharacter::I_ExitCombat()
{
	Super::I_ExitCombat();
	//
	if (PlayerWidget)
		PlayerWidget->HideEnemyStats();

	if (AttackInterface_Target)
		AttackInterface_Target->I_HandleTargetExitCombat();

	if (BackgroundAudio && BaseCharacterData)
		BackgroundAudio->SetSound(BaseCharacterData->ThemeSound);
}

void APlayerCharacter::I_EnterCombat(AActor* TargetActor)
{
	Super::I_EnterCombat(TargetActor);
	ShowTargetStats();

	if(BackgroundAudio && BaseCharacterData)
		BackgroundAudio->SetSound(BaseCharacterData->CombatSound);
}



void APlayerCharacter::ShowTargetStats()
{
	if (AttackInterface_Target == nullptr) return;

	if (PlayerWidget)
	{
		PlayerWidget->ShowEnemyStats();

		PlayerWidget->UpdateHealthBar_Enemy(
			AttackInterface_Target->I_GetHealth(),
			AttackInterface_Target->I_GetMaxHealth()
		);
		PlayerWidget->UpdateStaminaBar_Enemy(
			AttackInterface_Target->I_GetStamina(),
			AttackInterface_Target->I_GetMaxStamina()
		);
	}
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

void APlayerCharacter::I_HandleTargetDestroyed()
{
	Eliminations++;

	if (PlayerWidget)
	{
		PlayerWidget->HideEnemyStats();
		PlayerWidget->UpdateEliminations(Eliminations);
	}

	PlayThemeSound_Normal();
	NotStrafe();
}

void APlayerCharacter::I_HandleAttackSuccess()
{
	Super::I_HandleAttackSuccess();

	if(PlayerWidget && StaminaComponent)
		PlayerWidget->UpdateStaminaBar_Player(StaminaComponent->Stamina, StaminaComponent->MaxStamina);
}

void APlayerCharacter::I_HandleTargetAttacked(float Stamina_Target, float MaxStamina_Target)
{
	if (PlayerWidget)
		PlayerWidget->UpdateStaminaBar_Enemy(Stamina_Target, MaxStamina_Target);
}

void APlayerCharacter::I_StaminaUpdated()
{
	if (PlayerWidget && StaminaComponent)
		PlayerWidget->UpdateStaminaBar_Player(StaminaComponent->Stamina, StaminaComponent->MaxStamina);
}

void APlayerCharacter::I_StaminaUpdated_Target(float Stamina_Target, float MaxStamina_Target)
{
	if (PlayerWidget)
		PlayerWidget->UpdateStaminaBar_Enemy(Stamina_Target, MaxStamina_Target);
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
	AttackComponent->RequestAttackType = EAttackType::Normal;
	I_RequestAttack();
}

void APlayerCharacter::StrongAttackPressed()
{
	AttackComponent->RequestAttackType = EAttackType::Strong;
	I_RequestAttack();
}

void APlayerCharacter::ExitCombatPressed()
{
	I_ExitCombat();
}
void APlayerCharacter::PlayThemeSound_Normal()
{

	if (BackgroundAudio && BaseCharacterData)
		BackgroundAudio->SetSound(BaseCharacterData->ThemeSound);
}
void APlayerCharacter::PlayThemeSound_Combat()
{

	if (BackgroundAudio && BaseCharacterData)
		BackgroundAudio->SetSound(BaseCharacterData->CombatSound);
}
#pragma endregion