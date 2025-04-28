#pragma once

/**
 * @file MHLeftNavigationHouse.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-28
 */

#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

namespace Ui {
class MHLeftNavigationHouse;
}

namespace MHWindow {

class MHLeftNavigationHouse : public QWidget {
    Q_OBJECT

public:
    explicit MHLeftNavigationHouse(QWidget *parent = nullptr);
    ~MHLeftNavigationHouse();

private:
    void layoutWidget();

private:
    Ui::MHLeftNavigationHouse *ui;
    QGridLayout *m_gridLayout = nullptr;
    QVBoxLayout *m_vLayout = nullptr;
    int m_maxColumn = 2;
    std::vector<QWidget *> m_widgets;
};

}  // namespace MHWindow
