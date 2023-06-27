// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/EndHitReact_AN.h"
#include "Interface/AttackInterface.h"

void UEndHitReact_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (MeshComp == nullptr) return;

    auto AttackInterface =
        TScriptInterface<IAttackInterface>(MeshComp->GetOwner());

    if (AttackInterface)
        AttackInterface->I_AN_EndHitReact();
    // true -> false 

}