// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputData.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CPP_API UEnhancedInputData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Move;
};
