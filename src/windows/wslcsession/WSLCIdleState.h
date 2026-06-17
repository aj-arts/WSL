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

// Shared idle-termination state for a WSLC session. Held via shared_ptr so activity tokens
// and container wrappers can outlive the session and safely release activity without keeping
// the session object alive.
struct IdleState
{
    std::atomic<int> ActivityCount{0};
    wil::unique_event IdleCheckEvent{wil::EventOptions::ManualReset};

    // Protects COM refcount transitions (1→2, 2→1) and ActivityCount increments/decrements
    // in WSLCContainer::AddRef/Release to prevent underflow.
    wil::srwlock ActivityLock;
};

} // namespace wsl::windows::service::wslc
