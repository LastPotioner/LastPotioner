#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterStateChangedSignature, ECharacterState);

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Dodging UMETA(DisplayName = "Dodging"),
	EAS_Throwing UMETA(DisplayName = "Throwing"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_ArmingWeapon UMETA(DisplayName = "Arming Weapon"),
	EAS_DisarmingWeapon UMETA(DisplayName = "Disarming Weapon"),
	EAS_DisarmingBeforeArming UMETA(DisplayName = "Disarming Before Arming"),
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Death0 UMETA(DisplayName = "Death0"),
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
};
