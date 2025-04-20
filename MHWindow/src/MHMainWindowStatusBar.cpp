/**
 * @file MHMainWindowStatusBar.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-20
 */

#include "MHMainWindowStatusBar.h"

#include <QHBoxLayout>
#include <QStyle>

namespace MHWindow {

MHMainWindowStatusBar::MHMainWindowStatusBar(QStatusBar* statusBar) : QWidget(nullptr), m_statusBar(statusBar) {
    setupUi();
}

MHMainWindowStatusBar::~MHMainWindowStatusBar() = default;

void MHMainWindowStatusBar::setupUi() {
    m_leftWidget = new QWidget(this);
    QHBoxLayout* leftLayout = new QHBoxLayout(m_leftWidget);
    leftLayout->setContentsMargins(5, 0, 5, 0);
    leftLayout->setSpacing(5);
    m_leftNavigationButton = new QToolButton(m_leftWidget);
    m_leftNavigationButton->setIcon(QIcon("icons/left_navigation_status_bar.svg"));
    m_leftNavigationButton->setStyleSheet(
        "QToolButton { background-color: transparent; border: none; }"
        "QToolButton:hover { background-color: rgba(200, 200, 200, 50); }"
        "QToolButton:pressed { background-color: rgba(180, 180, 180, 70); }");
    connect(m_leftNavigationButton, &QToolButton::clicked, this, &MHMainWindowStatusBar::leftNavigationButtonClicked);
    leftLayout->addWidget(m_leftNavigationButton);
    leftLayout->addStretch();

    m_statusBar->addWidget(m_leftWidget);
}

}  // namespace MHWindow