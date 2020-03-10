/*
 * DialogUXStateAggregator.h
 *
 * Copyright 2018 Rockchip.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://www.rock-chips.com/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef LOCAL_PLAYER_SDK_RKPCOMMON_RKP_INCLUDE_RKPCOMMON_RKP_DIALOGUXSTATEAGGREGATOR_H_
#define LOCAL_PLAYER_SDK_RKPCOMMON_RKP_INCLUDE_RKPCOMMON_RKP_DIALOGUXSTATEAGGREGATOR_H_

#include <chrono>
#include <unordered_set>

#include "LPCommon/SDKInterfaces/AudioInputProcessorObserverInterface.h"
#include <LPCommon/SDKInterfaces/ConnectionStatusObserverInterface.h>
#include "LPCommon/SDKInterfaces/DialogUXStateObserverInterface.h"
#include "LPCommon/SDKInterfaces/MessageObserverInterface.h"
#include "LPCommon/SDKInterfaces/SpeechSynthesizerObserverInterface.h"

#include <LPCommon/Utils/Threading/Executor.h>
#include <LPCommon/Utils/Timing/Timer.h>

namespace localPlayerSDK {
namespace rkpCommon {
namespace rkp {

/**
 * This class serves as a component to aggregate other observer interfaces into one UX component that notifies
 * observers of RKP dialog specific UX changes based on events that occur within these components.
 */
class DialogUXStateAggregator
        : public sdkInterfaces::AudioInputProcessorObserverInterface
        , public sdkInterfaces::SpeechSynthesizerObserverInterface
        , public sdkInterfaces::MessageObserverInterface
        , public sdkInterfaces::ConnectionStatusObserverInterface {
public:
    /**
     * Constructor.
     *
     * @param timeoutForThinkingToIdle This timeout will be used to time out from the THINKING state in case no messages
     * arrive from RKP.
     */
    DialogUXStateAggregator(std::chrono::milliseconds timeoutForThinkingToIdle = std::chrono::seconds{5});

    /**
     * Adds an observer to be notified of UX state changes.
     *
     * @warning The user of this class must make sure that the observer remains valid until the destruction of this
     * object as state changes may come in at any time, leading to callbacks to the observer. Failure to do so may
     * result in crashes when this class attempts to access its observers.
     *
     * @param observer The new observer to notify of UX state changes.
     */
    void addObserver(std::shared_ptr<sdkInterfaces::DialogUXStateObserverInterface> observer);

    /**
     * Removes an observer from the internal collection of observers synchronously. If the observer is not present,
     * nothing will happen.
     *
     * @note This is a synchronous call which can not be made by an observer callback.  Attempting to call
     *     @c removeObserver() from @c DialogUXStateObserverInterface::onDialogUXStateChanged() will result in a
     *     deadlock.
     *
     * @param observer The observer to remove.
     */
    void removeObserver(std::shared_ptr<sdkInterfaces::DialogUXStateObserverInterface> observer);

    void onStateChanged(sdkInterfaces::AudioInputProcessorObserverInterface::State state) override;

    void onStateChanged(sdkInterfaces::SpeechSynthesizerObserverInterface::SpeechSynthesizerState state) override;

    void receive(const std::string& contextId, const std::string& message) override;

private:
    /**
     * Notifies all observers of the current state. This should only be used within the internal executor.
     */
    void notifyObserversOfState();

    /**
     * Sets the internal state to the new state.
     *
     * @param newState The new Nyx UX state.
     */
    void setState(sdkInterfaces::DialogUXStateObserverInterface::DialogUXState newState);

    /**
     * Sets the internal state to @c IDLE if both @c SpeechSynthesizer and @c AudioInputProcessor are in idle state.
     */
    void tryEnterIdleState();

    /**
     * Transitions the internal state from THINKING to IDLE.
     */
    void transitionFromThinkingTimedOut();

    /**
     * Timer callback that makes sure that the state is IDLE if both @c AudioInputProcessor and @c SpeechSynthesizer
     * are in IDLE state.
     */
    void tryEnterIdleStateOnTimer();

    void onConnectionStatusChanged(
        const rkpCommon::sdkInterfaces::ConnectionStatusObserverInterface::Status status,
        const rkpCommon::sdkInterfaces::ConnectionStatusObserverInterface::ChangedReason reason) override;

    /**
     * Called internally when some activity starts: Speech is going to be played or voice recognition is about to start.
     */
    void onActivityStarted();

    /**
     * @name Executor Thread Variables
     *
     * These member variables are only accessed by functions in the @c m_executor worker thread, and do not require any
     * synchronization.
     */
    /// @{

    /// The @c UXObserverInterface to notify any time the Nyx Voice Service UX state needs to change.
    std::unordered_set<std::shared_ptr<sdkInterfaces::DialogUXStateObserverInterface>> m_observers;

    /// The current overall UX state of the RKP system.
    sdkInterfaces::DialogUXStateObserverInterface::DialogUXState m_currentState;

    /// The timeout to be used for transitioning away from the THINKING state in case no messages are received.
    const std::chrono::milliseconds m_timeoutForThinkingToIdle;

    /// A timer to transition out of the THINKING state.
    rkpCommon::utils::timing::Timer m_thinkingTimeoutTimer;

    /// A timer to transition out of the SPEAKING state for multiturn situations.
    rkpCommon::utils::timing::Timer m_multiturnSpeakingToListeningTimer;
    /// @}

    /**
     * An internal executor that performs execution of callable objects passed to it sequentially but asynchronously.
     *
     * @note This declaration needs to come *after* the Executor Thread Variables above so that the thread shuts down
     *     before the Executor Thread Variables are destroyed.
     */
    rkpCommon::utils::threading::Executor m_executor;

    /// Contains the current state of the @c SpeechSynthesizer as reported by @c SpeechSynthesizerObserverInterface
    localPlayerSDK::rkpCommon::sdkInterfaces::SpeechSynthesizerObserverInterface::SpeechSynthesizerState
        m_speechSynthesizerState;

    /// Contains the current state of the @c AudioInputProcessor as reported by @c AudioInputProcessorObserverInterface
    localPlayerSDK::rkpCommon::sdkInterfaces::AudioInputProcessorObserverInterface::State m_audioInputProcessorState;
};

}  // namespace rkp
}  // namespace rkpCommon
}  // namespace localPlayerSDK

#endif  // LOCAL_PLAYER_SDK_RKPCOMMON_RKP_INCLUDE_RKPCOMMON_RKP_DIALOGUXSTATEAGGREGATOR_H_
