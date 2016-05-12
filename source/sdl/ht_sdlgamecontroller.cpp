/**
**    Hatchit Engine
**    Copyright(c) 2015 Third-Degree
**
**    GNU Lesser General Public License
**    This file may be used under the terms of the GNU Lesser
**    General Public License version 3 as published by the Free
**    Software Foundation and appearing in the file LICENSE.LGPLv3 included
**    in the packaging of this file. Please review the following information
**    to ensure the GNU Lesser General Public License requirements
**    will be met: https://www.gnu.org/licenses/lgpl.html
**
**/

#include <ht_sdlgamecontroller.h>
#include <ht_debug.h>

namespace Hatchit
{
    namespace Game
    {
        bool SDLGameController::AttachController(int joystickDeviceIndex)
        {
            // Validate that the joystick at the provided index is a controller.
            if (!SDL_IsGameController(joystickDeviceIndex))
            {
                HT_DEBUG_PRINTF("[SDL INPUT] Attempted to attach SDL_Joystick as SDL_GameController!");
                return false;
            }

            // Check if there are any open controller slots.
            if (m_freeControllerSlots.empty())
            {
                HT_DEBUG_PRINTF("[SDL INPUT] Failed to attach SDL_GameController, all slots full!");
                return false;
            }

            // Pop a ControllerSlot off of the stack.
            IController::ControllerSlot slot = m_freeControllerSlots.top();
            m_freeControllerSlots.pop();

            // Attach the controller to the slot.
            SDL_GameController* controller = SDL_GameControllerOpen(joystickDeviceIndex);
            SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
            SDL_JoystickID joystickInstanceId = SDL_JoystickInstanceID(joystick);
            m_idToControllerSlot.emplace(joystickInstanceId, slot);
            m_connectedGamepads.emplace(slot, controller);

            // Reset the input state for this controller.
            m_currentButtonState[slot] = ControllerButtonState{};
            m_previousButtonState[slot] = ControllerButtonState{};
            m_currentAxisState[slot] = ControllerAxisState{};

            return true;
        }

        bool SDLGameController::DetachController(SDL_JoystickID joystickInstanceId)
        {
            // Check that this joystick is currently mapped to a controller slot.
            auto iter = m_idToControllerSlot.find(joystickInstanceId);
            if (iter == m_idToControllerSlot.cend())
            {
                HT_DEBUG_PRINTF("[SDL INPUT] Failed to locate controller to remove!");
                return false;
            }

            // Close the SDL_GameController.
            IController::ControllerSlot slot = m_idToControllerSlot[joystickInstanceId];
            SDL_GameController* controller = m_connectedGamepads[slot];
            SDL_GameControllerClose(controller);

            // Clean up the respective map entries.
            m_idToControllerSlot.erase(joystickInstanceId);
            m_connectedGamepads.erase(slot);

            // This controller slot is now free again.
            m_freeControllerSlots.push(slot);

            return true;
        }

        bool SDLGameController::IsControllerAttached(IController::ControllerSlot slot)
        {
            auto iter = m_connectedGamepads.find(slot);
            return (iter != m_connectedGamepads.cend());
        }

        void SDLGameController::ButtonPressed(SDL_JoystickID id, SDL_GameControllerButton button)
        {
            IController::ControllerSlot slot = m_idToControllerSlot[id];
            ControllerButtonState& state = m_currentButtonState[slot];
            state.set(button);
        }

        void SDLGameController::ButtonReleased(SDL_JoystickID id, SDL_GameControllerButton button)
        {
            IController::ControllerSlot slot = m_idToControllerSlot[id];
            ControllerButtonState& state = m_currentButtonState[slot];
            state.reset(button);
        }

        void SDLGameController::AxisMotion(SDL_JoystickID id, SDL_GameControllerAxis axis, std::int16_t value)
        {
            IController::ControllerSlot slot = m_idToControllerSlot[id];
            ControllerAxisState& state = m_currentAxisState[slot];
            state[axis] = value;
        }

