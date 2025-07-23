
#include "FQPlayerUIActor.h"
#include "FQUI\FQWidgetComponent.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerState_InGame.h"
#include "FQUI\Player\FQPlayerHpWidget.h"
#include "FQUI\Soul\FQSoulGaugeWidget.h"

AFQPlayerUIActor::AFQPlayerUIActor()
{
	// Tick 함수 실행
	PrimaryActorTick.bCanEverTick = true;

	// 위젯 컴포넌트 생성
	mPlayerHpWidgetComponent = CreateDefaultSubobject<UFQWidgetComponent>(TEXT("PlayerHpWidget"));
	mArmourGaugeWidgetComponent = CreateDefaultSubobject<UFQWidgetComponent>(TEXT("GaugeWidget"));
	mPlayerHpWidgetComponent->SetupAttachment(RootComponent);
	mArmourGaugeWidgetComponent->SetupAttachment(RootComponent);

	// Player Hp 위젯 세팅
	static ConstructorHelpers::FClassFinder<UFQPlayerHpWidget> PlayerHpWidgetRef(TEXT("/Game/Blueprints/Player/WBP_PlayerHp.WBP_PlayerHp_C"));
	if (PlayerHpWidgetRef.Class)
	{
		mPlayerHpWidgetComponent->SetWidgetClass(PlayerHpWidgetRef.Class);
		mPlayerHpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		mPlayerHpWidgetComponent->SetDrawSize(FVector2D(30.f, 30.f));
		mPlayerHpWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidgetClass 클래스를 못 찾았습니다!!"), __LINE__);
	}

	// Player Gauge 위젯 세팅
	static ConstructorHelpers::FClassFinder<UFQSoulGaugeWidget> GaugeWidgetRef(TEXT("/Game/Blueprints/Player/WBP_ArmourGauge.WBP_ArmourGauge_C"));
	if (GaugeWidgetRef.Class)
	{
		mArmourGaugeWidgetComponent->SetWidgetClass(GaugeWidgetRef.Class);
		mArmourGaugeWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		mArmourGaugeWidgetComponent->SetDrawSize(FVector2D(10.f, 10.f));
		mArmourGaugeWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mArmourGaugeWidgetComponent->SetVisibility(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mGaugeWidgetClass 클래스를 못 찾았습니다!!"), __LINE__);
	}
}

void AFQPlayerUIActor::BeginPlay()
{
	Super::BeginPlay();

	// UI가 제대로 생성이 되었는지 체크
	mPlayerHpWidget = Cast<UFQPlayerHpWidget>(mPlayerHpWidgetComponent->GetUserWidgetObject());
	if (!mPlayerHpWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget 캐스팅 실패!"), __LINE__);
	}

	mArmourGaugeWidget = Cast<UFQSoulGaugeWidget>(mArmourGaugeWidgetComponent->GetUserWidgetObject());
	if (!mArmourGaugeWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget 캐스팅 실패!"), __LINE__);
	}
}

void AFQPlayerUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Hp 감소
	if (mPlayerHpWidget && mPlayerHpWidgetComponent->GetVisibleFlag())
	{
		mPlayerHpWidget->UpdateHpDecrasePercent(DeltaTime);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget가 유효하지 않습니다!"), __LINE__);
	}
}

void AFQPlayerUIActor::UpdatePlayerNumber(int32 PlayerControllerNumber, ESoulType PlayerSoulType)
{
	UE_LOG(LogTemp, Log, TEXT("[AFQPlayerUIActor %d] UpdatePlayerNumber 함수 실행 : (플레이어 컨트롤러 숫자 : %d, 플레이어 영혼 타입 : %d)"), __LINE__, PlayerControllerNumber, (int32)PlayerSoulType);
	
	// Player Head UI 업데이트
	if (mPlayerHpWidget)
	{
		mPlayerHpWidget->UpdatePlayerControllerNumber(PlayerControllerNumber, PlayerSoulType);
	}
}

void AFQPlayerUIActor::UpdatePlayerHp(float HpValue)
{
	// Player Hp 업데이트
	if (mPlayerHpWidget)
	{
		mPlayerHpWidget->UpdatePlayerHp(HpValue);
	}
}

void AFQPlayerUIActor::UpdateArmourGauge(bool bIsVisible, float GaugeValue)
{
	// 갑옷 게이지 업데이트
	if (mArmourGaugeWidgetComponent && mArmourGaugeWidget)
	{
		mArmourGaugeWidgetComponent->SetVisibility(bIsVisible);
		mArmourGaugeWidget->UpdateChargeGaugeValue(GaugeValue);
	}
}

void AFQPlayerUIActor::UpdateArmourType(EArmourType ChangedArmourType)
{
	if (!mPlayerHpWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerUIActor %d] mPlayerHpWidget가 유효하지 않습니다!"), __LINE__);
		return;
	}

	// 갑옷 장착 여부에 따라 Hp UI 렌더링
	if (ChangedArmourType == EArmourType::End)
	{
		mPlayerHpWidget->SetHpBarVisibility(false);
	}
	else
	{
		mPlayerHpWidget->SetHpBarVisibility(true);
	}
}
