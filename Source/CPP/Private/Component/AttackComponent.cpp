


#include "Component/AttackComponent.h"
#include "GameFramework/Character.h"

UAttackComponent::UAttackComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UAttackComponent::RequestAttack(ACharacter* Character)
{
	// character, attackmontage
	if(Character)
		Character->PlayAnimMontage(AttackMontage);
}


void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

