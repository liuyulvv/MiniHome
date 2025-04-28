#pragma once

/**
 * @file MHRightNavigation.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-24
 */

#include <QWidget>

namespace Ui {
class MHRightNavigation;
}

namespace MHWindow {

class MHRightNavigation : public QWidget {
    Q_OBJECT

public:
    explicit MHRightNavigation(QWidget *parent = nullptr);
    ~MHRightNavigation();

private:
    Ui::MHRightNavigation *ui;
};

}  // namespace MHWindow
