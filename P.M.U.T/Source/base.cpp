#include "Headers/base.h"

namespace pmut
{

	void timer::reset_frametimes()
	{
		shortest_frame_time = 100.0L;
		largest_frame_time = 0.0L;
	}

	void timer::start_timer()
	{
		start = clock::now();
	}

	void timer::end_timer()
	{
		end = clock::now();
		duration = end - start;
		timer_byte ms = duration.count() * 1000;
		current_frame_time = ms;
		if (current_frame_time > largest_frame_time)
			largest_frame_time = current_frame_time;
		else if (current_frame_time < shortest_frame_time)
			shortest_frame_time = current_frame_time;

		console::vars::pmut_console->print(3, false, console::vec3(0.0f, 0.6f, 0.6f), PMUT_TIMER, " frametime: ", current_frame_time, "ms");
		console::vars::pmut_console->print(4, false, console::vec3(0.0f, 0.6f, 0.6f), PMUT_TIMER, " shortest frametime: ", shortest_frame_time, "ms");
		console::vars::pmut_console->print(5, false, console::vec3(0.0f, 0.6f, 0.6f), PMUT_TIMER, " largest frametime: ", largest_frame_time, "ms ");
	}
}