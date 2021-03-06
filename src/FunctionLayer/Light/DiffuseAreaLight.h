/**
 * @file DiffuseAreaLight.h
 * @author Zhimin Fan
 * @brief Diffuse Area light.
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "AreaLight.h"
#include "CoreLayer/Geometry/Transform3d.h"
#include "FunctionLayer/Shape/Entity.h"

class DiffuseAreaLight : public AreaLight
{
    std::shared_ptr<Entity> shape;
    Spectrum radiance;

public:
    DiffuseAreaLight(std::shared_ptr<Entity> shape,
                     Spectrum radiance);
    virtual LightSampleResult evalEnvironment(const Ray &ray) override;
	virtual LightSampleResult eval(const Ray& ray, const Intersection &its, const Vec3d &d) override;
    virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) override;
    virtual LightSampleResult sampleDirect(const Intersection &its, const Point2d &sample, float time) override;
};