        SDL_GameControllerButton SDLGameController::ConvertButtonFromHatchitToSDL(IController::Buttons button)
        {
            switch (button)
            {
            case IController::Buttons::A:
                return SDL_CONTROLLER_BUTTON_A;
            case IController::Buttons::B:
                return SDL_CONTROLLER_BUTTON_B;
            case IController::Buttons::X:
                return SDL_CONTROLLER_BUTTON_X;
            case IController::Buttons::Y:
                return SDL_CONTROLLER_BUTTON_Y;
            case IController::Buttons::UP:
                return SDL_CONTROLLER_BUTTON_DPAD_UP;
            case IController::Buttons::DOWN:
                return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
            case IController::Buttons::LEFT:
                return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
            case IController::Buttons::RIGHT:
                return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
            case IController::Buttons::LEFT_BUMPER:
                return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
            case IController::Buttons::RIGHT_BUMPER:
                return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
            case IController::Buttons::LEFT_STICK:
                return SDL_CONTROLLER_BUTTON_LEFTSTICK;
            case IController::Buttons::RIGHT_STICK:
                return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
            case IController::Buttons::START:
                return SDL_CONTROLLER_BUTTON_START;
            case IController::Buttons::BACK:
                return SDL_CONTROLLER_BUTTON_BACK;
            }

            return SDL_CONTROLLER_BUTTON_INVALID;
        }

        SDL_GameControllerAxis SDLGameController::ConvertAxisFromHatchitToSDL(IController::Axis axis)
        {
            switch (axis)
            {
            case IController::Axis::LEFT_AXIS_X:
                return SDL_CONTROLLER_AXIS_LEFTX;
            case IController::Axis::LEFT_AXIS_Y:
                return SDL_CONTROLLER_AXIS_LEFTY;
            case IController::Axis::RIGHT_AXIS_X:
                return SDL_CONTROLLER_AXIS_RIGHTX;
            case IController::Axis::RIGHT_AXIS_Y:
                return SDL_CONTROLLER_AXIS_RIGHTY;
            case IController::Axis::LEFT_TRIGGER:
                return SDL_CONTROLLER_AXIS_TRIGGERLEFT;
            case IController::Axis::RIGHT_TRIGGER:
                return SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
            }

            return SDL_CONTROLLER_AXIS_INVALID;
        }

        void SDLGameController::VInitialize(void)
        {
            // Push onto the stack in reverse order.
            m_freeControllerSlots.push(IController::ControllerSlot::FOUR);
            m_freeControllerSlots.push(IController::ControllerSlot::THREE);
            m_freeControllerSlots.push(IController::ControllerSlot::TWO);
            m_freeControllerSlots.push(IController::ControllerSlot::ONE);

            // Probe for any controllers already connected.
            for (int deviceIndex = 0; deviceIndex < SDL_NumJoysticks(); ++deviceIndex)
            {
                AttachController(deviceIndex);
            }
        }

        void SDLGameController::VDeInitialize(void)
        {
            // Clean up any gamepads that are still connected.
            for (auto& pair : m_connectedGamepads)
            {
                SDL_GameControllerClose(pair.second);
            }
        }

        void SDLGameController::VUpdate(void)
        {
            m_previousButtonState = m_currentButtonState;
        }

        bool SDLGameController::VButtonHeld(IController::ControllerSlot slot, IController::Buttons button)
        {
            if (!IsControllerAttached(slot))
            {
                HT_DEBUG_PRINTF("[SDL INPUT] There is no controller attached at the requested slot!");
                return false;
            }

            SDL_GameControllerButton sdlButton = ConvertButtonFromHatchitToSDL(button);
            return (m_currentButtonState[slot][sdlButton] == m_previousButtonState[slot][sdlButton]);
        }

        bool SDLGameController::VButtonPressed(IController::ControllerSlot slot, IController::Buttons button)
        {
            if (!IsControllerAttached(slot))
            {
                HT_DEBUG_PRINTF("[SDL INPUT] There is no controller attached at the requested slot!");
                return false;
            }

            SDL_GameControllerButton sdlButton = ConvertButtonFromHatchitToSDL(button);
            return m_currentButtonState[slot][sdlButton];
        }

        bool SDLGameController::VButtonReleased(IController::ControllerSlot slot, IController::Buttons button)
        {
            if (!IsControllerAttached(slot))
            {
                HT_DEBUG_PRINTF("[SDL INPUT] There is no controller attached at the requested slot!");
                return false;
            }

            SDL_GameControllerButton sdlButton = ConvertButtonFromHatchitToSDL(button);
            return !m_currentButtonState[slot][sdlButton];
        }

        std::int16_t SDLGameController::VAxisValue(IController::ControllerSlot slot, IController::Axis axis)
        {
            if (!IsControllerAttached(slot))
            {
                HT_DEBUG_PRINTF("[SDL INPUT] There is no controller attached at the requested slot!");
                return false;
            }

            SDL_GameControllerAxis sdlAxis = ConvertAxisFromHatchitToSDL(axis);
            return m_currentAxisState[slot][sdlAxis];
        }
    }
}