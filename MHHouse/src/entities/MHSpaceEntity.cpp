/**
 * @file MHSpaceEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHSpaceEntity.h"

namespace MHHouse {

MHSpaceEntity::MHSpaceEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHHouseEntity(renderer) {
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
}

void MHSpaceEntity::createDefaultTexture() {
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->SetDimensions(1, 1, 1);
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
    unsigned char* pixel = static_cast<unsigned char*>(imageData->GetScalarPointer(0, 0, 0));
    pixel[0] = 245;
    pixel[1] = 245;
    pixel[2] = 245;
    m_texture = vtkSmartPointer<vtkTexture>::New();
    m_texture->SetInputData(imageData);
    m_texture->InterpolateOn();
}

}  // namespace MHHouse
