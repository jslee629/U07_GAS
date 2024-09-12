#include "CSpawnBotDataAsset.h"

UCSpawnBotDataAsset::UCSpawnBotDataAsset()
{
	BotColor = FLinearColor::Black;
}

FPrimaryAssetId UCSpawnBotDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType(TEXT("Bot")), GetFName());
}
