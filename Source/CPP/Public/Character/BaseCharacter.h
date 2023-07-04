#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AttackInterface.h"
#include "Enum/CombatState.h"
#include "BaseCharacter.generated.h"


class UAttackComponent;

class UHealthComponent;
class UStaminaComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UEnhancedInputData;
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


	void ChangeMaxWalkSpeed(float NewSpeed);

#pragma region Attack_Interface
	virtual void I_HandleTargetExitCombat() override;
	virtual void I_EnterCombat(AActor* TargetActor) override;

	virtual float I_GetHealth() const override;
	virtual float I_GetMaxHealth() const override;

	virtual float I_GetStamina() const override;
	virtual float I_GetMaxStamina() const override;

	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) override;
	virtual void I_PlayStartAttackSound() override;

	virtual void I_AN_EndAttack() override;
	virtual void I_AN_Combo() override;
	virtual FVector I_GetSocketLocation(const FName& SocketName) const override;
	virtual void I_ANS_TraceHit() override;
	virtual void I_ANS_BeginTraceHit() override;
	virtual void I_RequestAttack() override;

	virtual void I_AN_EndHitReact() override;

	virtual void I_HandleAttackSuccess() override;

	virtual bool I_DoesReadyAttack() const override;

	virtual bool I_HasEnoughStamina(float Cost) const override;

	virtual bool I_IsAttacking() const override;

#pragma endregion



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleTakePointDamage(AActor* DamagedActor, float Damage,
			class AController* InstigatedBy, FVector HitLocation,
			class UPrimitiveComponent* FHitComponent, FName BoneName,
			FVector ShotFromDirection, const class UDamageType* DamageType,
			AActor* DamageCauser);

	virtual void HandleDead();

private:
	UAnimMontage* GetCorrectHitReactMontage(const FVector& AttackDirection) const;

	UFUNCTION()
	void HandleHitSomething(const FHitResult& HitResult);

	void SpawnHitImpact(const FVector& HitLocation);

	void HandleBeaten(const FVector& ShotFromDirection);



protected:
	UPROPERTY()
	TScriptInterface<IAttackInterface> AttackInterface_Target;

	UPROPERTY(VisibleAnywhere)
	UAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere)
	UStaminaComponent* StaminaComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Character Data Assets")
	UBaseCharacterData* BaseCharacterData;

	UPROPERTY(EditDefaultsOnly, Category = "Character Data Assets")
	UEnhancedInputData* EnhancedInputData;
private:
	// getter
	ECombatState CombatState = ECombatState::Ready;











// getter setter
public:
	FORCEINLINE
	ECombatState GetCombatState() const { return CombatState; }



};
