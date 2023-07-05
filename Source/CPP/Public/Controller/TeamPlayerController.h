// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "TeamPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API ATeamPlayerController : 
	public APlayerController,
	public IGenericTeamAgentInterface
{
	GENERATED_BODY()


public:
	ATeamPlayerController();

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	UPROPERTY(EditDefaultsOnly)
	FGenericTeamId TeamId = FGenericTeamId(0);
	
};
