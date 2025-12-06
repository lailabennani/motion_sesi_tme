#pragma once
// lahkim - leon

#include <streampu.hpp>
#include "motion/kNN.h"

class KNN : public spu::module::Stateful
{
protected:
    kNN_data_t *knn_data;

public:
    KNN(const size_t max_sz, const int k, const uint32_t max_dist, const float min_ratio_S);
    virtual ~KNN();

    kNN_data_t* get_kNN_data() const;

};