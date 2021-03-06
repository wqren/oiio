/*
  Copyright 2013 Larry Gritz and the other authors and contributors.
  All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of the software's owners nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  (This is the Modified BSD License)
*/

#include <cstdio>
#include <cstdlib>

#include "OpenImageIO/timer.h"


OIIO_NAMESPACE_ENTER
{

double Timer::seconds_per_tick = 1.0e-6;


class TimerSetupOnce {
public:
    TimerSetupOnce () {
#ifdef _WIN32
        // From MSDN web site
        LARGE_INTEGER freq;
        QueryPerformanceFrequency (&freq);
        Timer::seconds_per_tick = 1.0 / (double)freq.QuadPart;
#elif defined(__APPLE__)
        // NOTE(boulos): Both this value and that of the windows
        // counterpart above only need to be calculated once. In
        // Manta, we stored this on the side as a scaling factor but
        // that would require a .cpp file (meaning timer.h can't just
        // be used as a header). It is also claimed that since
        // Leopard, Apple returns 1 for both numer and denom.
        mach_timebase_info_data_t time_info;
        mach_timebase_info(&time_info);
        Timer::seconds_per_tick = (1e-9*static_cast<double>(time_info.numer))/
                                       static_cast<double>(time_info.denom);
#endif
    }
};

static TimerSetupOnce once;

}
OIIO_NAMESPACE_EXIT
