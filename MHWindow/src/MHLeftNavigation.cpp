/**
 * @file MHLeftNavigation.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHLeftNavigation.h"

#include <QPushButton>
#include <QResizeEvent>
#include <QSpacerItem>

#include "ui_MHLeftNavigation.h"

namespace MHWindow {

MHLeftNavigation::MHLeftNavigation(QWidget *parent) : QWidget(parent), ui(new Ui::MHLeftNavigation) {
    ui->setupUi(this);
    m_hLayout1 = new QHBoxLayout();
    m_hLayout1->addWidget(ui->lineButton);
    m_hLayout1->addWidget(ui->rectangleButton);
    m_hLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_hLayout2 = new QHBoxLayout();
    m_hLayout2->addWidget(ui->arcButton);
    m_hLayout2->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_vLayout = new QVBoxLayout(this);
    m_vLayout->addLayout(m_hLayout1);
    m_vLayout->addLayout(m_hLayout2);
    setLayout(m_vLayout);
}

MHLeftNavigation::~MHLeftNavigation() {
    delete m_hLayout1;
    delete m_hLayout2;
    delete m_vLayout;
    delete ui;
}

void MHLeftNavigation::resizeEvent(QResizeEvent *event) {
    if (event->size().width() < 152) {
        m_vLayout->removeItem(m_hLayout1);
        m_vLayout->removeItem(m_hLayout2);
        m_vLayout->addWidget(ui->lineButton);
        m_vLayout->addWidget(ui->rectangleButton);
        m_vLayout->addWidget(ui->arcButton);
    } else {
        m_vLayout->removeWidget(ui->lineButton);
        m_vLayout->removeWidget(ui->rectangleButton);
        m_vLayout->removeWidget(ui->arcButton);
        m_vLayout->addLayout(m_hLayout1);
        m_vLayout->addLayout(m_hLayout2);
    }
    QWidget::resizeEvent(event);
}

}  // namespace MHWindow
