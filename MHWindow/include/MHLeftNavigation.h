#pragma once

/**
 * @file MHLeftNavigation.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

namespace Ui {
class MHLeftNavigation;
}

namespace MHWindow {

class MHLeftNavigation : public QWidget {
    Q_OBJECT

public:
    explicit MHLeftNavigation(QWidget *parent = nullptr);
    ~MHLeftNavigation();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void layoutWidget();

private:
    Ui::MHLeftNavigation *ui = nullptr;
    QGridLayout *m_gridLayout = nullptr;
    QVBoxLayout *m_vLayout = nullptr;
    int m_maxColumn = 2;
    std::vector<QWidget *> m_widgets;
};

}  // namespace MHWindow
