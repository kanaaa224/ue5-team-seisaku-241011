// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/System/GameInstance_GFF2024.h"

void UGameInstance_GFF2024::Initialize()
{
	//初期値を持つ同じクラスのDefaultObjectを取得する
	const UGameInstance_GFF2024* DefaultObject = GetDefault<UGameInstance_GFF2024>();

	//初期値を設定する
	this->Life = DefaultObject->Life;
	this->Floor = DefaultObject->Floor;
}
