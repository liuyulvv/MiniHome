/**
 * @file MHRightNavigationDrawWall.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include "MHRightNavigationDrawWall.h"

#include "ui_MHRightNavigationDrawWall.h"

namespace MHWindow {

MHRightNavigationDrawWall::MHRightNavigationDrawWall(QWidget *parent) : QWidget(parent), ui(new Ui::MHRightNavigationDrawWall) {
    ui->setupUi(this);
}

MHRightNavigationDrawWall::~MHRightNavigationDrawWall() {
    delete ui;
}

void MHRightNavigationDrawWall::setDrawType(MHHouse::MHDrawType drawType) {
    m_drawType = drawType;
}

}  // namespace MHWindow
