#pragma once

/**
 * @file MHMainWindow.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#ifdef MH_WINDOW_EXPORTS
#define MH_WINDOW_API __declspec(dllexport)
#else
#define MH_WINDOW_API __declspec(dllimport)
#endif

#include <QDockWidget>
#include <QMainWindow>

#include "MHLeftNavigation.h"
#include "MHMainVTKWindow.h"
#include "MHMainWindowStatusBar.h"

namespace Ui {
class MHMainWindow;
}

namespace MHWindow {

class MH_WINDOW_API MHMainWindow : public QMainWindow {
    Q_OBJECT
public:
    MHMainWindow();
    ~MHMainWindow();
    MHMainWindow(const MHMainWindow& window) = delete;
    MHMainWindow(MHMainWindow&& window) = delete;
    MHMainWindow& operator=(const MHMainWindow& window) = delete;
    MHMainWindow& operator=(MHMainWindow&& window) = delete;

public:
    void toggleLeftPanel();

private:
    Ui::MHMainWindow* ui;
    QDockWidget* m_leftDockWidget;
    MHLeftNavigation* m_leftNavigation;
    MHMainVTKWindow* m_vtkWindow;
    MHMainWindowStatusBar* m_statusBar;
};

}  // namespace MHWindow
