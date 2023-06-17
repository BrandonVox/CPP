

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


// hit result
// delegate type
DECLARE_DYNAMIC_DELEGATE_OneParam(FHitSomethingDelegate, const FHitResult&, HitResult);

class ACharacter;
class UAnimMontage;
class UBaseCharacterData;
class IAttackInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RequestAttack();
	void TraceHit();


	

	void SetupAttackComponent(UBaseCharacterData* BCD);
	void AN_EndAttack();
	void SetupTraceHit();
protected:

	virtual void BeginPlay() override;
	
private:
	void Attack();
	void HandleHitResult(const FHitResult& Result);
public:
	FHitSomethingDelegate HitSomethingDelegate;
private:
	// luu actor co trien khai interface
	// template
	UPROPERTY()
	TScriptInterface<IAttackInterface> AttackInterface;

	UPROPERTY()
	UBaseCharacterData* BaseCharacterData;

	bool bIsAttacking = false;

	/*
	* Trace Hit
	*/
	TArray<AActor*> HittedActors;

};
