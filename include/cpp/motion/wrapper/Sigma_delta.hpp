#pragma once
// lahkim - leon

#include <stdlib.h>
#include <nrc2.h>
#include <math.h>

#include <streampu.hpp>
#include "motion/sigma_delta.h"

class Sigma_delta : public spu::module::Stateful {
protected:
	int i0;
	int i1;
	int j0;
	int j1;
	uint8_t vmin;
	uint8_t vmax;
	int n;
	sigma_delta_data_t* sd_data;

public:
	Sigma_delta(const uint8_t** initial_img, const int i0, const int i1, const int j0, const int j1, const uint8_t vmin, const uint8_t vmax, const int n);
	virtual ~Sigma_delta();

};
