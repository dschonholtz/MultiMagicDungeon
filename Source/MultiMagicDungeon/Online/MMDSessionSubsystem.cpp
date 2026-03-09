#include "Online/MMDSessionSubsystem.h"
#include "MMDLog.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

static const FName MMDSessionName = TEXT("MMDGameSession");

IOnlineSessionPtr UMMDSessionSubsystem::GetSessionInterface() const
{
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	return OSS ? OSS->GetSessionInterface() : nullptr;
}

void UMMDSessionSubsystem::CreateSession(int32 NumPublicConnections)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (!Sessions)
	{
		UE_LOG(LogMMD, Warning, TEXT("CreateSession: no session interface available"));
		OnSessionCreated.Broadcast(false);
		return;
	}

	FOnlineSessionSettings Settings;
	Settings.bIsLANMatch            = true;
	Settings.bUsesPresence          = false;
	Settings.NumPublicConnections   = NumPublicConnections;
	Settings.bAllowJoinInProgress   = true;
	Settings.bShouldAdvertise       = true;
	Settings.bAllowJoinViaPresence  = false;

	CreateSessionHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMMDSessionSubsystem::OnCreateSessionComplete));

	if (!Sessions->CreateSession(0, MMDSessionName, Settings))
	{
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
		UE_LOG(LogMMD, Warning, TEXT("CreateSession call failed immediately"));
		OnSessionCreated.Broadcast(false);
	}
}

void UMMDSessionSubsystem::DestroySession()
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (!Sessions)
	{
		OnSessionEnded.Broadcast(false);
		return;
	}

	DestroySessionHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(
		FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMMDSessionSubsystem::OnDestroySessionComplete));

	if (!Sessions->DestroySession(MMDSessionName))
	{
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionHandle);
		OnSessionEnded.Broadcast(false);
	}
}

void UMMDSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (Sessions)
	{
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
	}
	UE_LOG(LogMMD, Log, TEXT("Session '%s' created: %s"), *SessionName.ToString(), bSuccess ? TEXT("OK") : TEXT("FAILED"));
	OnSessionCreated.Broadcast(bSuccess);
}

void UMMDSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	IOnlineSessionPtr Sessions = GetSessionInterface();
	if (Sessions)
	{
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionHandle);
	}
	UE_LOG(LogMMD, Log, TEXT("Session '%s' destroyed: %s"), *SessionName.ToString(), bSuccess ? TEXT("OK") : TEXT("FAILED"));
	OnSessionEnded.Broadcast(bSuccess);
}
