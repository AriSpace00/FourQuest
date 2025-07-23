#include "FQMainCenterCamera.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Components\BoxComponent.h"

#include "FQCameraDataAsset.h"
#include "FourQuest\FourQuest\Framework\InGame\FQ_GameInstance_InGame.h"

AFQMainCenterCamera::AFQMainCenterCamera()
{
    mCurrentCameraZoomValue = 2000.f;

    // 에셋 데이터
    ConstructorHelpers::FObjectFinder<UFQCameraDataAsset> DataAssetRef(TEXT("/Script/FourQuest.FQCameraDataAsset'/Game/Data/DA_CameraData.DA_CameraData'"));
    if (DataAssetRef.Succeeded()) 
    {
        mCameraDataAsset = DataAssetRef.Object;
    }

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // 카메라 붐 컴포넌트
    mCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    mCameraBoom->SetupAttachment(RootComponent);
    mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;
    mCameraBoom->bUsePawnControlRotation = true;
    mCameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    mCameraBoom->bDoCollisionTest = false;
    
    // 카메라 컴포넌트
    mFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    mFollowCamera->SetupAttachment(mCameraBoom, TEXT("SpringEndpoint"));
    mFollowCamera->bUsePawnControlRotation = false;

    // 틱 함수 On
    PrimaryActorTick.bCanEverTick = true;

    SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0.f));
    mCurrentCameraLocation = GetActorLocation();

    // 박스 컴포넌트
    mTopBoxComponents = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBoxComponents"));
    mTopBoxComponents->SetupAttachment(RootComponent);
    mTopBoxComponents->SetBoxExtent(FVector(300, 4000, 500));
    mTopBoxComponents->SetRelativeLocation(FVector(2000, 0, 0));
    mTopBoxComponents->SetCollisionProfileName(TEXT("CameraOutSizeCollision"));

    mBottomBoxComponents = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBoxComponents"));
    mBottomBoxComponents->SetupAttachment(RootComponent);
    mBottomBoxComponents->SetBoxExtent(FVector(50, 4000, 500));
    mBottomBoxComponents->SetRelativeLocation(FVector(-2000, 0, 0));
    mBottomBoxComponents->SetCollisionProfileName(TEXT("CameraOutSizeCollision"));
    
    mLeftBoxComponents = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBoxComponents"));
    mLeftBoxComponents->SetupAttachment(RootComponent);
    mLeftBoxComponents->SetBoxExtent(FVector(4000, 50, 500));
    mLeftBoxComponents->SetRelativeLocation(FVector(0, -2000, 0));
    mLeftBoxComponents->SetRelativeRotation(FRotator(0, -25, 0));
    mLeftBoxComponents->SetCollisionProfileName(TEXT("CameraOutSizeCollision"));

    mRightBoxComponents = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBoxComponents"));
    mRightBoxComponents->SetupAttachment(RootComponent);
    mRightBoxComponents->SetBoxExtent(FVector(4000, 50, 500));
    mRightBoxComponents->SetRelativeLocation(FVector(0, 2000, 0));
    mRightBoxComponents->SetRelativeRotation(FRotator(0, 25, 0));
    mRightBoxComponents->SetCollisionProfileName(TEXT("CameraOutSizeCollision"));
}

void AFQMainCenterCamera::BeginPlay()
{
    Super::BeginPlay();

    mCurrentCameraZoomValue = mCameraDataAsset->mCameraZoomMin;
    mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;

#if WITH_EDITOR
    mFollowCamera->bDrawFrustumAllowed = true;
    mFollowCamera->OverrideFrustumColor(FColor::Green);
#endif
}

void AFQMainCenterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (mCameraDataAsset)
    {
        CameraMoveControl(DeltaTime);           // 카메라 이동
        CameraZoomControl(DeltaTime);           // 카메라 줌 컨트롤
        CameraWallCollisionUpdate(DeltaTime);   // 카메라 벽 (시야 범위의 경계선 콜리전 설치)
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQMainCenterCamera %d] Is Not Find UFQCameraDataAsset"), __LINE__);
    }
}

