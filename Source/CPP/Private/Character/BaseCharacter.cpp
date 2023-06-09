


#include "Character/BaseCharacter.h"




#include "GameFramework/CharacterMovementComponent.h"

#include "DataAsset/BaseCharacterData.h"

#include "Component/AttackComponent.h"
#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"



ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;


	// u attack component
	// actor component
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));

	// boolean
	bUseControllerRotationYaw = false;
	// Character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Z -> Yaw
	GetCharacterMovement()->RotationRate.Yaw = 540.0;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}



void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// attack component
	if (AttackComponent)
	{
		AttackComponent->HitSomethingDelegate.BindDynamic(this, &ABaseCharacter::HandleHitSomething);
		AttackComponent->SetupAttackComponent(BaseCharacterData);
	}

	if (HealthComponent)
		HealthComponent->SetupComponent(BaseCharacterData);

	if (StaminaComponent)
		StaminaComponent->SetupComponent(BaseCharacterData);
}

void ABaseCharacter::ChangeMaxWalkSpeed(float NewSpeed)
{
	if(GetCharacterMovement())
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	// Combat Speed
	// Default Speed
	// Patrol Speed
}

void ABaseCharacter::I_ExitCombat()
{
	NotStrafe();
}

void ABaseCharacter::I_HandleTargetExitCombat()
{
	NotStrafe();
}

void ABaseCharacter::I_EnterCombat(AActor* TargetActor)
{
	AttackInterface_Target = TScriptInterface<IAttackInterface>(TargetActor);
	Strafe();
}

void ABaseCharacter::I_ReceiveCombat(AActor* TargetActor)
{
	AttackInterface_Target = TScriptInterface<IAttackInterface>(TargetActor);
	Strafe();
}

void ABaseCharacter::Strafe()
{
	if (BaseCharacterData)
		ChangeMaxWalkSpeed(BaseCharacterData->CombatSpeed);

	bUseControllerRotationYaw = true;
	if (GetCharacterMovement())
		GetCharacterMovement()->bOrientRotationToMovement = false;

	bIsStrafing = true;
}

void ABaseCharacter::NotStrafe()
{
	if (BaseCharacterData)
		ChangeMaxWalkSpeed(BaseCharacterData->DefaultSpeed);

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
		GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsStrafing = false;
}

float ABaseCharacter::I_GetHealth() const
{
	return HealthComponent ? HealthComponent->Health : 0.0f;
}

float ABaseCharacter::I_GetMaxHealth() const
{
	return HealthComponent ? HealthComponent->MaxHealth : 0.0f;
}

float ABaseCharacter::I_GetStamina() const
{
	return StaminaComponent ? StaminaComponent->Stamina : 0.0f;
}

float ABaseCharacter::I_GetMaxStamina() const
{
	return StaminaComponent ? StaminaComponent->MaxStamina : 0.0f;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakePointDamage.AddDynamic(this, &ABaseCharacter::HandleTakePointDamage);

	if (BaseCharacterData)
		ChangeMaxWalkSpeed(BaseCharacterData->DefaultSpeed);
}

#pragma region Attack_Interface



void ABaseCharacter::I_PlayAttackMontage(UAnimMontage* AttackMontage)
{
	PlayAnimMontage(AttackMontage);
}

void ABaseCharacter::I_PlayStartAttackSound()
{
	// play sound at location
	if (BaseCharacterData == nullptr) return;

	UGameplayStatics::PlaySoundAtLocation(
		this,
		BaseCharacterData->StartAttackSound,
		GetActorLocation()
	);

}

void ABaseCharacter::I_AN_EndAttack()
{
	// attack component
	// bisattacking -> false
	if(AttackComponent)
		AttackComponent->AN_EndAttack();
}

void ABaseCharacter::I_AN_Combo()
{
	if (AttackComponent)
		AttackComponent->AN_Combo();
}

FVector ABaseCharacter::I_GetSocketLocation(const FName& SocketName) const
{
	if (GetMesh() == nullptr) return FVector();

	return GetMesh()->GetSocketLocation(SocketName);
}


void ABaseCharacter::I_ANS_BeginTraceHit()
{
	if (AttackComponent)
		AttackComponent->SetupTraceHit();
}

void ABaseCharacter::I_RequestAttack()
{
	if(AttackComponent)
		AttackComponent->RequestAttack();
}

void ABaseCharacter::I_AN_EndHitReact()
{
	// khi minh bi danh xong
	// combatstate -> ready
	CombatState = ECombatState::Ready;
	I_AN_EndAttack();
}

