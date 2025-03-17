#pragma once

/**
 * @file MHLeftNavigation.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

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
    Ui::MHLeftNavigation *ui;
    QHBoxLayout *m_hLayout1;
    QHBoxLayout *m_hLayout2;
    QVBoxLayout *m_vLayout;
};

}  // namespace MHWindow
