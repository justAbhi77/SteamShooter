/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "IpConnection.h"
#include "NetConnectionEOSCore.generated.h"

UCLASS(Transient, Config=Engine)
class UNetConnectionEOSCore : public UIpConnection
{
	GENERATED_BODY()

public:
	explicit UNetConnectionEOSCore(const FObjectInitializer& ObjectInitializer);
	virtual void InitLocalConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitRemoteConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, const FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket = 0,
	                                  int32 InPacketOverhead = 0) override;
	virtual void CleanUp() override;
public:
	void DestroyEOSConnection();
public:
	bool bIsPassthrough;
	bool bHasP2PSession;
};
