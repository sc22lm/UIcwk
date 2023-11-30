#ifndef HELP_SECTION_H
#define HELP_SECTION_H

#include <QMainWindow>

namespace Ui {
class help_section;
}

class help_section : public QMainWindow
{
    Q_OBJECT

public:
    explicit help_section(QWidget *parent = nullptr);
    ~help_section();

private:
    Ui::help_section *ui;
};

#endif // HELP_SECTION_H
