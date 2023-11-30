#include "help_section.h"
#include "ui_help_section.h"

help_section::help_section(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::help_section)
{
    ui->setupUi(this);
}

help_section::~help_section()
{
    delete ui;
}
