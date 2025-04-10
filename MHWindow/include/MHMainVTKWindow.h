#pragma once

/**
 * @file MHMainVTKWindow.h
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

#include "MHInteractor.h"

namespace MHWindow {

class MH_WINDOW_API MHMainVTKWindow : public QVTKOpenGLNativeWidget {
public:
    explicit MHMainVTKWindow(QWidget* parent = nullptr);
    ~MHMainVTKWindow();
    MHMainVTKWindow(const MHMainVTKWindow& window) = delete;
    MHMainVTKWindow(MHMainVTKWindow&& window) = delete;
    MHMainVTKWindow& operator=(const MHMainVTKWindow& window) = delete;
    MHMainVTKWindow& operator=(MHMainVTKWindow&& window) = delete;

private:
    void createGround();

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_window;
    vtkSmartPointer<MHCore::MHInteractor> m_interactor;
};

}  // namespace MHWindow
