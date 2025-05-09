/**
 * @file MHEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHEntity.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "MHActor.h"
#include "MHEntityManager.h"
#include "MHRendererManager.h"

namespace MHCore {

MHEntity::MHEntity(vtkSmartPointer<MHRenderer> renderer) : m_renderer(renderer) {
    if (!m_renderer) {
        m_renderer = MHRendererManager::getInstance().getMain3DRenderer();
    }
    m_localTransform = vtkSmartPointer<vtkTransform>::New();
    m_localTransform->Identity();
    boost::uuids::time_generator_v7 generator;
    auto uuid = generator();
    m_id = boost::uuids::to_string(uuid);
    m_actor = vtkSmartPointer<MHActor>::New();
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
}

MHEntity::~MHEntity() {
    m_renderer->RemoveActor(m_actor);
    m_actor = nullptr;
    m_renderer = nullptr;
    m_children.clear();
}

void MHEntity::show(bool forceRender) {
    m_shown = true;
    m_renderer->AddActor(m_actor);
    m_actor->setEntity(shared_from_this());
    for (const auto& child : m_children) {
        if (child) {
            child->show(false);
        }
    }
    if (forceRender) {
        MHRendererManager::getInstance().getActiveMainRenderer()->render();
    }
}

void MHEntity::destroy() {
    MHEntityManager::getInstance().removeEntity(m_id);
    m_renderer->RemoveActor(m_actor);
    m_actor->setEntity(nullptr);
    for (const auto& child : m_children) {
        if (child) {
            child->destroy();
        }
    }
}

void MHEntity::onEnter(const MHEntityInteractorInfo& info) {
    m_hovered = true;
}

void MHEntity::onLeave(const MHEntityInteractorInfo& info) {
    m_hovered = false;
}

void MHEntity::onSelected(const MHEntityInteractorInfo& info) {
    m_selected = info.selected;
}

void MHEntity::onDelete() {
    destroy();
}

const std::string& MHEntity::getId() const {
    return m_id;
}

bool MHEntity::isSame(std::shared_ptr<MHEntity> entity) const {
    if (entity) {
        return m_id == entity->getId();
    }
    return false;
}

void MHEntity::move(double x, double y, double z) {
    m_localTransform->Translate(x, y, z);
    applyTransform();
}

void MHEntity::scale(double x, double y, double z) {
    m_localTransform->Scale(x, y, z);
    applyTransform();
}

void MHEntity::rotateX(double angle) {
    m_localTransform->RotateX(angle);
    applyTransform();
}

void MHEntity::rotateY(double angle) {
    m_localTransform->RotateY(angle);
    applyTransform();
}

void MHEntity::rotateZ(double angle) {
    m_localTransform->RotateZ(angle);
    applyTransform();
}

vtkSmartPointer<vtkTransform> MHEntity::getGlobalTransform() const {
    vtkSmartPointer<vtkTransform> globalTransform = vtkSmartPointer<vtkTransform>::New();
    globalTransform->DeepCopy(m_localTransform);
    if (auto parent = m_parent.lock()) {
        vtkSmartPointer<vtkTransform> parentTransform = parent->getGlobalTransform();
        parentTransform->Concatenate(globalTransform);
        return parentTransform;
    }
    return globalTransform;
}

vtkSmartPointer<vtkTransform> MHEntity::getLocaltransform() const {
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->DeepCopy(m_localTransform);
    return transform;
}

std::weak_ptr<MHEntity> MHEntity::getParent() const {
    return m_parent;
}

std::vector<std::shared_ptr<MHEntity>> MHEntity::getChildren() const {
    return m_children;
}

void MHEntity::addChild(std::shared_ptr<MHEntity> child) {
    if (child) {
        child->m_parent = shared_from_this();
        m_children.push_back(child);
    }
}

void MHEntity::setTexture(vtkSmartPointer<vtkTexture> texture) {
    m_texture = texture;
    m_actor->SetTexture(m_texture);
}

void MHEntity::enableLayerMask(MHEntityLayerMask layerMask) {
    m_layerMask = static_cast<MHEntityLayerMask>(static_cast<unsigned int>(m_layerMask) | static_cast<unsigned int>(layerMask));
}

void MHEntity::disableLayerMask(MHEntityLayerMask layerMask) {
    m_layerMask = static_cast<MHEntityLayerMask>(static_cast<unsigned int>(m_layerMask) & ~static_cast<unsigned int>(layerMask));
}

void MHEntity::setLayerMask(MHEntityLayerMask layerMask) {
    m_layerMask = layerMask;
}

MHEntityLayerMask MHEntity::getLayerMask() const {
    return m_layerMask;
}

vtkSmartPointer<MHActor> MHEntity::getActor() const {
    return m_actor;
}

bool MHEntity::isPickable() const {
    return m_pickable;
}

void MHEntity::setPickable(bool pickable) {
    m_pickable = pickable;
}

int MHEntity::getPickerOrder() const {
    return m_pickerOrder;
}

void MHEntity::setPickerOrder(int order) {
    m_pickerOrder = order;
    for (auto& child : m_children) {
        if (child) {
            child->setPickerOrder(order);
        }
    }
}

void MHEntity::createDefaultTexture() {
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->SetDimensions(1, 1, 1);
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
    unsigned char* pixel = static_cast<unsigned char*>(imageData->GetScalarPointer(0, 0, 0));
    pixel[0] = 0;
    pixel[1] = 255;
    pixel[2] = 255;
    m_texture = vtkSmartPointer<vtkTexture>::New();
    m_texture->SetInputData(imageData);
    m_texture->InterpolateOn();

    vtkSmartPointer<vtkImageReader2Factory> selectReaderFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> selectTextureReader = selectReaderFactory->CreateImageReader2("textures/default_select.png");
    selectTextureReader->SetFileName("textures/default_select.png");
    selectTextureReader->Update();
    m_selectedTexture = vtkSmartPointer<vtkTexture>::New();
    m_selectedTexture->SetInputConnection(selectTextureReader->GetOutputPort());
    m_selectedTexture->InterpolateOn();

    vtkSmartPointer<vtkImageReader2Factory> hoverReaderFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> hoverTextureReader = selectReaderFactory->CreateImageReader2("textures/default_hover.png");
    hoverTextureReader->SetFileName("textures/default_hover.png");
    hoverTextureReader->Update();
    m_hoveredTexture = vtkSmartPointer<vtkTexture>::New();
    m_hoveredTexture->SetInputConnection(hoverTextureReader->GetOutputPort());
    m_hoveredTexture->InterpolateOn();
}

void MHEntity::applyTransform() {
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    if (auto parent = m_parent.lock()) {
        vtkSmartPointer<vtkTransform> parentTransform = parent->getGlobalTransform();
        transform->Concatenate(parentTransform);
    }
    transform->Concatenate(m_localTransform);
    m_actor->SetUserTransform(transform);
}

}  // namespace MHCore
