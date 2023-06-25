
#include "Component/StaminaComponent.h"
#include "DataAsset/BaseCharacterData.h"
#include "Interface/AttackInterface.h"


UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// 1/30
	SetComponentTickInterval(0.03f);
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	AttackInterface = TScriptInterface<IAttackInterface>(GetOwner());

}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Stamina < MaxStamina && AttackInterface && AttackInterface->I_IsAttacking() == false)
	{
		Stamina = FMath::Min(Stamina + (RegenSpeed * DeltaTime), MaxStamina);
		 
		AttackInterface->I_HandleStaminaUpdated(Stamina, MaxStamina);
	}
		
}

void UStaminaComponent::SetupComponent(UBaseCharacterData* BCD)
{
	if (BCD == nullptr) return;

	Stamina = BCD->Stamina;
	MaxStamina = BCD->MaxStamina;
	RegenSpeed = BCD->RegenSpeed_Stamina;
}

void UStaminaComponent::UpdateStaminaByCost(float Cost)
{
	Stamina = FMath::Max(Stamina - Cost, 0.0f);
}

bool UStaminaComponent::HasEnoughStamina(float Cost) const
{
	return Stamina >= Cost;
}




