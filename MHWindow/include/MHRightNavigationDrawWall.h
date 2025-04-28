#pragma once

/**
 * @file MHRightNavigationDrawWall.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include <QWidget>

#include "MHDrawBase.h"

namespace Ui {
class MHRightNavigationDrawWall;
}

namespace MHWindow {

class MHRightNavigationDrawWall : public QWidget {
    Q_OBJECT

public:
    explicit MHRightNavigationDrawWall(QWidget *parent = nullptr);
    ~MHRightNavigationDrawWall();

public:
    void setDrawType(MHHouse::MHDrawType drawType);

private:
    Ui::MHRightNavigationDrawWall *ui;
    MHHouse::MHDrawType m_drawType = MHHouse::MHDrawType::NONE;
};

}  // namespace MHWindow
