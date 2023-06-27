#pragma once

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Patrol,
	Combat,
	Regen,
	Attack
};