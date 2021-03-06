#pragma once

#include "Sampler.h"

#pragma  once
class DirectSampler : public Sampler
{
public:
    virtual double sample() const;
    virtual std::vector<double> sample(int num) const;
};