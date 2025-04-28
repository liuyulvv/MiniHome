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

#include <QHBoxLayout>
#include <QMainWindow>

#include "MHLeftNavigation.h"
#include "MHMainVTKWindow.h"
#include "MHMainWindowStatusBar.h"
#include "MHRightNavigation.h"

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
    void toggleRightPanel();

private:
    Ui::MHMainWindow* ui;
    MHLeftNavigation* m_leftNavigation;
    MHRightNavigation* m_rightNavigation;
    MHMainVTKWindow* m_vtkWindow;
    QHBoxLayout* m_hLayout;
    MHMainWindowStatusBar* m_statusBar;
};

}  // namespace MHWindow
