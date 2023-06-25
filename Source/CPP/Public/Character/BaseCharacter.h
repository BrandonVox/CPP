#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AttackInterface.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UAttackComponent;
class UHealthComponent;
class UStaminaComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UBaseCharacterData;

UCLASS()
class CPP_API ABaseCharacter : 
	public ACharacter,
	public IAttackInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	virtual void PostInitializeComponents() override;
	// virtual void Tick(float DeltaSeconds) override;

	/*
	* Attack Interface
	*/
	virtual void I_RequestAttack() override;

	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) override;
	virtual void I_PlayStartAttackSound() override;
	virtual void I_HandleAttackSuccess(float Cost) override;

	virtual void I_AN_EndAttack() override;
	virtual void I_AN_EndHitReact() override;

	virtual void I_AN_Combo() override;
	virtual FVector I_GetSocketLocation(const FName& SocketName) const override;
	virtual void I_ANS_TraceHit() override;
	virtual void I_ANS_BeginTraceHit() override;

	virtual ECombatState I_GetCombatState() const override;
	virtual bool I_HasEnoughStamina(float Cost) const override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleTakePointDamage(AActor* DamagedActor, float Damage,
		class AController* InstigatedBy, FVector HitLocation,
		class UPrimitiveComponent* FHitComponent, FName BoneName,
		FVector ShotFromDirection, const class UDamageType* DamageType,
		AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void ChangeMaxWalkSpeed(float NewSpeed);

	virtual void HandleDead();

private:
	// attack direction
	UAnimMontage* GetCorrectHitReactMontage(const FVector& AttackDirection) const;



	// Event Function
	UFUNCTION()
	void HandleHitSomething(const FHitResult& HitResult);

	void SpawnHitImpact(const FVector& HitLocation);
	void HandleHitted(const FVector& ShotFromDirection);

protected:
	UPROPERTY(VisibleAnywhere)
	UAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere)
	UStaminaComponent* StaminaComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Character Data Assets")
	UBaseCharacterData* BaseCharacterData;

	float BaseSpeed = 0.0f;





private:
	// getter
	ECombatState CombatState = ECombatState::Ready;

// getter setter
public:
	FORCEINLINE
	ECombatState GetCombatState() const { return CombatState; }


};
