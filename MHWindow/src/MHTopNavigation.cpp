/**
 * @file MHTopNavigation.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include "MHTopNavigation.h"

#include <QDebug>

#include "MHCylinderManager.h"
#include "MHDrawHouseManager.h"
#include "MHEntityManager.h"
#include "MHHoleManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHPillarManager.h"
#include "MHSpaceManager.h"
#include "MHWallManager.h"
#include "ui_MHTopNavigation.h"

using namespace MHCore;
using namespace MHHouse;

namespace MHWindow {

MHTopNavigation::MHTopNavigation(QWidget *parent) : QWidget(parent), ui(new Ui::MHTopNavigation) {
    ui->setupUi(this);
    setAutoFillBackground(true);
    connect(ui->clearButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "clearButton clicked"; 
        MHDrawHouseManager::getInstance().endDraw();
        MHMainVTKInteractorStyle::getInstance().setSelectedEntity(nullptr);
        MHEntityManager::getInstance().clear();
        MHWallManager::getInstance().clear();
        MHPillarManager::getInstance().clear();
        MHCylinderManager::getInstance().clear();
        MHHoleManager::getInstance().clear();
        MHSpaceManager::getInstance().clear();
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    });
}

MHTopNavigation::~MHTopNavigation() {
    delete ui;
}

}  // namespace MHWindow
