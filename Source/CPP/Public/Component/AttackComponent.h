

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class ACharacter;
class UAnimMontage;
class UBaseCharacterData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RequestAttack();

	void SetupAttackComponent(UBaseCharacterData* BCD);
protected:

	virtual void BeginPlay() override;
	

private:
	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	UBaseCharacterData* BaseCharacterData;

};
