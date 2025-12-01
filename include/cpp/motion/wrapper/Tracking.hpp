#pragma once

#include <streampu.hpp>
#include "motion/tracking.h"

class Tracking : public spu::module::Stateful
{
protected:
	
	const size_t r_extrapol;
       	const size_t fra_obj_min;
        const uint8_t save_RoIs_id;
	const uint8_t extrapol_order_max;
       	const float min_extrapol_ratio_S;
	
	const size_t max_history_size;
	const size_t max_RoIs_size;

	tracking_data_t *tracking_data;

public:
	Tracking(const size_t r_extrapol, const size_t fra_obj_min, const uint8_t save_RoIs_id, const uint8_t extrapol_order_max,
		 const float min_extrapol_ratio_S, const size_t max_history_size, const size_t max_RoIs_size);

	~Tracking();

	tracking_data_t *get_tracking_data() const;
};

