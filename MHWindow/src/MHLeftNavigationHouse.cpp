/**
 * @file MHLeftNavigationHouse.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include "MHLeftNavigationHouse.h"

#include <QResizeEvent>
#include <QSpacerItem>

#include "MHDrawHouseManager.h"
#include "MHLeftNavigationHouse.h"
#include "ui_MHLeftNavigationHouse.h"

namespace MHWindow {

MHLeftNavigationHouse::MHLeftNavigationHouse(QWidget *parent) : QWidget(parent), ui(new Ui::MHLeftNavigationHouse) {
    ui->setupUi(this);
    m_gridLayout = new QGridLayout();
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_widgets.push_back(ui->lineButton);
    m_widgets.push_back(ui->rectangleButton);
    m_widgets.push_back(ui->arcButton);
    m_widgets.push_back(ui->pillarButton);
    m_widgets.push_back(ui->cylinderButton);
    m_widgets.push_back(ui->holeButton);
    m_maxColumn = 2;
    layoutWidget();
    m_vLayout = new QVBoxLayout();
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_vLayout->addLayout(m_gridLayout);
    m_vLayout->addStretch();
    setLayout(m_vLayout);

    connect(ui->lineButton, &QToolButton::clicked, this, [this]() {
        MHHouse::MHDrawHouseManager::getInstance().beginDraw(MHHouse::MHDrawType::WALL_LINE);
    });
    connect(ui->rectangleButton, &QToolButton::clicked, this, [this]() {
        MHHouse::MHDrawHouseManager::getInstance().beginDraw(MHHouse::MHDrawType::WALL_RECTANGLE);
    });
    connect(ui->arcButton, &QToolButton::clicked, this, [this]() {
        MHHouse::MHDrawHouseManager::getInstance().beginDraw(MHHouse::MHDrawType::WALL_ARC);
    });
    connect(ui->pillarButton, &QToolButton::clicked, this, [this]() {
        MHHouse::MHDrawHouseManager::getInstance().beginDraw(MHHouse::MHDrawType::PILLAR);
    });
    connect(ui->cylinderButton, &QToolButton::clicked, this, [this]() {
        MHHouse::MHDrawHouseManager::getInstance().beginDraw(MHHouse::MHDrawType::CYLINDER);
    });
    connect(ui->holeButton, &QToolButton::clicked, this, [this]() {
        MHHouse::MHDrawHouseManager::getInstance().beginDraw(MHHouse::MHDrawType::HOLE);
    });
}

MHLeftNavigationHouse::~MHLeftNavigationHouse() {
    delete m_gridLayout;
    delete m_vLayout;
    m_widgets.clear();
    delete ui;
}

void MHLeftNavigationHouse::layoutWidget() {
    for (int i = 0; i < m_widgets.size(); i++) {
        m_gridLayout->removeWidget(m_widgets[i]);
    }
    for (int i = 0; i < m_widgets.size(); i++) {
        m_gridLayout->addWidget(m_widgets[i], i / m_maxColumn, i % m_maxColumn);
    }
}

}  // namespace MHWindow