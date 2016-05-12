/**
**    Hatchit Engine
**    Copyright(c) 2015-2016 Third-Degree
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

#pragma once

#include <ht_platform.h>
#include <ht_controller.h>
#include <ht_sdl.h>

#include <stack>
#include <bitset>
#include <unordered_map>
#include <cstdint>

namespace Hatchit {
    namespace Game {
        class HT_API SDLGameController : public IController
        {
            using ControllerButtonState = std::bitset<SDL_CONTROLLER_BUTTON_MAX>;
            using ControllerAxisState = std::unordered_map<SDL_GameControllerAxis, std::int16_t>;
        public:
            SDLGameController(void) = default;
            virtual ~SDLGameController(void) = default;
            SDLGameController(const SDLGameController& rhs) = default;
            SDLGameController(SDLGameController&& rhs) = default;
            SDLGameController& operator=(const SDLGameController& rhs) = default;
            SDLGameController& operator=(SDLGameController&& rhs) = default;

            bool AttachController(int joystickDeviceIndex);
            bool DetachController(SDL_JoystickID joystickInstanceId);
            bool IsControllerAttached(IController::ControllerSlot slot);

            void ButtonPressed(SDL_JoystickID joystickInstanceId, SDL_GameControllerButton button);
            void ButtonReleased(SDL_JoystickID joystickInstanceId, SDL_GameControllerButton button);

            void AxisMotion(SDL_JoystickID joystickInstanceId, SDL_GameControllerAxis axis, std::int16_t value);

            SDL_GameControllerButton ConvertButtonFromHatchitToSDL(IController::Buttons button);
            SDL_GameControllerAxis ConvertAxisFromHatchitToSDL(IController::Axis axis);

            virtual void VInitialize(void) override;
            virtual void VDeInitialize(void) override;
            virtual void VUpdate(void) override;
            virtual bool VButtonHeld(IController::ControllerSlot slot, IController::Buttons button) override;
            virtual bool VButtonPressed(IController::ControllerSlot slot, IController::Buttons button) override;
            virtual bool VButtonReleased(IController::ControllerSlot slot, IController::Buttons button) override;
            virtual std::int16_t VAxisValue(IController::ControllerSlot slot, IController::Axis axis) override;
        private:
            std::stack<IController::ControllerSlot> m_freeControllerSlots;
            std::unordered_map<SDL_JoystickID, IController::ControllerSlot> m_idToControllerSlot;
            std::unordered_map<IController::ControllerSlot, SDL_GameController*> m_connectedGamepads;
            std::unordered_map<IController::ControllerSlot, ControllerButtonState> m_currentButtonState;
            std::unordered_map<IController::ControllerSlot, ControllerButtonState> m_previousButtonState;
            std::unordered_map<IController::ControllerSlot, ControllerAxisState> m_currentAxisState;
        };
    }
}
