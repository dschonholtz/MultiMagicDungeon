#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MMDSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMMDSessionCreated, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMMDSessionEnded,   bool, bSuccess);

/**
 * Thin wrapper around IOnlineSession for LAN session lifecycle.
 * Responsibilities: create, destroy. Finding/joining via open <IP> for now.
 *
 * Phase 5+: extend with FindSessions + JoinSession for Steam/EOS discovery.
 */
UCLASS()
class MULTIMAGICDUNGEON_API UMMDSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Create a LAN listen session with the given number of public slots. */
	void CreateSession(int32 NumPublicConnections = 8);

	/** Destroy the current session (call before returning to main menu). */
	void DestroySession();

	UPROPERTY(BlueprintAssignable, Category = "MMD|Sessions")
	FOnMMDSessionCreated OnSessionCreated;

	UPROPERTY(BlueprintAssignable, Category = "MMD|Sessions")
	FOnMMDSessionEnded OnSessionEnded;

private:
	IOnlineSessionPtr GetSessionInterface() const;

	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);

	FDelegateHandle CreateSessionHandle;
	FDelegateHandle DestroySessionHandle;
};
