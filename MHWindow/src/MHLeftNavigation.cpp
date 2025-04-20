/**
 * @file MHLeftNavigation.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHLeftNavigation.h"

#include <QResizeEvent>
#include <QSpacerItem>

#include "MHDrawHouseManager.h"
#include "ui_MHLeftNavigation.h"

namespace MHWindow {

MHLeftNavigation::MHLeftNavigation(QWidget *parent) : QWidget(parent), ui(new Ui::MHLeftNavigation) {
    ui->setupUi(this);
    m_gridLayout = new QGridLayout();
    m_widgets.push_back(ui->lineButton);
    m_widgets.push_back(ui->rectangleButton);
    m_widgets.push_back(ui->arcButton);
    m_widgets.push_back(ui->pillarButton);
    m_widgets.push_back(ui->cylinderButton);
    m_widgets.push_back(ui->holeButton);
    m_maxColumn = 2;
    layoutWidget();
    m_vLayout = new QVBoxLayout();
    m_vLayout->addLayout(m_gridLayout);
    m_vLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
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

MHLeftNavigation::~MHLeftNavigation() {
    m_widgets.clear();
    delete m_gridLayout;
    delete m_vLayout;
    delete ui;
}

void MHLeftNavigation::resizeEvent(QResizeEvent *event) {
    if (event->size().width() < 152) {
        m_maxColumn = 1;
        layoutWidget();
    } else {
        m_maxColumn = 2;
        layoutWidget();
    }
    QWidget::resizeEvent(event);
}

void MHLeftNavigation::layoutWidget() {
    for (int i = 0; i < m_widgets.size(); i++) {
        m_gridLayout->removeWidget(m_widgets[i]);
    }
    for (int i = 0; i < m_widgets.size(); i++) {
        m_gridLayout->addWidget(m_widgets[i], i / m_maxColumn, i % m_maxColumn);
    }
}

}  // namespace MHWindow
