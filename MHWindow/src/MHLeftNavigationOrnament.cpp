/**
 * @file MHLeftNavigationOrnament.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include "MHLeftNavigationOrnament.h"

#include "ui_MHLeftNavigationOrnament.h"

namespace MHWindow {

MHLeftNavigationOrnament::MHLeftNavigationOrnament(QWidget* parent) : QWidget(parent), ui(new Ui::MHLeftNavigationOrnament) {
    ui->setupUi(this);
}

MHLeftNavigationOrnament::~MHLeftNavigationOrnament() {
    delete ui;
}

}  // namespace MHWindow
