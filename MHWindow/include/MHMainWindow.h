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

#include <QGridLayout>
#include <QHBoxLayout>
#include <QMainWindow>

#include "MHDrawBase.h"
#include "MHLeftNavigation.h"
#include "MHMainVTKWindow.h"
#include "MHMainWindowStatusBar.h"
#include "MHRightNavigation.h"
#include "MHTopNavigation.h"

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

private:
    void showRightPanel(bool show);
    void showRightPanelDraw(bool show, MHHouse::MHDrawType drawType);
    void toggleLeftPanel();
    void toggleRightPanel();

private:
    Ui::MHMainWindow* ui;
    MHTopNavigation* m_topNavigation;
    MHLeftNavigation* m_leftNavigation;
    MHRightNavigation* m_rightNavigation;
    MHMainVTKWindow* m_vtkWindow;
    QHBoxLayout* m_hLayout;
    QGridLayout* m_vtkLayout;
    MHMainWindowStatusBar* m_statusBar;
};

}  // namespace MHWindow
