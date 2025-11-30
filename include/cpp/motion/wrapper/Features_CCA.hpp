#pragma once
// lahkim leon

#include <streampu.hpp>
#include "motion/features.h"

class Features_CCA : public spu::module::Stateful // or Stateless ?
{
protected:
        const int i0, i1, j0, j1;
        const size_t max_sz;
public:
        Features_CCA(const int i0, const int i1, const int j0, const int j1, const size_t max_RoIs_size);
        virtual ~Features_CCA() = default; // todo: verify
};