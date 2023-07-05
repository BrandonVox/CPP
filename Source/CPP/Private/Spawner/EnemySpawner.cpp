#include "Spawner/EnemySpawner.h"
#include "Character/EnemyCharacter.h"


AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}


void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() == nullptr) return;

	auto EnemyCharacter = GetWorld()
		->SpawnActor<AEnemyCharacter>(EnemyClass, GetActorTransform());
	
	if(EnemyCharacter)
		EnemyCharacter->PatrolPoints = PatrolPoints;
}
