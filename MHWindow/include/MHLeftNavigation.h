#pragma once

/**
 * @file MHLeftNavigation.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include <QButtonGroup>
#include <QWidget>

#include "MHLeftNavigationHouse.h"
#include "MHLeftNavigationOrnament.h"

namespace Ui {
class MHLeftNavigation;
}

namespace MHWindow {

class MHLeftNavigation : public QWidget {
    Q_OBJECT

public:
    explicit MHLeftNavigation(QWidget *parent = nullptr);
    ~MHLeftNavigation();

private:
    Ui::MHLeftNavigation *ui = nullptr;
    QButtonGroup *m_buttonGroup = nullptr;
    MHLeftNavigationHouse *m_house = nullptr;
    MHLeftNavigationOrnament *m_ornament = nullptr;
};

}  // namespace MHWindow
