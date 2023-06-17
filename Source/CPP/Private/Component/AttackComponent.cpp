


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
	if (bIsAttacking) return;
	Attack();
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


	bool bDoHitSomething = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		StartLocation,
		EndLocation,
		BaseCharacterData->TraceRadius,
		BaseCharacterData->TraceObjectTypes,
		false,
		BaseCharacterData->ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
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

void UAttackComponent::Attack()
{
	if (AttackInterface && BaseCharacterData)
		AttackInterface->I_PlayAttackMontage(BaseCharacterData->AttackMontage);
	bIsAttacking = true;
}

void UAttackComponent::SetupAttackComponent(UBaseCharacterData* BCD)
{
	BaseCharacterData = BCD;
}

void UAttackComponent::AN_EndAttack()
{
	bIsAttacking = false;
}

void UAttackComponent::SetupTraceHit()
{
	HittedActors.Empty();
}




