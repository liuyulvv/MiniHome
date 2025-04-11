#pragma once

/**
 * @file MHMainVTKRenderWindow.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <vtkGenericOpenGLRenderWindow.h>

namespace MHWindow {

class MHMainVTKRenderWindow : public vtkGenericOpenGLRenderWindow {
public:
    static MHMainVTKRenderWindow* New() {
        return new MHMainVTKRenderWindow();
    }

    vtkTypeMacro(MHMainVTKRenderWindow, vtkGenericOpenGLRenderWindow);
    MHMainVTKRenderWindow(const MHMainVTKRenderWindow& window) = delete;
    MHMainVTKRenderWindow(MHMainVTKRenderWindow&& window) = delete;
    MHMainVTKRenderWindow& operator=(const MHMainVTKRenderWindow& window) = delete;
    MHMainVTKRenderWindow& operator=(MHMainVTKRenderWindow&& window) = delete;

private:
    MHMainVTKRenderWindow() = default;
    ~MHMainVTKRenderWindow() = default;

public:
    virtual void Render() override;
};

}  // namespace MHWindow