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
#include "settings_window.h"
#include "homepage.h"
#include <QLabel>

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
    QVBoxLayout *layout = new QVBoxLayout();
    buttonWidget->setLayout(layout);

    // Set up polaroid frames with video widgets
    const int polaroidWidth = 450; // Adjust the width based on your design
    const int polaroidHeight = 550; // Adjust the height based on your design
    const int verticalSpacing = 20; // Adjust the vertical spacing

    for (int i = 0; i < videos.size(); ++i) {
        // Polaroid frame
        QLabel* polaroidLabel = new QLabel();
        QPixmap polaroidPixmap(":/images/polaroid.png");
        polaroidLabel->setPixmap(polaroidPixmap);
        polaroidLabel->setScaledContents(true);
        polaroidLabel->setFixedSize(polaroidWidth, polaroidHeight);
        polaroidLabel->move(0, i * (polaroidHeight + verticalSpacing));

        // Video widget
        ThePlayer *mediaPlayer = new ThePlayer;
        QVideoWidget *videoWidget = new QVideoWidget;
        mediaPlayer->setVideoOutput(videoWidget);
        videoWidget->setFixedSize(polaroidWidth, polaroidHeight);

        // Set up layout for polaroid frame with video widget
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(videoWidget);
        layout->setContentsMargins(0, 0, 0, 0);

        // Show polaroid frame
        polaroidLabel->show();
       }


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
    window.setMaximumSize(480, 800);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonWidget);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
