#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class HomePage;
}

class HomePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:
    void showVideo(QListWidgetItem* item);
    QStringList getVideoFilesInDirectory(const QString &directoryPath);

private:
    Ui::HomePage *ui;
};

#endif // HOMEPAGE_H
