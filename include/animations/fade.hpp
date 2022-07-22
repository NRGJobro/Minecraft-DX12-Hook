/***************************************************************************************
 *    imgui_fade_in_out.hpp is a file frome miniDart project
 *    Author : Eric Bachard  / monday april 1st 2020, 20:5133:03 (UTC+0200)
 *    eric.bachard@free.fr
 *    2018 december 30th
 *
 *                                  MIT License
 *                                  -----------
 *                               eric.bachard@free.fr
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * **************************************************************************************/
#ifndef _CUSTOM_FADE_IN_OUT_HPP
#define __CUSTOM_FADE_IN_OUT_HPP

#include "../imgui/imgui.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

namespace md {
	class FadeInOut {
	public:
		FadeInOut();
		~FadeInOut();

		ImGuiIO io;
		void init();

		void set_range(float min, float max) { range = max - min; }
		float get_range(void) { return range; }

		float calculate_step(float f_range, float f_duration) { return ((f_range * io.DeltaTime) / f_duration); }

		float fadeInOut(float up_duration, float down_duration, float min, float max, bool inside_window);
		float heartBeat(float up_hb_duration, float down_hb_duration, float min_hb, float max_hb, bool inside_window_hb);

		bool up_hb_action;

		float up_hb_step;
		float down_hb_step;

	private:
		float range;
	};
} /* namespace md */

#endif /* __CUSTOM_FADE_IN_OUT_HPP */