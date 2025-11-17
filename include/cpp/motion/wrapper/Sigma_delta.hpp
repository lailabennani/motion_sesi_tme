

#pragma once

#include <stdint.h>
#include <streampu.hpp>
#include "motion/sigma_delta_compute.h"

class Sigma_delta : public::module::Stateful {
protected:
	const int i0;
	const int i1;
	const int j0;
	const int j1;
	const sigma_delta_data_t* sd_data;

public:
	Sigma_delta(const int i0, const int i1, const int j0, const int j1, const uint8_t vmin, const uint8_t vmax )


}
