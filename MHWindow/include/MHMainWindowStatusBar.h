#pragma once

/**
 * @file MHMainWindowStatusBar.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-20
 */

#include <QStatusBar>
#include <QToolButton>
#include <QWidget>

namespace MHWindow {

class MHMainWindowStatusBar : public QWidget {
    Q_OBJECT

public:
    explicit MHMainWindowStatusBar(QStatusBar* statusBar);
    ~MHMainWindowStatusBar() override;
    MHMainWindowStatusBar(const MHMainWindowStatusBar& statusBar) = delete;
    MHMainWindowStatusBar(MHMainWindowStatusBar&& statusBar) = delete;
    MHMainWindowStatusBar& operator=(const MHMainWindowStatusBar& statusBar) = delete;
    MHMainWindowStatusBar& operator=(MHMainWindowStatusBar&& statusBar) = delete;

signals:
    void leftNavigationButtonClicked();

private:
    void setupUi();

private:
    QStatusBar* m_statusBar;

    QWidget* m_leftWidget;
    QToolButton* m_leftNavigationButton;
};

}  // namespace MHWindow