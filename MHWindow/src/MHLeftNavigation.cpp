/**
 * @file MHLeftNavigation.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHLeftNavigation.h"

#include <QFile>

#include "MHDrawHouseManager.h"
#include "ui_MHLeftNavigation.h"

namespace MHWindow {

MHLeftNavigation::MHLeftNavigation(QWidget *parent) : QWidget(parent), ui(new Ui::MHLeftNavigation) {
    ui->setupUi(this);
    QFile qss("styles/MHLeftNavigation.qss");
    if (qss.open(QFile::ReadOnly)) {
        setStyleSheet(qss.readAll());
        qss.close();
    }
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(ui->houseButton, 0);
    m_buttonGroup->addButton(ui->ornamentButton, 1);
    m_buttonGroup->button(0)->setChecked(true);
    m_buttonGroup->setExclusive(true);
    connect(ui->houseButton, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->ornamentButton, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    m_house = new MHLeftNavigationHouse();
    m_ornament = new MHLeftNavigationOrnament();
    ui->stackedWidget->addWidget(m_house);
    ui->stackedWidget->addWidget(m_ornament);
    ui->stackedWidget->setCurrentIndex(0);
    setAutoFillBackground(true);
}

MHLeftNavigation::~MHLeftNavigation() {
    delete m_buttonGroup;
    delete m_house;
    delete m_ornament;
    delete ui;
}

}  // namespace MHWindow
