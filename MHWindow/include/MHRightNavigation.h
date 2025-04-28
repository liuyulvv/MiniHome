#pragma once

/**
 * @file MHRightNavigation.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-24
 */

#include <QWidget>

#include "MHDrawBase.h"
#include "MHRightNavigationDrawWall.h"
#include "MHRightNavigationSpace.h"

namespace Ui {
class MHRightNavigation;
}

namespace MHWindow {

class MHRightNavigation : public QWidget {
    Q_OBJECT

public:
    explicit MHRightNavigation(QWidget *parent = nullptr);
    ~MHRightNavigation();

public:
    void showRightNavigationDrawWall(bool show, MHHouse::MHDrawType drawType);

private:
    Ui::MHRightNavigation *ui;
    MHRightNavigationSpace *m_rightNavigationSpace = nullptr;
    MHRightNavigationDrawWall *m_rightNavigationDrawWall = nullptr;
};

}  // namespace MHWindow
