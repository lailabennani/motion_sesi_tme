#pragma once

#include "streampu.hpp"
#include "motion/CCL.h"

class CCL : public spu::module::Stateful {

protected:
	int i0;
	int i1;
	int j0;
	int j1;

	CCL_data_t * CCL_data;
	
public:
	CCL(const int i0, const int i1, const int j0, const int j1);

	virtual ~CCL();
};
