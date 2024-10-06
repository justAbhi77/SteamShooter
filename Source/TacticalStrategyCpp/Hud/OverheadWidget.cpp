// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(const FString& TextToDisplay) const
{
	if(DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(const APawn* InPawn) const
{
	if(InPawn)
	{
		const ENetRole LocalRole = InPawn->GetLocalRole();
		FString Role;
		switch(LocalRole)
		{
		case ROLE_None:
			Role = FString("None");
			break;
		case ROLE_SimulatedProxy:
			Role = FString("SimulatedProxy");
			break;
		case ROLE_AutonomousProxy:
			Role = FString("AutonomousProxy");
			break;
		case ROLE_Authority:
			Role = FString("Authority");
			break;
		case ROLE_MAX:
			Role = FString("MAX");
			break;
		default: ;
		}

		const FString LocalRoleString = FString::Printf(TEXT("Local Role %ls"), *Role);
		SetDisplayText(LocalRoleString);
	}
}

void UOverheadWidget::ShowPlayerName(const FString& TextToDisplay) const
{
	SetDisplayText(TextToDisplay);
}

void UOverheadWidget::NativeDestruct()
{
	Super::NativeDestruct();
	RemoveFromParent();
}
