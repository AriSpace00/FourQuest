// Fill out your copyright notice in the Description page of Project Settings.


#include "FQCameraDataAsset.h"

UFQCameraDataAsset::UFQCameraDataAsset()
{
    // 디버그 렌더링
    mbIsDebugRendering = false;

    // 카메라 줌인 줌아웃
    mCameraZoomMin = 2000.f;
    mCameraZoomMax = 3500.f;
    CameraZoomSpeed = 0.2f;

    // 카메라 줌인 줌아웃 판정 범위
    mBaseCheckRange = FVector2D(800.f, 1500.f);
    mCheckRangeMaxScale = 1.25f;
    mCheckRangeMinScale = 0.75f;

    // 카메라 이동 블렌더 값
    mCameraBlendValue = 0.5f;
}
