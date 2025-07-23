#include "FQMonsterModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FFQMonster, FQMonster);

void FFQMonster::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("FQMonster StartupModule"));
}

void FFQMonster::ShutdownModule()
{
}
