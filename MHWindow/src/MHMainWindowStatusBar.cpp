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
    m_widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(m_widget);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(5);

    m_leftNavigationButton = new QToolButton(m_widget);
    m_leftNavigationButton->setIcon(QIcon("icons/left_navigation_status_bar.svg"));
    m_leftNavigationButton->setStyleSheet(
        "QToolButton { background-color: transparent; border: none; }"
        "QToolButton:hover { background-color: rgba(200, 200, 200, 50); }"
        "QToolButton:pressed { background-color: rgba(180, 180, 180, 70); }");

    m_rightNavigationButton = new QToolButton(m_widget);
    m_rightNavigationButton->setIcon(QIcon("icons/left_navigation_status_bar.svg"));
    m_rightNavigationButton->setStyleSheet(
        "QToolButton { background-color: transparent; border: none; }"
        "QToolButton:hover { background-color: rgba(200, 200, 200, 50); }"
        "QToolButton:pressed { background-color: rgba(180, 180, 180, 70); }");

    connect(m_leftNavigationButton, &QToolButton::clicked, this, &MHMainWindowStatusBar::leftNavigationButtonClicked);
    connect(m_rightNavigationButton, &QToolButton::clicked, this, &MHMainWindowStatusBar::rightNavigationButtonClicked);
    layout->addWidget(m_leftNavigationButton);
    layout->addStretch();
    layout->addWidget(m_rightNavigationButton);

    m_statusBar->addWidget(m_widget, 1);
}

}  // namespace MHWindow