void AFQMainCenterCamera::CameraMoveControl(float DeltaTime)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // 플레이어 컨트롤러 확인
    TArray<APlayerController*> PlayerControllers;
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        PlayerControllers.Add(It->Get());
    }

    if (PlayerControllers.Num() == 0)
    {
        return;
    }

    // 플레이어들의 위치 확인
    auto LocalPlayerInfomation = Cast<UFQ_GameInstance_InGame>(GetWorld()->GetGameInstance())->GetLocalMultiPlayerInfomation();

    FVector Sum = FVector::ZeroVector;
    int32 Count = 0;
    for (APlayerController* PC : PlayerControllers)
    {
        if (PC && PC->GetPawn())
        {
            if (LocalPlayerInfomation[PC->GetLocalPlayer()->GetLocalPlayerIndex()].bSpawnLocalPlayer)
            {
                Sum += PC->GetPawn()->GetActorLocation();
                Count++;
            }
        }
    }

    // 카메라 위치 조정 (블렌더 처리)
    FVector CenterLocation = Count > 0 ? (Sum / Count) : FVector::ZeroVector;

    FVector TargetLocation = CenterLocation;
    FVector NewLocation = FMath::VInterpTo(mCurrentCameraLocation, TargetLocation, DeltaTime, mCameraDataAsset->mCameraBlendValue);
    SetActorLocation(TargetLocation);
    mCurrentCameraLocation = NewLocation;
}

void AFQMainCenterCamera::CameraZoomControl(float DeltaTime)
{
    TArray<APlayerController*> PlayerControllers;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        PlayerControllers.Add(It->Get());
    }

    // 플레이어의 수가 1명이면 반환
    if (PlayerControllers.Num() <= 1)
    {
        return;
    }

    // 판정 범위 계산
    FVector2D CurrentRange;
    if (mCurrentCameraZoomValue > mCameraDataAsset->mCameraZoomMin)
    {
        float ZoomAlpha = (mCurrentCameraZoomValue - mCameraDataAsset->mCameraZoomMin) / (mCameraDataAsset->mCameraZoomMax - mCameraDataAsset->mCameraZoomMin);
        ZoomAlpha = FMath::Clamp(ZoomAlpha, 0.0f, 1.0f);

        float RangeScale = FMath::Lerp(mCameraDataAsset->mCheckRangeMinScale, mCameraDataAsset->mCheckRangeMaxScale, ZoomAlpha);
        CurrentRange = mCameraDataAsset->mBaseCheckRange * RangeScale;
    }
    else
    {
        CurrentRange = mCameraDataAsset->mBaseCheckRange * mCameraDataAsset->mCheckRangeMinScale;
    }
    
    // 디버그 체크
    if (mCameraDataAsset->mbIsDebugRendering)
    {
        DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), FVector(CurrentRange.X, CurrentRange.Y, 0.f), FColor::Yellow);
        DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), 
            FVector(CurrentRange.X * mCameraDataAsset->mCheckRangeMinScale, CurrentRange.Y * mCameraDataAsset->mCheckRangeMinScale, 0.f), FColor::Green);
        DrawDebugBox(GetWorld(), FVector(mCurrentCameraLocation.X, mCurrentCameraLocation.Y, 0.f), 
            FVector(CurrentRange.X * mCameraDataAsset->mCheckRangeMaxScale, CurrentRange.Y * mCameraDataAsset->mCheckRangeMaxScale, 0.f), FColor::Red);
    }

    // 플레이어와 카메라 간격 계산
    float PlayerDistanceX = 0.0f;
    float PlayerDistanceY = 0.0f;
    for (APlayerController* PC : PlayerControllers)
    {
        if (PC && PC->GetPawn())
        {
            FVector PlayerLocation = PC->GetPawn()->GetActorLocation();
            PlayerDistanceX = FMath::Max(PlayerDistanceX, FMath::Abs(mCurrentCameraLocation.X - PlayerLocation.X));
            PlayerDistanceY = FMath::Max(PlayerDistanceY, FMath::Abs(mCurrentCameraLocation.Y - PlayerLocation.Y));
        }
    }

    // 줌 인, 줌 아웃
    if (PlayerDistanceX >= (CurrentRange.X * mCameraDataAsset->mCheckRangeMaxScale) || PlayerDistanceY >= (CurrentRange.Y * mCameraDataAsset->mCheckRangeMaxScale))
    {
        mCurrentCameraZoomValue = FMath::FInterpTo(mCurrentCameraZoomValue, mCameraDataAsset->mCameraZoomMax, DeltaTime, mCameraDataAsset->CameraZoomSpeed);
        mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;
    }
    else if (PlayerDistanceX <= (CurrentRange.X * mCameraDataAsset->mCheckRangeMinScale) || PlayerDistanceY <= (CurrentRange.Y * mCameraDataAsset->mCheckRangeMinScale))
    {
        mCurrentCameraZoomValue = FMath::FInterpTo(mCurrentCameraZoomValue, mCameraDataAsset->mCameraZoomMin, DeltaTime, mCameraDataAsset->CameraZoomSpeed);
        mCameraBoom->TargetArmLength = mCurrentCameraZoomValue;
    }
}

