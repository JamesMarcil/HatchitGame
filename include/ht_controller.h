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

#include <cstdint>

namespace Hatchit
{
    namespace Game
    {
        class HT_API IController
        {
        public:
            enum class Buttons
            {
                UP,
                DOWN,
                LEFT,
                RIGHT,
                A,
                B,
                X,
                Y,
                LEFT_BUMPER,
                RIGHT_BUMPER,
                LEFT_STICK,
                RIGHT_STICK,
                START,
                BACK
            };

            enum class Axis
            {
                LEFT_AXIS_X,
                LEFT_AXIS_Y,
                RIGHT_AXIS_X,
                RIGHT_AXIS_Y,
                LEFT_TRIGGER,
                RIGHT_TRIGGER
            };

            enum class ControllerSlot
            {
                ONE,
                TWO,
                THREE,
                FOUR
            };

            IController(void) = default;
            virtual ~IController(void) = default;
            IController(const IController& rhs) = default;
            IController(IController&& rhs) = default;
            IController& operator=(const IController& rhs) = default;
            IController& operator=(IController&& rhs) = default;

            virtual void VInitialize(void) = 0;
            virtual void VDeInitialize(void) = 0;
            virtual void VUpdate(void) = 0;
            virtual bool VButtonHeld(ControllerSlot slot, Buttons button) = 0;
            virtual bool VButtonPressed(ControllerSlot slot, Buttons button) = 0;
            virtual bool VButtonReleased(ControllerSlot slot, Buttons button) = 0;
            virtual std::int16_t VAxisValue(ControllerSlot slot, Axis axis) = 0;
        };
    }
}
