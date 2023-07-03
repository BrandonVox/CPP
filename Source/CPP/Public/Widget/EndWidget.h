// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class CPP_API UEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateResult_Text(FText NewText);

	virtual bool Initialize() override;

private:
	UFUNCTION()
	void HandlePlayAgain();

	UFUNCTION()
	void HandleQuit();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Result_Text;

	UPROPERTY(meta = (BindWidget))
	UButton* PlayAgain_Button;

	UPROPERTY(meta = (BindWidget))
	UButton* Quit_Button;

};
