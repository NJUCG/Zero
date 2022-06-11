/**
 * @file ImageTexture.h
 * @author Zhimin Fan
 * @brief Image texture.
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../Core/CoreLayer/Color.h"
#include <cmath>
#include "../Core/FunctionLayer/Texture.h"
#include "../Core/FunctionLayer/Intersection.h"
#include "../Core/ResourceLayer/Image.h"
#include "Texture.h"
#include "TextureMapping.h"

enum class WrapMode
{
    // todo
};

template <typename T>
class PrefilteredImage
{
protected:
    WrapMode wrapMode;

public:
    virtual void setWrapMode(enum WrapMode _wrapMode);
    virtual WrapMode getWrapMode();
    // todo: other common parameters

    virtual T eval(const TextureCoord2D &coord) = 0;
    virtual T texel(const Point2i &coord) = 0;
    virtual void loadImage(const std::string &filename) = 0;
};

template <typename T>
class DirectImage : public PrefilteredImage<T>
{
protected:
    std::shared_ptr<Image> image;

public:
    virtual void loadImage(const std::string &filename);
    virtual T eval(const TextureCoord2D &coord);
    virtual T texel(const Point2i &coord);
};

template <typename T>
class LinearMIPMap : public PrefilteredImage<T>
{
protected:
    std::shared_ptr<Image> image;

public:
    virtual void loadImage(const std::string &filename);
    virtual T eval(const TextureCoord2D &coord);
    virtual T texel(const Point2i &coord);
};

template <typename Treturn, typename Tmemory>
class ImageTexture : public StdTexture<Treturn, TextureCoord2D>
{
protected:
    std::shared_ptr<PrefilteredImage<Tmemory>> imageSampler;

public:
    ImageTexture(const std::string &filename,
                 std::shared_ptr<TextureMapping2D> mapping = std::make_shared<UVTextureMapping2D>()); // using default sampler
    ImageTexture(const std::string &filename,
                 std::shared_ptr<PrefilteredImage<Tmemory>> imageSampler,
                 std::shared_ptr<TextureMapping2D> mapping = std::make_shared<UVTextureMapping2D>());
    virtual Treturn eval(const TextureCoord2D &coord) const override;
};

// * Example: Creating a Image-based Color Texture using UV coordinates from mesh
// * >  ImageTexture<RGB3>("1.jpg");
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<DirectSampler>());
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<UVTextureMapping2D>());
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<DirectSampler>(), std::make_shared<UVTextureMapping2D>());

// * Example: Create a Image-based Normal Map (wip)
// * since normal cannot be directly interpolated, you need to provide T with some compact NDF type
// * The NDF must provide convertor from RGB3 and some accessors for shadings (depends on implementation of material)

template <typename T>
void PrefilteredImage<T>::setWrapMode(enum WrapMode _wrapMode)
{
    wrapMode = _wrapMode;
}

template <typename T>
WrapMode PrefilteredImage<T>::getWrapMode()
{
    return wrapMode;
}

template <typename T>
void DirectImage<T>::loadImage(const std::string &filename)
{
    image = std::make_shared<Image>(filename);
}

template <typename T>
T DirectImage<T>::texel(const Point2i &coord)
{
    // todo
    return 0.0;
}

template <typename Treturn, typename Tmemory>
ImageTexture<Treturn, Tmemory>::ImageTexture(const std::string &filename,
                                             std::shared_ptr<TextureMapping2D> mapping) : StdTexture(mapping)
{
    imageSampler = std::make_shared<DirectImage<Tmemory>>();
    imageSampler->loadImage(filename);
}

template <typename Treturn, typename Tmemory>
ImageTexture<Treturn, Tmemory>::ImageTexture(const std::string &filename,
                                             std::shared_ptr<PrefilteredImage<Tmemory>> imageSampler,
                                             std::shared_ptr<TextureMapping2D> mapping) : imageSampler(imageSampler), StdTexture(mapping)
{
    this->imageSampler->loadImage(filename);
}
