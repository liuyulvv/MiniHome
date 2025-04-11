#pragma once

/**
 * @file MHVertex.hpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#ifdef MH_GEOMETRY_EXPORTS
#define MH_GEOMETRY_API __declspec(dllexport)
#else
#define MH_GEOMETRY_API __declspec(dllimport)
#endif

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

class MHTopoBase {
public:
    MHTopoBase() = default;
    virtual ~MHTopoBase() = default;
    MHTopoBase(const MHTopoBase& base) = default;
    MHTopoBase(MHTopoBase&& base) = default;
    MHTopoBase& operator=(const MHTopoBase& base) = default;
    MHTopoBase& operator=(MHTopoBase&& base) = default;

public:
    virtual MHTopoType getType() const = 0;
    virtual MHTopoBase* clone() const = 0;
};

class MH_GEOMETRY_API MHVertex : public MHTopoBase {
public:
    MHVertex() = default;
    MHVertex(double x, double y, double z) : x(x), y(y), z(z) {}
    ~MHVertex() = default;
    MHVertex(const MHVertex& vertex) = default;
    MHVertex(MHVertex&& vertex) = default;
    MHVertex& operator=(const MHVertex& vertex) = default;
    MHVertex& operator=(MHVertex&& vertex) = default;

public:
    virtual MHTopoType getType() const override {
        return MHTopoType::VERTEX;
    }

    virtual MHTopoBase* clone() const override {
        return new MHVertex(this->x, this->y, this->z);
    }

public:
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

}  // namespace MHGeometry
