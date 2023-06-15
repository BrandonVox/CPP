#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UEnhancedInputData;

UCLASS()
class CPP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

	void AddMapingContextForCharacter();

private:
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Character Data Assets")
	UEnhancedInputData* EnhancedInputData;
};
