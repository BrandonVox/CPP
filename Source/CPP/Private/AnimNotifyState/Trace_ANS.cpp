// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/Trace_ANS.h"
#include "Interface/AttackInterface.h"

void UTrace_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr) return;
	
	AttackInterface = TScriptInterface<IAttackInterface>(MeshComp->GetOwner());

	if(AttackInterface)
		AttackInterface->I_ANS_BeginTraceHit();
}

void UTrace_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick( MeshComp,  Animation,  FrameDeltaTime,  EventReference);
	// vung kiem
	// trace hit
	// actor -> attack interface -> trace hit
	// attack interface -> tracehit
	if(AttackInterface)
		AttackInterface->I_ANS_TraceHit();
	
}
