#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <QMainWindow>
#include <QCheckBox>

namespace Ui {
class Settings_Window;
}

class Settings_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings_Window(QWidget *parent = nullptr);
    ~Settings_Window();

private slots:
    void handleCheckboxClicked();

private:
    Ui::Settings_Window *ui;
    QCheckBox dark_mode;
    QCheckBox post_notifs;
    QCheckBox friendpost_notifs;
    QCheckBox friendrequest_notifs;
};

#endif // SETTINGS_WINDOW_H
