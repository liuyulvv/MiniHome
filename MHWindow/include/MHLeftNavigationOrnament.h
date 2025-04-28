#pragma once

/**
 * @file MHLeftNavigationOrnament.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include <QWidget>

namespace Ui {
class MHLeftNavigationOrnament;
}

namespace MHWindow {

class MHLeftNavigationOrnament : public QWidget {
    Q_OBJECT

public:
    explicit MHLeftNavigationOrnament(QWidget *parent = nullptr);
    ~MHLeftNavigationOrnament();

private:
    Ui::MHLeftNavigationOrnament *ui;
};

}  // namespace MHWindow
