// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockOnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULockOnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE5_GFF2024_API ILockOnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	//ロックオンの有効フラグを設定する
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void SetLockOnEnable(bool LockOnFlg);

};
