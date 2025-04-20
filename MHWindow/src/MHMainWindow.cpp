/**
 * @file MHMainWindow.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHMainWindow.h"

#include "ui_MHMainWindow.h"

namespace MHWindow {

MHMainWindow::MHMainWindow() : ui(new Ui::MHMainWindow) {
    ui->setupUi(this);
    m_leftDockWidget = new QDockWidget("Left Panel", this);
    m_leftDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    m_leftDockWidget->setTitleBarWidget(new QWidget(m_leftDockWidget));
    m_leftNavigation = new MHLeftNavigation(this);
    m_leftDockWidget->setWidget(m_leftNavigation);
    addDockWidget(Qt::LeftDockWidgetArea, m_leftDockWidget);
    m_vtkWindow = new MHMainVTKWindow(this);
    setCentralWidget(m_vtkWindow);
    m_statusBar = new MHMainWindowStatusBar(statusBar());
    connect(m_statusBar, &MHMainWindowStatusBar::leftNavigationButtonClicked, this, &MHMainWindow::toggleLeftPanel);
}

MHMainWindow::~MHMainWindow() {
    delete m_vtkWindow;
    delete m_leftNavigation;
    delete m_leftDockWidget;
    delete m_statusBar;
    delete ui;
}

void MHMainWindow::toggleLeftPanel() {
    if (m_leftDockWidget->isVisible()) {
        m_leftDockWidget->hide();
    } else {
        m_leftDockWidget->show();
    }
}

}  // namespace MHWindow