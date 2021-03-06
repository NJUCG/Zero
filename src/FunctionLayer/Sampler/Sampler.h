/**
 * @file Sampler.h
 * @author orbitchen
 * @brief generate random numbers between (0,1).
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <vector>
#include "CoreLayer/Adapter/random.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Math/Common.h"

/**
 * @brief Base class for samplers
 * 
 */
struct CameraSample;

class Sampler
{
protected:
	RandomNumberGenerator rng;
public:
	Sampler() = default;

	virtual ~Sampler() = default;

	virtual void startPixel(const Point2i &pixelPosition) = 0;

	virtual void nextSample() = 0;
	
	virtual double sample1D() = 0;

	virtual Point2d sample2D() = 0;

	CameraSample getCameraSample();

};

/**
 * @brief Shuffle the vector samples
 * 
 * @tparam T 
 * @param samples 
 */
template<typename T>
void shuffle(std::vector<T> &samples, RandomNumberGenerator &rng) {
	int count = samples.size();
	for (int i = 0; i < count; ++i) {
		int other = rng(i, count);
		std::swap(samples[i], samples[other]);
	}
}

/**
 * @brief Base class for pixel sampler, which generate specific samples before rendering each pixel
 *  need the dimensions and samplesPerPixels when construct
 */
class PixelSampler : public Sampler {
protected:
	Point2i pixelPosition;
	std::vector<std::vector<double>>  samples1D;
	std::vector<std::vector<Point2d>> samples2D;
	int sppSqrt;
	int samplesPerPixel;
	int nDimensions;
	int curSamplePixelIndex;
	int curDimensionIndex1D;
	int curDimensionIndex2D;

	virtual void generateSamples1D(std::vector<double> &samples) = 0;
	virtual void generateSamples2D(std::vector<Point2d> &samples) = 0; 

public:
	PixelSampler() = delete;
	
	PixelSampler(int _sppSqrt)
		: sppSqrt(_sppSqrt), samplesPerPixel(_sppSqrt * _sppSqrt), nDimensions(4), curSamplePixelIndex(0), curDimensionIndex1D(0), curDimensionIndex2D(0) {
		for (int i = 0; i < nDimensions; ++i) {
			samples1D.emplace_back(std::vector<double>(samplesPerPixel));
			samples2D.emplace_back(std::vector<Point2d>(samplesPerPixel));
		}
	}
	
	PixelSampler(int _sppSqrt, int _nDimensions)
		: sppSqrt(_sppSqrt) ,samplesPerPixel(_sppSqrt * _sppSqrt), nDimensions(_nDimensions), curSamplePixelIndex(0), curDimensionIndex1D(0), curDimensionIndex2D(0) { 
		for (int i = 0; i < nDimensions; ++i) {
			samples1D.emplace_back(std::vector<double>(samplesPerPixel));
			samples2D.emplace_back(std::vector<Point2d>(samplesPerPixel));
		}
	}
	
	virtual ~PixelSampler() = default;
	
	// @brief The sampler may change the sampling stragety at different pixel location ,record the current pixel location
	// @param _pixelPositon location of current pixel
	virtual void startPixel(const Point2i &_pixelPositon) override {
		// record the position
		pixelPosition = _pixelPositon;
		// reset the sample index
		curDimensionIndex1D = curDimensionIndex2D = curSamplePixelIndex = 0;
		// generate the samples will be used in current pixel
		for (int i = 0; i < nDimensions; ++i) {
			generateSamples1D(samples1D[i]);
			shuffle(samples1D[i], rng);
			generateSamples2D(samples2D[i]);
			shuffle(samples2D[i], rng);
		}
	}

	// @brief Start the next sample at a specific pixel
	virtual void nextSample() override {
		++curSamplePixelIndex;
		curDimensionIndex1D = curDimensionIndex2D = 0;
	}

	// @brief Sample a double in [0, 1], if curDimension exceeds the nDimensions, just return rng()
	// @return A double sample
	virtual double sample1D() override {
		if (curDimensionIndex1D < nDimensions)
			return samples1D[curDimensionIndex1D++][curSamplePixelIndex];
		else
			return rng();
	}

	// @brief Sample a point2d in [0, 1]^2, if curDimension exceeds the nDimensions, just return {rng(), rng()}
	// @return A point2d sample	
	virtual Point2d sample2D() override {
		if (curDimensionIndex2D < nDimensions)
			return samples2D[curDimensionIndex2D++][curSamplePixelIndex];
		else
		 	return Point2d{rng(), rng()};
	}
};
