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
#include <QLabel>
#include <QScrollArea>

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

   std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
   QDir dir(QString::fromStdString(loc) );
   QDirIterator it(dir);

   while (it.hasNext()) { // for all files

       QString f = it.next();

           if (f.contains("."))

            #if defined(_WIN32)
           if (f.contains(".wmv"))  { // windows
            #else
           if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
            #endif

           QString thumb = f.left( f .length() - 4) +".png";
           if (QFile(thumb).exists()) { // if a png thumbnail exists
               QImageReader *imageReader = new QImageReader(thumb);
                   QImage sprite = imageReader->read(); // read the thumbnail
                   if (!sprite.isNull()) {
                       QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                       QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                       out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                   }
                   else
                       qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
           }
           else
               qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
       }
   }

   return out;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn(std::string(argv[1]));

    if (videos.size() == 0) {
        const int result = QMessageBox::information(
            NULL,
            QString("Tomeo"),
            QString("No videos found! Add a command line argument for the file location.")
        );
        exit(-1);
    }

    // Set up a scroll area to make the content scrollable
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: #3a3b3c");

    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);

    QWidget *buttonWidget = new QWidget();
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);

    // Add a logo label to the top left corner
    QLabel *logoLabel = new QLabel();
    QPixmap logoPixmap(":/images/logo.png");  // Adjust the path to your logo image
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setScaledContents(true);
    logoLabel->setFixedSize(250, 80);  // Adjust the size of the logo
    logoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    scrollLayout->addWidget(logoLabel);


    // Set up polaroid frames with video widgets
    const int polaroidWidth = 425; // Adjust the width based on your design
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
        ThePlayer *mediaPlayer = new ThePlayer();
        QVideoWidget *videoWidget = new QVideoWidget();

        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), mediaPlayer, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));

        mediaPlayer->setVideoOutput(videoWidget);
        mediaPlayer->setContent(&buttons, & videos);
        mediaPlayer->jumpTo(& videos.at(i));
        videoWidget->setFixedSize(polaroidWidth, polaroidHeight);

        // Set up layout for polaroid frame with video widget
        QVBoxLayout *polaroidLayout = new QVBoxLayout();
        polaroidLayout->addWidget(videoWidget);
        polaroidLayout->setContentsMargins(0, 0, 0, 0);


        // Show polaroid frame
        polaroidLabel->setLayout(polaroidLayout);
        scrollLayout->addWidget(polaroidLabel);
    }

    // Create the main window
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setStyleSheet("background-color: #3a3b3c");
    window.setWindowTitle("Tomeo");
    window.setMinimumSize(480, 800);
    top->addWidget(scrollArea);

    // Show the main window
    window.show();

    // Wait for the app to terminate
    return app.exec();
}
