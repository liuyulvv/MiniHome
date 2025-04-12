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
#include <string>
#include <vector>

#include "MHActor.h"
#include "MHRenderer.h"
#include "MHVertex.h"

namespace MHCore {

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

public:
    const std::string& getId() const;
    void move(double x, double y, double z);
    void scale(double x, double y, double z);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
    vtkSmartPointer<vtkTransform> getGlobalTransform() const;
    vtkSmartPointer<vtkTransform> getLocaltransform() const;
    std::vector<std::shared_ptr<MHEntity>> getChildren() const;
    void addChild(std::shared_ptr<MHEntity> child);
    void setTopo(std::unique_ptr<MHGeometry::MHTopoBase> topo);

protected:
    void createDefaultTexture();
    void applyTransform();

protected:
    vtkSmartPointer<MHRenderer> m_renderer;
    std::string m_id;
    vtkSmartPointer<MHActor> m_actor;
    vtkSmartPointer<vtkTexture> m_texture;
    std::unique_ptr<MHGeometry::MHTopoBase> m_topo = nullptr;
    std::weak_ptr<MHEntity> m_parent;
    std::vector<std::shared_ptr<MHEntity>> m_children;
    vtkSmartPointer<vtkTransform> m_localTransform;
};

}  // namespace MHCore
