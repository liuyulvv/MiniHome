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
    setAutoFillBackground(true);
}

MHRightNavigation::~MHRightNavigation() {
    delete ui;
}

}  // namespace MHWindow
