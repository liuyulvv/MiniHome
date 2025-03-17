#pragma once

/**
 * @file MHVtkWindow.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#ifdef MH_WINDOW_EXPORTS
#define MH_WINDOW_API __declspec(dllexport)
#else
#define MH_WINDOW_API __declspec(dllimport)
#endif

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>

#include "MHRenderer.h"

namespace MHWindow {

class MH_WINDOW_API MHVtkWindow : public QVTKOpenGLNativeWidget {
public:
    explicit MHVtkWindow(QWidget* parent = nullptr);
    ~MHVtkWindow();
    MHVtkWindow(const MHVtkWindow& window) = delete;
    MHVtkWindow(MHVtkWindow&& window) = delete;
    MHVtkWindow& operator=(const MHVtkWindow& window) = delete;
    MHVtkWindow& operator=(MHVtkWindow&& window) = delete;

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_window;
    vtkSmartPointer<MHCore::MHRenderer> m_renderer;
};

}  // namespace MHWindow
