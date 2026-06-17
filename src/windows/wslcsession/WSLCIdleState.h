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
};

} // namespace wsl::windows::service::wslc
