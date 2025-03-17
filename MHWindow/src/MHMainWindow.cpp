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
    m_vtkWindow = new MHVtkWindow(this);
    setCentralWidget(m_vtkWindow);
}

MHMainWindow::~MHMainWindow() {
    delete m_vtkWindow;
    delete m_leftNavigation;
    delete m_leftDockWidget;
    delete ui;
}

}  // namespace MHWindow