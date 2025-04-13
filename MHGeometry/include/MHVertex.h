#pragma once

/**
 * @file MHVertex.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#ifdef MH_GEOMETRY_EXPORTS
#define MH_GEOMETRY_API __declspec(dllexport)
#else
#define MH_GEOMETRY_API __declspec(dllimport)
#endif

#include <vtkSmartPointer.h>
#include <vtkTransform.h>

namespace MHGeometry {

enum class MHTopoType {
    VERTEX,
    EDGE,
    WIRE,
    FACE,
};

enum class MHTopoOrientation {
    NONE,
    CLOCK_WISE,
    COUNTER_CLOCK_WISE,
};

class MH_GEOMETRY_API MHTopoBase {
public:
    MHTopoBase() = default;
    virtual ~MHTopoBase() = default;
    MHTopoBase(const MHTopoBase& base) = default;
    MHTopoBase(MHTopoBase&& base) = default;
    MHTopoBase& operator=(const MHTopoBase& base) = default;
    MHTopoBase& operator=(MHTopoBase&& base) = default;

public:
    virtual MHTopoType getType() const = 0;
    virtual std::unique_ptr<MHTopoBase> clone() const = 0;
    virtual void applyTransform(vtkSmartPointer<vtkTransform> transform) = 0;
};

class MH_GEOMETRY_API MHVertex : public MHTopoBase {
public:
    MHVertex() = default;
    MHVertex(double x, double y, double z);
    ~MHVertex() = default;
    MHVertex(const MHVertex& vertex) = default;
    MHVertex(MHVertex&& vertex) = default;
    MHVertex& operator=(const MHVertex& vertex) = default;
    MHVertex& operator=(MHVertex&& vertex) = default;

public:
    virtual MHTopoType getType() const override;
    virtual std::unique_ptr<MHTopoBase> clone() const override;
    virtual void applyTransform(vtkSmartPointer<vtkTransform> transform) override;

public:
    MHVertex operator+(const MHVertex& vertex) const;
    MHVertex operator-(const MHVertex& vertex) const;
    MHVertex operator*(double scalar) const;
    MHVertex operator/(double scalar) const;
    MHVertex operator-() const;
    MHVertex normalize() const;
    MHVertex cross(const MHVertex& vertex) const;
    double dot(const MHVertex& vertex) const;
    double length() const;
    bool isParallel(const MHVertex& vertex) const;

public:
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

}  // namespace MHGeometry
