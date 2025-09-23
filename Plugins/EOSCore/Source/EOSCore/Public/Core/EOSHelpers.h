/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Core/EOSTypes.h"

namespace EOSHelpers
{
	static EOSResult ToEOSCoreResult(EOS_EResult result)
	{
		EOSResult Result = EOSResult::EOS_ServiceFailure;

		const FString String = EOS_EResult_ToString(result);

		UEnum* EnumClass = StaticEnum<EOSResult>();

		for (int32 i = 0; i < static_cast<int32>(EOSResult::MAX); i++)
		{
			FString ResultAsString = EnumClass->GetNameStringByIndex(i);;

			if (String == ResultAsString)
			{
				Result = static_cast<EOSResult>(i);
				break;
			}
		}

		return Result;
	}

	static EOS_EResult ToEOSResult(EOSResult result)
	{
		EOS_EResult Result = EOS_EResult::EOS_ServiceFailure;

		UEnum* EnumClass = StaticEnum<EOSResult>();
		const FString ResultAsString = EnumClass->GetNameStringByIndex(static_cast<int32>(result));

		for (int32 i = 0; i < static_cast<int32>(EOS_EResult::EOS_Lobby_MemberUpdateOnly); i++)
		{
			FString String = EOS_EResult_ToString(static_cast<EOS_EResult>(i));

			if (String == ResultAsString)
			{
				Result = static_cast<EOS_EResult>(i);
				break;
			}
		}

		return Result;
	}
}