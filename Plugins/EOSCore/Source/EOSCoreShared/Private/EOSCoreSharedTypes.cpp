/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSCoreSharedTypes.h"

#if PLATFORM_IOS

#if (defined(__IPHONE_13_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_13_0)
#define PRESENTATIONCONTEXT_REQUIRED 1
#else
#define PRESENTATIONCONTEXT_REQUIRED 0
#endif

#if PRESENTATIONCONTEXT_REQUIRED
#import <Foundation/Foundation.h>
#import <AuthenticationServices/AuthenticationServices.h>
#include "IOS/IOSAppDelegate.h"

@interface PresentationContext : NSObject <ASWebAuthenticationPresentationContextProviding>
{
}
@end

@implementation PresentationContext
- (ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:(ASWebAuthenticationSession *)session
{
	if ([IOSAppDelegate GetDelegate].Window == nullptr)
	{
		NSLog(@"authorizationController: presentationAnchorForAuthorizationController: error window is NULL");
	}
	return [IOSAppDelegate GetDelegate].Window;
}
@end

static PresentationContext* PresentationContextProvider = nil;
#endif
#endif

FAuthCredentials::FAuthCredentials(FString id, FString token)
	: EOS_Auth_Credentials()
{
	ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;

	if (!id.IsEmpty())
	{
		Id = new char[256];
		FCStringAnsi::Strncpy(const_cast<char*>(Id), TCHAR_TO_UTF8(*id), 256);
	}
	else
	{
		id = nullptr;
	}

	if (!token.IsEmpty())
	{
		Token = new char[4096];
		FCStringAnsi::Strncpy(const_cast<char*>(Token), TCHAR_TO_UTF8(*token), 4096);
	} else
	{
		Token = nullptr;
	}

	SystemAuthCredentialsOptions = nullptr;

#if PLATFORM_IOS
#if PRESENTATIONCONTEXT_REQUIRED
	if (PresentationContextProvider == nil)
	{
		PresentationContextProvider = [PresentationContext new];
	}

#endif

	CredentialsOptions.ApiVersion = EOS_IOS_AUTH_CREDENTIALSOPTIONS_API_LATEST;
		
#if PRESENTATIONCONTEXT_REQUIRED
	CredentialsOptions.PresentationContextProviding = (void*)CFBridgingRetain(PresentationContextProvider);		// SDK will release when consumed
	#else
	CredentialsOptions.PresentationContextProviding = nullptr;
#endif

	SystemAuthCredentialsOptions = (void*)&CredentialsOptions;
#endif
}

FAuthCredentials::~FAuthCredentials()
{
	delete[] Id;
	delete[] Token;

#if PLATFORM_IOS
#if PRESENTATIONCONTEXT_REQUIRED
	if (PresentationContextProvider != nil)
	{
		[PresentationContextProvider release];
		PresentationContextProvider = nil;
	}
#endif
#endif
}
