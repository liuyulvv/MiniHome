#pragma once

/**
 * @file MHTopNavigation.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include <QWidget>

namespace Ui {
class MHTopNavigation;
}

namespace MHWindow {

class MHTopNavigation : public QWidget {
    Q_OBJECT

public:
    explicit MHTopNavigation(QWidget *parent = nullptr);
    ~MHTopNavigation();

private:
    Ui::MHTopNavigation *ui;
};

}  // namespace MHWindow
