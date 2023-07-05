#include "Spawner/EnemySpawner.h"
#include "Character/EnemyCharacter.h"


AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}


void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{
	if (GetWorld() == nullptr) return;

	auto EnemyCharacter = GetWorld()
		->SpawnActor<AEnemyCharacter>(EnemyClass, GetActorTransform());

	if (EnemyCharacter)
	{
		EnemyCharacter->PatrolPoints = PatrolPoints;
		EnemyCharacter->OnDestroyed.AddDynamic(this, &AEnemySpawner::HandleEnemyDestroyed);
	}

}

void AEnemySpawner::HandleEnemyDestroyed(AActor* DestroyedActor)
{
	if (GetWorldTimerManager().IsTimerActive(RespawnTimer))
		GetWorldTimerManager().ClearTimer(RespawnTimer);

	GetWorldTimerManager()
		.SetTimer(
			RespawnTimer,
			this,
			&AEnemySpawner::SpawnEnemy,
			RespawnSecond
		);
}
