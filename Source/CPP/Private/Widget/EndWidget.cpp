// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/EndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UEndWidget::UpdateResultText(FText NewText)
{
	if (ResultText)
		ResultText->SetText(NewText);
}
