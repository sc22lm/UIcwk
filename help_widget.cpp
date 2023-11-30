#include "help_widget.h"
#include "ui_help_widget.h"

#include <QPushButton>
#include <QLabel>

help_widget::help_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::help_widget)
{
    ui->setupUi(this);
    // Connect buttons  to slots
    connect(ui->helpButton, &QPushButton::clicked, this, &help_widget::onButtonClicked);
}

help_widget::~help_widget()
{
    delete ui;
}

void help_widget::onButtonClicked(){

}
