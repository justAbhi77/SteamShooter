
#include "BlasterGameMode.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimedCharacter, ABlasterPlayerController* VictimController,
                                        ABlasterPlayerController* AttackerController)
{
	if(ElimedCharacter)
	{
		ElimedCharacter->Elim();
	}
}
