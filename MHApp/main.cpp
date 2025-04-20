/**
 * @file main.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include <QVTKOpenGLNativeWidget.h>

#include <QApplication>

#include "MHMainWindow.h"

int main(int argc, char* argv[]) {
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    QApplication app(argc, argv);
    MHWindow::MHMainWindow mainWindow;
    mainWindow.showMaximized();
    return app.exec();
}