void AFQMainCenterCamera::CameraWallCollisionUpdate(float DeltaTime)
{
    RaycastFrustumEdges();
}

void AFQMainCenterCamera::RaycastFrustumEdges()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !mFollowCamera) return;

    // 1. Viewport 크기 가져오기
    int32 ViewportX, ViewportY;
    PC->GetViewportSize(ViewportX, ViewportY);

    // 2. 화면 중심 기준 Top/Bottom/Left/Right 좌표 계산
    const float CenterX = ViewportX * 0.5f;
    const float CenterY = ViewportY * 0.5f;

    TMap<EFrustumDirection, FVector2D> ScreenPoints;
    ScreenPoints.Add(EFrustumDirection::Top, FVector2D(CenterX, 0.f));
    ScreenPoints.Add(EFrustumDirection::Bottom, FVector2D(CenterX, ViewportY - 1.f));
    ScreenPoints.Add(EFrustumDirection::Left, FVector2D(0.f, CenterY));
    ScreenPoints.Add(EFrustumDirection::Right, FVector2D(ViewportX - 1.f, CenterY));

    // 3. 레이 쏠 기준점
    FVector RayStartPoint = FMath::Lerp(GetActorLocation(), mFollowCamera->GetComponentLocation(), 1.f);
    const float TraceLength = 10000.f;

    // 4. 컴포넌트 대응
    TMap<EFrustumDirection, UBoxComponent*> TargetBoxMap;
    TargetBoxMap.Add(EFrustumDirection::Top, mTopBoxComponents);
    TargetBoxMap.Add(EFrustumDirection::Bottom, mBottomBoxComponents);
    TargetBoxMap.Add(EFrustumDirection::Left, mLeftBoxComponents);
    TargetBoxMap.Add(EFrustumDirection::Right, mRightBoxComponents);

    // 5. 레이 쏘기 함수
    for (const TPair<EFrustumDirection, FVector2D>& Pair : ScreenPoints)
    {
        EFrustumDirection DirectionType = Pair.Key;
        FVector2D ScreenPos = Pair.Value;

        FVector WorldOrigin, WorldDirection;
        if (PC->DeprojectScreenPositionToWorld(ScreenPos.X, ScreenPos.Y, WorldOrigin, WorldDirection))
        {
            FVector TraceEnd = RayStartPoint + WorldDirection * TraceLength;

            TArray<FHitResult> HitResults;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(this);

            if (GetWorld()->LineTraceMultiByChannel(HitResults, RayStartPoint, TraceEnd, ECC_Visibility, Params))
            {
                for (auto HitResult : HitResults)
                {
                    // Actor 또는 Component에 Tag가 붙어 있는지 확인
                    AActor* HitActor = HitResult.GetActor();
                    if (!HitActor || !HitActor->ActorHasTag(TEXT("Floor")))
                    {
                        return;
                    }

                    UBoxComponent* TargetBox = TargetBoxMap[DirectionType];
                    if (TargetBox)
                    {
                        TargetBox->SetWorldLocation(HitResult.ImpactPoint);

                        // 디버그 충돌
                        if (mCameraDataAsset->mbIsDebugRendering)
                        {
                            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 12.f, 25.f, FColor::White);
                        }
                    }
                }
            }

            // 디버그 라인
            if (mCameraDataAsset->mbIsDebugRendering)
            {
				DrawDebugLine(GetWorld(), RayStartPoint, TraceEnd, FColor::White);
            }
        }
    }

    // 6. 디버그 박스 시각화
    if (mCameraDataAsset->mbIsDebugRendering)
    {
        DrawDebugBox(GetWorld(), mTopBoxComponents->GetComponentLocation(), mTopBoxComponents->GetScaledBoxExtent(), mTopBoxComponents->GetComponentQuat(), FColor::Red);
        DrawDebugBox(GetWorld(), mBottomBoxComponents->GetComponentLocation(), mBottomBoxComponents->GetScaledBoxExtent(), mBottomBoxComponents->GetComponentQuat(), FColor::Green);
        DrawDebugBox(GetWorld(), mRightBoxComponents->GetComponentLocation(), mRightBoxComponents->GetScaledBoxExtent(), mRightBoxComponents->GetComponentQuat(), FColor::Blue);
        DrawDebugBox(GetWorld(), mLeftBoxComponents->GetComponentLocation(), mLeftBoxComponents->GetScaledBoxExtent(), mLeftBoxComponents->GetComponentQuat(), FColor::Yellow);
    }
}