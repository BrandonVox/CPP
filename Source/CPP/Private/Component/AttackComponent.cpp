


#include "Component/AttackComponent.h"
#include "GameFramework/Character.h"
#include "DataAsset/BaseCharacterData.h"
#include "Interface/AttackInterface.h"

#include "Kismet/KismetSystemLibrary.h"

UAttackComponent::UAttackComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AttackInterface = TScriptInterface<IAttackInterface>(GetOwner());
}

void UAttackComponent::RequestAttack()
{
	if (CanAttack()) 
		Attack();
	else 
		bSavedAttack = true;
}

bool UAttackComponent::CanAttack() const
{
	if (AttackInterface == nullptr) return false;

	const bool A = bIsAttacking == false || bCanCombo == true;
	const bool B = AttackInterface->I_DoesReadyAttack();
	const bool C = AttackInterface->I_HasEnoughStamina(20.0f);

	return A && B && C;
}

void UAttackComponent::TraceHit()
{
	if (AttackInterface == nullptr) return;
	if (BaseCharacterData == nullptr) return;
	
	const FVector& StartLocation = 
		AttackInterface->I_GetSocketLocation(BaseCharacterData->TraceStart);

	const FVector& EndLocation =
		AttackInterface->I_GetSocketLocation(BaseCharacterData->TraceEnd);

	// Hit Results
	TArray<FHitResult> HitResults;

	// 
	auto DrawType = 
		BaseCharacterData->bDrawDebugTrace 
		? EDrawDebugTrace::ForDuration 
		: EDrawDebugTrace::None;

	// false -> none
	bool bDoHitSomething = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		StartLocation,
		EndLocation,
		BaseCharacterData->TraceRadius,
		BaseCharacterData->TraceObjectTypes,
		false,
		BaseCharacterData->ActorsToIgnore,
		DrawType,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		BaseCharacterData->DrawTime
	);
	// 
	if (bDoHitSomething == false) return;


	// point damage

	// 1 2 3 4
	for (const FHitResult& Result : HitResults)
	{
		if (HittedActors.Contains(Result.GetActor())) continue;

		HandleHitResult(Result);


		HittedActors.Emplace(Result.GetActor());
	}
}

void UAttackComponent::HandleHitResult(const FHitResult& Result)
{
	// Print String
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Cyan,
			Result.BoneName.ToString()
		);

	// character -> dang ky delegate
	// thong bao cho character
	// da danh trung muc tieu
	// hit result
	// Character -> Apply Point Damage
	// delegate
	if(HitSomethingDelegate.IsBound())
		HitSomethingDelegate.Execute(Result);

}

UAnimMontage* UAttackComponent::GetCorrectAttackMontage()
{
	if (BaseCharacterData == nullptr) return nullptr;
	if (BaseCharacterData->AttackMontages.IsEmpty()) return nullptr;

	return BaseCharacterData->AttackMontages[AttackIndex];
}



void UAttackComponent::Attack()
{
	if (AttackInterface && BaseCharacterData && GetCorrectAttackMontage())
	{
		AttackInterface->I_PlayAttackMontage(GetCorrectAttackMontage());
		AttackInterface->I_PlayStartAttackSound();

		bIsAttacking = true;
		bCanCombo = false;

		AttackIndex = (AttackIndex + 1) % BaseCharacterData->AttackMontages.Num();

		AttackInterface->I_HandleAttackSuccess();
	}
}

void UAttackComponent::SetupAttackComponent(UBaseCharacterData* BCD)
{
	BaseCharacterData = BCD;
}

void UAttackComponent::AN_EndAttack()
{
	bIsAttacking = false;
	bCanCombo = false;
	bSavedAttack = false;
	AttackIndex = 0;
}

void UAttackComponent::AN_Combo()
{
	// bcancombo
	bCanCombo = true;
	if (bSavedAttack)
	{
		RequestAttack();
		bSavedAttack = false;
	}
}

void UAttackComponent::SetupTraceHit()
{
	HittedActors.Empty();
}




