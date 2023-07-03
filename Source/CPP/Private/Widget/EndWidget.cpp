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

	if (PlayAgain_Button)
		PlayAgain_Button->OnClicked.AddDynamic(this, &UEndWidget::HandlePlayAgain);

	if (Quit_Button)
		Quit_Button->OnClicked.AddDynamic(this, &UEndWidget::HandleQuit);

	return true;
}

void UEndWidget::UpdateResult_Text(FText NewText)
{
	if(Result_Text)
		Result_Text->SetText(NewText);
}

void UEndWidget::HandlePlayAgain()
{
	if (GetOwningPlayer() == nullptr) return;

	FInputModeGameOnly MyInputMode;
	GetOwningPlayer()->SetInputMode(MyInputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	auto LevelString = UGameplayStatics::GetCurrentLevelName(GetWorld());
	auto LevelName = FName(*LevelString);
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UEndWidget::HandleQuit()
{
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;

	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		GetOwningPlayer(),
		QuitPreference,
		true
	);
}
