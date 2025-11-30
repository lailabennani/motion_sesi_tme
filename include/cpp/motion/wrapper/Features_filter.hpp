#pragma once

#include <streampu.hpp>
#include "motion/features.h"

class Features_filter : public spu::module::Stateful
{
protected:
	const int i0, i1, j0, j1;
	
	const size_t max_RoIs_size;
	RoI_t * RoIs_local;
public:
	Features_filter(const int i0, const int i1, const int j0, const int j1, const size_t max_RoIs_size);
	~Features_filter() = default;
};
