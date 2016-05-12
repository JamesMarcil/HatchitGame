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

namespace Hatchit {
    namespace Game {
        class HT_API SDLGameController : public IController
        {
        public:
            SDLGameController(void) = default;
            virtual ~SDLGameController(void) = default;
            SDLGameController(const SDLGameController& rhs) = default;
            SDLGameController(SDLGameController&& rhs) = default;
            SDLGameController& operator=(const SDLGameController& rhs) = default;
            SDLGameController& operator=(SDLGameController&& rhs) = default;

            virtual bool VButtonHeld(IController::ControllerSlot slot, IController::Buttons button) override;
            virtual bool VButtonPressed(IController::ControllerSlot slot, IController::Buttons button) override;
            virtual bool VButtonReleased(IController::ControllerSlot slot, IController::Buttons button) override;
            virtual float VAxisValue(IController::ControllerSlot slot, IController::Axis axis) override;
            virtual float VGetAxisDeadzone(IController::ControllerSlot slot, IController::Axis axis) override;
            virtual void VSetAxisDeadzone(IController::ControllerSlot slot, IController::Axis axis, float deadzone) override;
        protected:
        private:
        };
    }
}
