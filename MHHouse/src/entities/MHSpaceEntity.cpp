/**
 * @file MHSpaceEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHSpaceEntity.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include "MHRendererManager.h"

namespace MHHouse {

MHSpaceEntity::MHSpaceEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHHouseEntity(renderer) {
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
}

void MHSpaceEntity::onDelete() {}

void MHSpaceEntity::updateSpace(const TopoDS_Shape& topo) {
    m_topo = topo;
}

void MHSpaceEntity::generateSpace2D() {
    if (!m_space2DEntity) {
        m_space2DEntity = std::make_shared<MHHouseEntity>(MHCore::MHRendererManager::getInstance().getMain2DRenderer());
        addChild(m_space2DEntity);
    }
    m_space2DEntity->setTopo(m_topo);
    m_space2DEntity->updateTopo();
    m_space2DEntity->setTexture(m_texture);
}

void MHSpaceEntity::generateSpace3D() {
    updateTopo();
    setTexture(m_texture);
}

void MHSpaceEntity::createDefaultTexture() {
    vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> textureReader = readerFactory->CreateImageReader2("textures/default_space.jpeg");
    textureReader->SetFileName("textures/default_space.jpeg");
    textureReader->Update();
    m_texture->SetInputConnection(textureReader->GetOutputPort());
    m_texture->InterpolateOn();
}

}  // namespace MHHouse
