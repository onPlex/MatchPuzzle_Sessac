// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdateScore)
{
     //��ü�κ��� ���ο� ������ �޾Ƽ� Current Score�� UPdate 

    CurrentScore = UpdateScore;

    UpdateSocreUI();
}
