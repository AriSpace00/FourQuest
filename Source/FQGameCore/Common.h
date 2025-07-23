#pragma once

// 플레이어 상태
UENUM()
enum class EArmourType : uint8
{
	Knight = 0	UMETA(DisplayName = "Knight"),
	Magic		UMETA(DisplayName = "Magic"),
	End
};

UENUM()
enum class ESoulType : uint8
{
	Sword	UMETA(DisplayName = "Sword"),	// 검 영혼 0
	Staff	UMETA(DisplayName = "Staff"),	// 마법 영혼 1
	Axe		UMETA(DisplayName = "Axe"),		// 도끼 영혼 2
	Bow		UMETA(DisplayName = "Bow"),		// 활 영혼 3
	End
};


namespace FQ_PlayerStateDelegate
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FQSoulTypeChangeDelegate, ESoulType /*NewSoulType*/);					// PlayerState의 SoulType 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQArmourTypeChangeDelegate, EArmourType /*NewArmourType*/);				// PlayerState의 ArmourType 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQSoulGaugeSettingDelegate, float /*GaugeValue*/);						// PlayerState의 SoulGauge 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQPlayerHpUpdateDelegate, float /*HpValue*/);							// PlayerState의 Hp 변경 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FQPlayerDeadDelegate, bool /*bIsDead*/);								// PlayerState의 Dead 호출 델리게이트
}


// 인 게임 관련 세팅
namespace FQ_InGameSetting
{
	UENUM()
		enum class EResolutionType : uint8
	{
		Resolution_1080x600		UMETA(DisplayName = "Resolution_1080x600"),
		Resolution_1280x720		UMETA(DisplayName = "Resolution_1280x720"),
		Resolution_1600x900		UMETA(DisplayName = "Resolution_1600x900"),
		Resolution_1920x1080	UMETA(DisplayName = "Resolution_1920x1080"),
	};

	struct FQLocalMultiPlayerInfomation
	{
		FQLocalMultiPlayerInfomation() : bSpawnLocalPlayer(false), mSoulType(ESoulType::End), mArmourType(EArmourType::End) {}

		uint8 bSpawnLocalPlayer : 1;
		ESoulType mSoulType;
		EArmourType mArmourType;
	};

	struct FAudioInfomation
	{
		FAudioInfomation() : mbIsFullVolumMute(false), mbIsBGMVolumMute(false), mbIsSFXVolumMute(false), mFullVolumValue(0.8f), mBGMVolumValue(0.8f), mSFXVolumValue(0.8f) {}

		uint8 mbIsFullVolumMute : 1;
		uint8 mbIsBGMVolumMute : 1;
		uint8 mbIsSFXVolumMute : 1;

		float mFullVolumValue;
		float mBGMVolumValue;
		float mSFXVolumValue;
	};

	struct FVideoInfomation
	{
		FVideoInfomation() : mbIsFullScreen(false), mCurrentResolutionType(EResolutionType::Resolution_1600x900) {}

		uint8 mbIsFullScreen : 1;
		EResolutionType mCurrentResolutionType;
	};
}


// UI
UENUM()
enum class EWidgetInputType : uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right"),
	Up			UMETA(DisplayName = "Up"),
	Down		UMETA(DisplayName = "Down"),
	Select		UMETA(DisplayName = "Select"),
	Cancel		UMETA(DisplayName = "Cancel"),
	End
};

UENUM()
enum class ESettingUIType : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	MainSetting		UMETA(DisplayName = "MainSetting"),
	VideoSetting	UMETA(DisplayName = "VideoSetting"),
	AudioSetting	UMETA(DisplayName = "AudioSetting"),
	End
};

namespace FQ_UIDelegate
{
	DECLARE_DELEGATE_OneParam(FQSettingUIDelegate, ESettingUIType /*TitleSettingType*/);
}


// 퀘스트
UENUM()
enum class EQuestStateType
{
	Started			UMETA(DisplayName = "Started"),
	InPrograss		UMETA(DisplayName = "InPrograss"),
	Exit			UMETA(DisplayName = "Exit"),
	End				UMETA(DisplayName = "End"),
};

UENUM()
enum class EQuestType : uint8
{
	None			UMETA(DisplayName = "None"),
	Interaction		UMETA(DisplayName = "Interaction"),
	MonsterKill		UMETA(DisplayName = "MonsterKill"),
};

UENUM()
enum class EQuestTriggerType : uint8
{
	None		UMETA(DisplayName = "None"),
	QuestStarts UMETA(DisplayName = "QuestStarts"),
	QuestClear	UMETA(DisplayName = "QuestClear"),
};

UENUM()
enum class EQuestMonsterType : uint8
{
	None					UMETA(DisplayName = "None"),
	CommonMeleeMonster		UMETA(DisplayName = "CommonMeleeMonster"),
	CommonRangedMonster		UMETA(DisplayName = "CommonRangedMonster"),
	CommonSpawnerMonster	UMETA(DisplayName = "CommonSpawnerMonster"),
	BossMonster				UMETA(DisplayName = "BossMonster"),

	MonsterGroup			UMETA(DisplayName = "MonsterGroup"),
};

UENUM()
enum class EQuestInteractionType : uint8
{
	None			UMETA(DisplayName = "None"),
	Teleport		UMETA(DisplayName = "Teleport"),
	Activate		UMETA(DisplayName = "Activate"),
	Arrive			UMETA(DisplayName = "Arrive"),
	Escape			UMETA(DisplayName = "Escape"),
	Capture			UMETA(DisplayName = "Capture"),
};

#include "Quest\FQQuestTable.h"

namespace FQ_QuestDelegate
{
	DECLARE_DELEGATE_TwoParams(FQQuestTriggerDelegate, int32 /*QuestID*/, EQuestTriggerType /*QuestTriggerType*/);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FQQuestActiveDelegate,int32 /*QuestID*/, bool /*bIsQuestActive*/);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FQQuestMonsterDelegate, EQuestMonsterType /*QuestMonsterType*/, FName /*MonsterGroupName*/);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FQQuestInteractionDelegate, EQuestInteractionType /*QuestInteractionType*/, int32 /*QuestConditionCount*/);
	DECLARE_MULTICAST_DELEGATE_OneParam(FQQuestClearDelegate, int32 /*ClearQuestID*/);
}