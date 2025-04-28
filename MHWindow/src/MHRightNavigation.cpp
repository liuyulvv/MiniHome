/**
 * @file MHRightNavigation.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-24
 */

#include "MHRightNavigation.h"

#include "ui_MHRightNavigation.h"

namespace MHWindow {

MHRightNavigation::MHRightNavigation(QWidget *parent) : QWidget(parent), ui(new Ui::MHRightNavigation) {
    ui->setupUi(this);
    m_rightNavigationSpace = new MHRightNavigationSpace(this);
    m_rightNavigationDrawWall = new MHRightNavigationDrawWall(this);
    ui->stackedWidget->addWidget(m_rightNavigationSpace);
    ui->stackedWidget->addWidget(m_rightNavigationDrawWall);
    ui->stackedWidget->setCurrentIndex(0);
    setAutoFillBackground(true);
}

MHRightNavigation::~MHRightNavigation() {
    delete ui;
}

void MHRightNavigation::showRightNavigationDrawWall(bool show, MHHouse::MHDrawType drawType) {
    if (show) {
        m_rightNavigationDrawWall->setDrawType(drawType);
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

}  // namespace MHWindow
