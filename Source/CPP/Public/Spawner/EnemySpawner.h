#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class CPP_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();
	// virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnEnemy();

	UFUNCTION()
	void HandleEnemyDestroyed(AActor* DestroyedActor);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrolPoints;

	FTimerHandle RespawnTimer;

	UPROPERTY(EditDefaultsOnly)
	float RespawnSecond = 3.0f;
};
