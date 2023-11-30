#include "settings_window.h"
#include "ui_settings_window.h"
#include <QCheckBox>
#include <QDebug>

Settings_Window::Settings_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings_Window)
{
    ui->setupUi(this);

    // Connect the clicked signal of the checkboxes to the handleCheckboxClicked slot
    connect(ui->dark_mode, &QCheckBox::stateChanged, this, &Settings_Window::handleCheckboxClicked);
    connect(ui->post_notifs, &QCheckBox::stateChanged, this, &Settings_Window::handleCheckboxClicked);
    connect(ui->friendpost_notifs, &QCheckBox::stateChanged, this, &Settings_Window::handleCheckboxClicked);
    connect(ui->friendrequest_notifs, &QCheckBox::stateChanged, this, &Settings_Window::handleCheckboxClicked);
}

Settings_Window::~Settings_Window()
{
    delete ui;
}

void Settings_Window::handleCheckboxClicked(){
    QCheckBox *clickedCheckbox = qobject_cast<QCheckBox *>(sender());
    if (clickedCheckbox){
        qDebug() << "Checkbox clicked:" << clickedCheckbox->text() << "State:" << clickedCheckbox->isChecked();

        // Toggle checkbox icon based on its state
        if (clickedCheckbox->isChecked()){
            clickedCheckbox->setChecked("True");
        }else{
            clickedCheckbox->setChecked("False");
        }
    }
}
