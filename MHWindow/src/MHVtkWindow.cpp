/**
 * @file MHVtkWindow.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHVtkWindow.h"

namespace MHWindow {

MHVtkWindow::MHVtkWindow(QWidget* parent) : QVTKOpenGLNativeWidget(parent) {
    m_window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_renderer = vtkSmartPointer<MHCore::MHRenderer>::New();
    m_window->AddRenderer(m_renderer);
    setRenderWindow(m_window);
}

MHVtkWindow::~MHVtkWindow() {
    m_renderer = nullptr;
    m_window = nullptr;
}

}  // namespace MHWindow