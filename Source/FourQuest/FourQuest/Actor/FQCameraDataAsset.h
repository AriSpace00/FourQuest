// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQCameraDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API UFQCameraDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFQCameraDataAsset();

	UPROPERTY(EditAnywhere, Category = FQDebug, meta = (ToolTip = "디버그 렌더링"))
	uint8 mbIsDebugRendering : 1;

	UPROPERTY(EditAnywhere, Category = FQCameraMove, meta = (ToolTip = "카메라 이동 속도"))
	float mCameraBlendValue;

	UPROPERTY(EditAnywhere, Category = FQCameraZoom, meta = (ToolTip = "최대 줌인 카메라 거리"))
	float mCameraZoomMin;
	UPROPERTY(EditAnywhere, Category = FQCameraZoom, meta = (ToolTip = "최대 줌아웃 카메라 거리"))
	float mCameraZoomMax;
	UPROPERTY(EditAnywhere, Category = FQCameraZoom, meta = (ToolTip = "카메라 줌 속도"))
	float CameraZoomSpeed;

	UPROPERTY(EditAnywhere, Category = FQCameraZoomRange, meta = (ToolTip = "카메라 줌 인/아웃 기준 범위"))
	FVector2D mBaseCheckRange;
	UPROPERTY(EditAnywhere, Category = FQCameraZoomRange, meta = (ToolTip = "카메라 줌 인 시작 거리 (기준 범위 * 해당 값)"))
	float mCheckRangeMinScale;
	UPROPERTY(EditAnywhere, Category = FQCameraZoomRange, meta = (ToolTip = "카메라 줌 아웃 시작 거리 (기준 범위 * 해당 값)"))
	float mCheckRangeMaxScale;
};
