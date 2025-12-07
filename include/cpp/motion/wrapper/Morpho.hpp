#pragma once
// lahkim - leon

#include <math.h>
#include <stdlib.h>
#include <nrc2.h>

#include <streampu.hpp>
#include "motion/morpho.h"

class Morpho : public spu::module::Stateful
{
protected:
	int i0; /* do we  */
	int i1; /* really */ 
	int j0; /* need   */
	int j1; /* this ? */
	morpho_data_t *morpho_data;

	uint8_t **tmp_img;

public:
	Morpho(const int i0, const int i1, const int j0, const int j1);
	virtual ~Morpho();
	virtual Morpho* clone() const;
	virtual void deep_copy(const Morpho& m);
};
