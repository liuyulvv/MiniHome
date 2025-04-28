/**
 * @file MHRightNavigationSpace.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include "MHRightNavigationSpace.h"

#include "ui_MHRightNavigationSpace.h"

namespace MHWindow {

MHRightNavigationSpace::MHRightNavigationSpace(QWidget *parent) : QWidget(parent), ui(new Ui::MHRightNavigationSpace) {
    ui->setupUi(this);
}

MHRightNavigationSpace::~MHRightNavigationSpace() {
    delete ui;
}

}  // namespace MHWindow
