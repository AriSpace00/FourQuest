#pragma once

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	CanMove		UMETA(DisplayName = "CanMove"),
	CannotMove	UMETA(DisplayName = "CannotMove")
};

UENUM(BlueprintType)
enum class EHitState : uint8
{
	None		UMETA(DisplayName = "None"),
	HitReacting	UMETA(DisplayName = "HitReacting")
};

UENUM(BlueprintType)
enum class EComboState : uint8
{
	None			UMETA(DisplayName = "None"),
	CanBeCombo		UMETA(DisplayName = "CanBeCombo"),			// 입력 대기 시간 내에 입력이 들어온다면, 콤보 가능 상태
	Combo			UMETA(DisplayName = "Combo")				// 실제로 입력이 들어와 콤보가 된 상태
};

// Knight
UENUM(BlueprintType)
enum class EKnightSwordAttackState : uint8
{
	None			UMETA(DisplayName = "None"),
	Attack1			UMETA(DisplayName = "Attack1"),
	Attack2			UMETA(DisplayName = "Attack2"),
	Attack3			UMETA(DisplayName = "Attack3"),
	CoolDown		UMETA(DisplayName = "CoolDown")
};

UENUM(BlueprintType)
enum class EKnightShieldState : uint8
{
	None			UMETA(DisplayName = "None"),
	Shield			UMETA(DisplayName = "Shield"),
	CoolDown		UMETA(DisplayName = "CoolDown")
};

// Mage
UENUM(BlueprintType)
enum class EMageProjectileAttackState : uint8
{
	None			UMETA(DisplayName = "None"),
	Attack1			UMETA(DisplayName = "Attack1"),
	Attack2			UMETA(DisplayName = "Attack2"),
	CoolDown		UMETA(DisplayName = "CoolDown")
};

UENUM(BlueprintType)
enum class EMageExplosionState : uint8
{
	None			UMETA(DisplayName = "None"),
	Enabled			UMETA(DisplayName = "Enabled"),
	Destroy			UMETA(DisplayName = "Destroy"),
	CoolDown		UMETA(DisplayName = "CoolDown")
};

UENUM(BlueprintType)
enum class EMageLaserState : uint8
{
	None			UMETA(DisplayName = "None"),
	InputReceived	UMETA(DisplayName = "InputReceived"),
	Preparing		UMETA(DisplayName = "Preparing"),
	Enabled			UMETA(DisplayName = "Enabled"),
	CoolDown		UMETA(DisplayName = "CoolDown")
};
