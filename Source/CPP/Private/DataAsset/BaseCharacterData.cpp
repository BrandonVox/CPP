// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/BaseCharacterData.h"

UBaseCharacterData::UBaseCharacterData()
{
	CostMap.Emplace(EAttackType::Normal, 20.0f);
	CostMap.Emplace(EAttackType::Strong, 20.0f);

	DamageMap.Emplace(EAttackType::Normal, 20.0f);
	DamageMap.Emplace(EAttackType::Strong, 20.0f);
}
