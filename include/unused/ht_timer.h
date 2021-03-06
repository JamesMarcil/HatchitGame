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

#pragma once

#include <ht_platform.h>

namespace Hatchit {

    namespace Game {

        class HT_API ITimer
        {
        public:
            virtual ~ITimer() { };

            virtual void    VStart() = 0;
            virtual void    VTick() = 0;
            virtual void    VCalculateFPS() = 0;
            virtual float   VDeltaTime() = 0;
            virtual float   VFramesPerSecond() = 0;
            virtual float   VTotalTime() = 0;

        };

    }

}