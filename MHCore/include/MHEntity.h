#pragma once

/**
 * @file MHEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include <vtkImageData.h>
#include <vtkTexture.h>
#include <vtkTransform.h>

#include <array>
#include <set>
#include <string>
#include <vector>

#include "MHActor.h"
#include "MHRenderer.h"
#include "MHVertex.h"

namespace MHCore {

enum class MHEntityLayerMask : unsigned int {
    ALL = 0xFFFFFFFF,
    NONE = 0x00000000,
    LAYER_2D = 0x00000001,
    LAYER_3D = 0x00000010,
};

inline unsigned int operator&(MHEntityLayerMask a, MHEntityLayerMask b) {
    return static_cast<unsigned int>(a) & static_cast<unsigned int>(b);
}

struct MHEntityInteractorInfo {
    bool selected = false;
    bool hovered = false;
    std::set<std::string> keyPressed;
};

class MH_CORE_API MHEntity : public std::enable_shared_from_this<MHEntity> {
public:
    explicit MHEntity(vtkSmartPointer<MHRenderer> renderer = nullptr);
    virtual ~MHEntity();
    MHEntity(const MHEntity& entity) = delete;
    MHEntity(MHEntity&& entity) = delete;
    MHEntity& operator=(const MHEntity& entity) = delete;
    MHEntity& operator=(MHEntity&& entity) = delete;

public:
    virtual void show(bool forceRender = true);
    virtual void destroy();
    virtual void onEnter();
    virtual void onLeave();
    virtual void onSelected(const MHEntityInteractorInfo& info);

public:
    const std::string& getId() const;
    bool isSame(std::shared_ptr<MHEntity> entity) const;
    void move(double x, double y, double z);
    void scale(double x, double y, double z);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
    vtkSmartPointer<vtkTransform> getGlobalTransform() const;
    vtkSmartPointer<vtkTransform> getLocaltransform() const;
    std::weak_ptr<MHEntity> getParent() const;
    std::vector<std::shared_ptr<MHEntity>> getChildren() const;
    void addChild(std::shared_ptr<MHEntity> child);
    void setTexture(vtkSmartPointer<vtkTexture> texture);
    void enableLayerMask(MHEntityLayerMask layerMask);
    void disableLayerMask(MHEntityLayerMask layerMask);
    void setLayerMask(MHEntityLayerMask layerMask);
    MHEntityLayerMask getLayerMask() const;

protected:
    void createDefaultTexture();
    void applyTransform();

protected:
    vtkSmartPointer<MHRenderer> m_renderer;
    std::string m_id;
    vtkSmartPointer<MHActor> m_actor;
    vtkSmartPointer<vtkTexture> m_texture;
    vtkSmartPointer<vtkTexture> m_selectedTexture;
    vtkSmartPointer<vtkTexture> m_hoveredTexture;
    std::weak_ptr<MHEntity> m_parent;
    std::vector<std::shared_ptr<MHEntity>> m_children;
    vtkSmartPointer<vtkTransform> m_localTransform;
    MHEntityLayerMask m_layerMask = MHEntityLayerMask::ALL;
    bool m_selected = false;
    bool m_hovered = false;
};

}  // namespace MHCore
