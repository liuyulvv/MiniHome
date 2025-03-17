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
    m_vtkWindow = new MHVtkWindow();
    setCentralWidget(m_vtkWindow);
}

MHMainWindow::~MHMainWindow() {
    delete m_vtkWindow;
    delete ui;
}

}  // namespace MHWindow