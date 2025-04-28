/**
 * @file MHMainWindow.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHMainWindow.h"

#include <QDebug>

#include "MHDrawHouseManager.h"
#include "ui_MHMainWindow.h"

namespace MHWindow {

MHMainWindow::MHMainWindow() : ui(new Ui::MHMainWindow) {
    ui->setupUi(this);
    m_vtkWindow = new MHMainVTKWindow(this);
    m_hLayout = new QHBoxLayout(m_vtkWindow);
    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_leftNavigation = new MHLeftNavigation();
    m_rightNavigation = new MHRightNavigation();
    m_hLayout->addWidget(m_leftNavigation);
    m_hLayout->addStretch();
    m_hLayout->addWidget(m_rightNavigation);
    m_statusBar = new MHMainWindowStatusBar(statusBar());
    setCentralWidget(m_vtkWindow);
    connect(m_statusBar, &MHMainWindowStatusBar::leftNavigationButtonClicked, this, &MHMainWindow::toggleLeftPanel);
    connect(m_statusBar, &MHMainWindowStatusBar::rightNavigationButtonClicked, this, &MHMainWindow::toggleRightPanel);
    MHHouse::MHDrawHouseManager::getInstance().setShowRightPanelCallback([this](bool show, MHHouse::MHDrawType drawType) { showRightPanelDraw(show, drawType); });
}

MHMainWindow::~MHMainWindow() {
    delete m_leftNavigation;
    delete m_rightNavigation;
    delete m_vtkWindow;
    delete m_statusBar;
    delete ui;
}

void MHMainWindow::showRightPanel(bool show) {
    if (show) {
        m_rightNavigation->show();
    } else {
        m_rightNavigation->hide();
    }
}

void MHMainWindow::showRightPanelDraw(bool show, MHHouse::MHDrawType drawType) {
    qDebug() << m_rightNavigation->width();
    switch (drawType) {
        case MHHouse::MHDrawType::WALL_LINE:
        case MHHouse::MHDrawType::WALL_RECTANGLE:
        case MHHouse::MHDrawType::WALL_ARC:
            m_rightNavigation->showRightNavigationDrawWall(show, drawType);
            break;
        default:
            break;
    }
}

void MHMainWindow::toggleLeftPanel() {
    if (m_leftNavigation->isVisible()) {
        m_leftNavigation->hide();
    } else {
        m_leftNavigation->show();
    }
}

void MHMainWindow::toggleRightPanel() {
    if (m_rightNavigation->isVisible()) {
        m_rightNavigation->hide();
    } else {
        m_rightNavigation->show();
    }
}

}  // namespace MHWindow