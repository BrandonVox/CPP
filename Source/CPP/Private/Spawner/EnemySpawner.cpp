// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/EnemySpawner.h"
#include "Character/EnemyCharacter.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() == nullptr) return;

	auto EnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, GetActorTransform());

	if(EnemyCharacter)
		EnemyCharacter->PatrolPoints = PatrolPoints;
}
