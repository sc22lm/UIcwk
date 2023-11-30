//
//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QFileDialog>
#include "the_player.h"
#include "the_button.h"
#include "help_section.h"

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (QStringList fileNames) {
    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();

    // create image reader to read thumbnails
    QImageReader* imageReader = new QImageReader();

    // iterate through each selected video file for thumbnails
    for (QString file: fileNames) {
        // split the name of the video file and look for a png sharing the same name
        QString thumb = file.split(".")[0] + ".png";

        // ensure png thumbnail exists
        if (QFile(thumb).exists()) {
            // update image reader and read thumbnail
            imageReader->setFileName(thumb);
            QImage sprite = imageReader->read();

            // ensure sprite was read
            if (!sprite.isNull()) {
                QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                QUrl* url = new QUrl(QUrl::fromLocalFile(file)); // convert the file location to a generic url
                out.push_back(TheButtonInfo(url, ico)); // add to the output list
            }
            else {
                qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << Qt::endl;
            }
        }
        else {
            qDebug() << "warning: skipping video because I could't find thumbnail" << thumb << Qt::endl;
        }
    }

    // free imageReader when done
    delete imageReader;

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << Qt::endl;

    // create the Qt Application
    QApplication app(argc, argv);

    help_section w;
    w.show();

    // create window
    QWidget window;

    // open file dialogue to select videos
    QFileDialog dialogue(&window);
    dialogue.setFileMode(QFileDialog::ExistingFiles);
    dialogue.setNameFilter("Videos (*.MOV *.mp4 *.wmv)");
    dialogue.setViewMode(QFileDialog::List);

    // list of filenames and the videos opened
    QStringList fileNames;
    std::vector<TheButtonInfo> videos;
    if (dialogue.exec()) {
        fileNames = dialogue.selectedFiles();
        videos = getInfoIn(fileNames);
    }
    else {
        const int result = QMessageBox::information(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the layout
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonWidget);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
