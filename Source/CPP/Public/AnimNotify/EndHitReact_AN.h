// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EndHitReact_AN.generated.h"

/**
 * 
 */
UCLASS()
class CPP_API UEndHitReact_AN : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	
};
