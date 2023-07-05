// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/EndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"



bool UEndWidget::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	if(PlayAgainButton)
		PlayAgainButton->OnClicked.AddDynamic(this, &UEndWidget::PlayAgainClicked);

	if(QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UEndWidget::QuitClicked);

	return true;
}

void UEndWidget::UpdateResultText(FText NewText)
{
	if (ResultText)
		ResultText->SetText(NewText);
}

void UEndWidget::PlayAgainClicked()
{
	if (GetOwningPlayer() == nullptr) return;
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	FInputModeGameOnly MyInputMode;
	GetOwningPlayer()->SetInputMode(MyInputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	auto LevelString = UGameplayStatics::GetCurrentLevelName(GetWorld());
	auto LevelName = FName(*LevelString);
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UEndWidget::QuitClicked()
{
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;

	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		QuitPreference,
		true
	);
}