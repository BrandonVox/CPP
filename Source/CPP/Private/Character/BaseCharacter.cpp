


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
	PrimaryActorTick.bCanEverTick = true;

	// Components
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));

	bUseControllerRotationYaw = false;

	// Movement Component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = 540.0;
	
	// Ignore Camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
						   
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
		HealthComponent->SetupHealthComponent(BaseCharacterData);

	if (StaminaComponent)
		StaminaComponent->SetupComponent(BaseCharacterData);
		
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	// bind delegate nhan sat thuong
	OnTakePointDamage.AddDynamic(this, &ABaseCharacter::HandleTakePointDamage);

	// Print Health - Max Health
	if (GEngine && HealthComponent)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Red,
			FString::Printf(
				TEXT("Health = %f, MaxHealth = %f"),
				HealthComponent->GetHealth(),
				HealthComponent->GetMaxHealth()
				)
		);

	if(GetCharacterMovement())
		BaseSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

#pragma region Attack_Interface



void ABaseCharacter::I_RequestAttack()
{
	if (CombatState == ECombatState::Ready && AttackComponent)
		AttackComponent->RequestAttack();
}

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

void ABaseCharacter::I_HandleAttackSuccess()
{
	if(StaminaComponent)
		StaminaComponent->UpdateStaminaByCost(20.0f);
}

void ABaseCharacter::I_AN_EndAttack()
{
	if(AttackComponent)
		AttackComponent->AN_EndAttack();
}

void ABaseCharacter::I_AN_EndHitReact()
{
	CombatState = ECombatState::Ready;

	I_AN_EndAttack();
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

void ABaseCharacter::I_ANS_TraceHit()
{
	if (AttackComponent)
		AttackComponent->TraceHit();
}

#pragma endregion


void ABaseCharacter::HandleHitSomething(const FHitResult& HitResult)
{
	if (BaseCharacterData == nullptr) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Cyan,
			TEXT("Handle Hit Something")
		);

	// gameplay statics kismet
	// apply point damage
	// hit from direction
	// vi tri cua nguoi tan cong - vi tri cua nan nhan
	// kismet math library
	auto HitActor = HitResult.GetActor();

	if (HitActor == nullptr) return;

	const auto AttackDirection = UKismetMathLibrary::GetDirectionUnitVector(
		GetActorLocation(),
		HitActor->GetActorLocation()
	);


	UGameplayStatics::ApplyPointDamage(
		HitActor,
		BaseCharacterData->Damage,
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
	if (HealthComponent == nullptr) return;
	
	SpawnHitImpact(HitLocation);
	HealthComponent->UpdateHealthByDamage(Damage);

	if (HealthComponent->GetHealth() > 0.0f)
		HandleHitted(ShotFromDirection);
	else
		HandleDead();
}

void ABaseCharacter::HandleHitted(const FVector& ShotFromDirection)
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
	if (
		BaseCharacterData == nullptr 
		|| GetCharacterMovement() == nullptr 
		|| GetCapsuleComponent() == nullptr 
		|| GetMesh() == nullptr
		)
		return;

	UGameplayStatics::PlaySoundAtLocation(
		this,
		BaseCharacterData->DeadSound,
		GetActorLocation()
	);

	float DeadMontageSecond = 
		PlayAnimMontage(BaseCharacterData->DeadMontage);
	CombatState = ECombatState::Dead;

	GetCharacterMovement()->StopMovementImmediately();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

void ABaseCharacter::ChangeMaxWalkSpeed(float NewSpeed)
{
	if (GetCharacterMovement())
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
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