#pragma once

/**
 * @file MHRightNavigationSpace.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include <QWidget>

namespace Ui {
class MHRightNavigationSpace;
}

namespace MHWindow {

class MHRightNavigationSpace : public QWidget {
    Q_OBJECT

public:
    explicit MHRightNavigationSpace(QWidget *parent = nullptr);
    ~MHRightNavigationSpace();

private:
    Ui::MHRightNavigationSpace *ui;
};

}  // namespace MHWindow
