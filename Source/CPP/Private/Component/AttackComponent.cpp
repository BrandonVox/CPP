


#include "Component/AttackComponent.h"
#include "GameFramework/Character.h"
#include "DataAsset/BaseCharacterData.h"

UAttackComponent::UAttackComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
}

void UAttackComponent::RequestAttack()
{
	
	// character, attackmontage
	if(Character && BaseCharacterData)
		Character->PlayAnimMontage(BaseCharacterData->AttackMontage);
}

void UAttackComponent::SetupAttackComponent(UBaseCharacterData* BCD)
{
	BaseCharacterData = BCD;
}




