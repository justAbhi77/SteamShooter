﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ElimAnnouncement.h"

#include "Components/TextBlock.h"

void UElimAnnouncement::SetElimAnnouncementText(const FString& AttackerName, const FString& VictimName) const
{
	FString ElimAnnouncementText = FString::Printf(TEXT("%s elimmed %s!"), *AttackerName, *VictimName);
	if(AnnouncementText)
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
}