void ABaseCharacter::I_HandleAttackSuccess()
{
	if(StaminaComponent && AttackComponent)
		StaminaComponent->UpdateStamina(AttackComponent->SuccessAttackCost);
}

bool ABaseCharacter::I_DoesReadyAttack() const
{
	return CombatState == ECombatState::Ready;
}

bool ABaseCharacter::I_HasEnoughStamina(float Cost) const
{
	if (StaminaComponent == nullptr) return false;

	return StaminaComponent->Stamina >= Cost;
}

bool ABaseCharacter::I_IsAttacking() const
{
	if (AttackComponent == nullptr) return false;

	return AttackComponent->bIsAttacking;
}

void ABaseCharacter::I_ANS_TraceHit()
{
	if (AttackComponent)
		AttackComponent->TraceHit();
}

#pragma endregion








void ABaseCharacter::HandleHitSomething(const FHitResult& HitResult)
{
	if (BaseCharacterData == nullptr) return;
	if (AttackComponent == nullptr) return;

	auto HitActor = HitResult.GetActor();

	if (HitActor == nullptr) return;

	const auto AttackDirection = UKismetMathLibrary::GetDirectionUnitVector(
		GetActorLocation(),
		HitActor->GetActorLocation()
	);

	UGameplayStatics::ApplyPointDamage(
		HitActor,
		AttackComponent->SuccessAttackDamage,
		AttackDirection,
		HitResult, 
		GetController(),
		this,
		UDamageType::StaticClass()
		);

}

void ABaseCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage,
	AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName,
	FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{

	SpawnHitImpact(HitLocation);

	if(HealthComponent)
		HealthComponent->UpdateHealthByDamage(Damage);

	if (HealthComponent->Health > 0.0f)
		HandleBeaten(ShotFromDirection);
	else
		HandleDead();
}

void ABaseCharacter::HandleBeaten(const FVector& ShotFromDirection)
{
	if (BaseCharacterData == nullptr) return;

	// Play Pain Sound
	UGameplayStatics::PlaySoundAtLocation(
		this,
		BaseCharacterData->PainSound,
		GetActorLocation()
	);

	// Hit React Montage
	PlayAnimMontage(GetCorrectHitReactMontage(ShotFromDirection));
	CombatState = ECombatState::Beaten;
}

void ABaseCharacter::HandleDead()
{
	if (BaseCharacterData == nullptr) return;

	UGameplayStatics::PlaySoundAtLocation(
		this,
		BaseCharacterData->DeadSound,
		GetActorLocation()
	);

	float DeadMontageSecond = PlayAnimMontage(BaseCharacterData->DeadMontage);

	CombatState = ECombatState::Dead;

	if (GetCharacterMovement())
		GetCharacterMovement()->StopMovementImmediately();

	if (GetCapsuleComponent() && GetMesh())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// enemy character
	// sau khi enemy character bi xoa ra khoi level
	// override function destroyed
	// enemy character -> thong bao cho player
	// de player xoa thanh mau cua enemy
	SetLifeSpan(DeadMontageSecond);
}

void ABaseCharacter::SpawnHitImpact(const FVector& HitLocation)
{
	if (BaseCharacterData == nullptr) return;

	// Spawn Hit Impact Effect
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		BaseCharacterData->HitImpactEffect,
		HitLocation
	);

	// Play Hit Impact Sound
	UGameplayStatics::PlaySoundAtLocation(
		this,
		BaseCharacterData->HitImpactSound,
		HitLocation
	);
}

UAnimMontage* ABaseCharacter::GetCorrectHitReactMontage(const FVector& AttackDirection) const
{
	if (BaseCharacterData == nullptr) return nullptr;

	const double Dot = FVector::DotProduct(AttackDirection, GetActorForwardVector());
	const bool bShouldUseDot = FMath::Abs(Dot) > 0.5;

	if (bShouldUseDot)
	{
		if (Dot > 0.0)
			return BaseCharacterData->HitReactMontage_Back;
		else
			return BaseCharacterData->HitReactMontage_Front;
	}
	else
	{
		const FVector Cross = FVector::CrossProduct(AttackDirection, GetActorForwardVector());
		if (Cross.Z > 0.0)
			return BaseCharacterData->HitReactMontage_Right;
		else
			return BaseCharacterData->HitReactMontage_Left;
	}

	return nullptr;
}