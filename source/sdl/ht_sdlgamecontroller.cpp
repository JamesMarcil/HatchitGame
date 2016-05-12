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

namespace Hatchit
{
    namespace Game
    {
        bool SDLGameController::VButtonHeld(IController::ControllerSlot slot, IController::Buttons button)
        {
            return false; // TODO
        }

        bool SDLGameController::VButtonPressed(IController::ControllerSlot slot, IController::Buttons button)
        {
            return false; // TODO
        }

        bool SDLGameController::VButtonReleased(IController::ControllerSlot slot, IController::Buttons button)
        {
            return false; // TODO
        }

        float SDLGameController::VAxisValue(IController::ControllerSlot slot, IController::Axis axis)
        {
            return 0x0; // TODO
        }

        float SDLGameController::VGetAxisDeadzone(IController::ControllerSlot slot, IController::Axis axis)
        {
            return 0x0; // TODO
        }

        void SDLGameController::VSetAxisDeadzone(IController::ControllerSlot slot, IController::Axis axis, float deadzone)
        {
            /// TODO
        }
    }
}