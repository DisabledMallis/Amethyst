#pragma once

//Base class for API features
struct IFeature
{
    virtual ~IFeature() = 0;
    virtual void Setup() const = 0;
};