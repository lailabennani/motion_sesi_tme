#pragma once

#include <streampu.hpp>
#include "motion/kNN.h"

class KNN : public spu::module::Stateful
{
protected:
    kNN_data_t knn_data;

public:
    KNN(const size_t max_size) : spu::module::Stateful() {};
    virtual ~KNN();

};