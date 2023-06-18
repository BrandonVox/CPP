#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AttackInterface.h"
#include "Enum/CombatState.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAttackComponent;
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	// virtual void Tick(float DeltaSeconds) override;

	// Attack Interface
	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) override;
	virtual void I_AN_EndAttack() override;
	virtual FVector I_GetSocketLocation(const FName& SocketName) const override;
	virtual void I_ANS_TraceHit() override;
	virtual void I_ANS_BeginTraceHit() override;
protected:
	virtual void BeginPlay() override;

	void AddMapingContextForCharacter();

private:
	// attack direction
	UAnimMontage* GetCorrectHitReactMontage(const FVector& AttackDirection) const;
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void AttackPressed();

	// Event Function
	UFUNCTION()
	void HandleHitSomething(const FHitResult& HitResult);

	UFUNCTION()
	void HandleTakePointDamage(AActor* DamagedActor, float Damage,
		class AController* InstigatedBy, FVector HitLocation,
		class UPrimitiveComponent* FHitComponent, FName BoneName,
		FVector ShotFromDirection, const class UDamageType* DamageType,
		AActor* DamageCauser);

private:
	// getter
	ECombatState CombatState = ECombatState::Ready;


	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UAttackComponent* AttackComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Character Data Assets")
	UEnhancedInputData* EnhancedInputData;


	UPROPERTY(EditDefaultsOnly, Category = "Character Data Assets")
	UBaseCharacterData* BaseCharacterData;

// getter setter
public:
	FORCEINLINE
	ECombatState GetCombatState() const { return CombatState; }



};
