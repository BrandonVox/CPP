


#include "Component/AttackComponent.h"
#include "GameFramework/Character.h"
#include "DataAsset/BaseCharacterData.h"
#include "Interface/AttackInterface.h"

UAttackComponent::UAttackComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AttackInterface =  TScriptInterface<IAttackInterface>(GetOwner());
}

void UAttackComponent::RequestAttack()
{
	if (bIsAttacking) return;
	Attack();
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




