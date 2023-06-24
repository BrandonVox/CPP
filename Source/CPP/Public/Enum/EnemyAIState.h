#pragma once

UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Patrol,
	Fight,
	Attack
};