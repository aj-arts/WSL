/*++

Copyright (c) Microsoft. All rights reserved.

Module Name:

    WSLCIdleState.h

Abstract:

    Shared idle-termination state for WSLC session VM lifecycle.

--*/
#pragma once

#include <atomic>
#include <wil/resource.h>

namespace wsl::windows::service::wslc {

// Shared idle-termination state for a WSLC session. Held via shared_ptr so activity tokens and
// container COM wrappers can outlive the session (e.g. a client keeps a root-namespace process or
// container proxy past releasing the session) and still safely release their activity reference /
// wake the idle worker without keeping the session object alive. Tearing down the session therefore
// proceeds normally; a late token release simply decrements the count and signals an event with no
// waiter.
struct IdleState
{
    std::atomic<int> ActivityCount{0};
    wil::unique_event IdleCheckEvent{wil::EventOptions::ManualReset};
    
    // Protects the invariant that COM refcount transitions (1→2, 2→1) and ActivityCount
    // increments/decrements happen atomically together in WSLCContainer::AddRef/Release.
    // Without this lock, concurrent AddRef and Release can interleave such that Release
    // decrements ActivityCount before AddRef increments it, causing underflow.
    wil::srwlock ActivityLock;
};

} // namespace wsl::windows::service::wslc
