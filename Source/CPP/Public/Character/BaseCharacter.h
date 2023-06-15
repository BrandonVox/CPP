#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AttackInterface.h"
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
	// Attack Interface
	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) override;
protected:
	virtual void BeginPlay() override;

	void AddMapingContextForCharacter();

private:
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void AttackPressed();

private:
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

};
