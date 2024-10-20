// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdateScore)
{
     //주체로부터 새로운 점수를 받아서 Current Score를 UPdate 

    CurrentScore = UpdateScore;

    UpdateSocreUI();
}
