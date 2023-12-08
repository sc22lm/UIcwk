#include "homepage.h"
#include "ui_homepage.h"
#include <QListWidgetItem>
#include <QDir>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QBoxLayout>

HomePage::HomePage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    QString directoryPath = "/home/csunix/sc22ah3/Documents/comp2811/UIcwk/videos";

    // Populate video list with filenames from the specified directory
    QStringList videoFiles = getVideoFilesInDirectory(directoryPath);
    ui->videoListWidget->addItems(videoFiles);

    // Set up polaroid frames with video widgets
    const int polaroidWidth = 450; // Adjust the width based on your design
    const int polaroidHeight = 550; // Adjust the height based on your design
    const int verticalSpacing = 20; // Adjust the vertical spacing

    for (int i = 0; i < videoFiles.size(); ++i) {
        // Polaroid frame
        QLabel* polaroidLabel = new QLabel(this);
        QPixmap polaroidPixmap(":/images/polaroid.png");
        polaroidLabel->setPixmap(polaroidPixmap);
        polaroidLabel->setScaledContents(true);
        polaroidLabel->setFixedSize(polaroidWidth, polaroidHeight);
        polaroidLabel->move(0, i * (polaroidHeight + verticalSpacing));

        // Video widget
        QMediaPlayer *mediaPlayer = new QMediaPlayer(this);
        QVideoWidget *videoWidget = new QVideoWidget(polaroidLabel);
        mediaPlayer->setVideoOutput(videoWidget);
        videoWidget->setFixedSize(polaroidWidth, polaroidHeight);

        // Set up layout for polaroid frame with video widget
        QVBoxLayout *layout = new QVBoxLayout(polaroidLabel);
        layout->addWidget(videoWidget);
        layout->setContentsMargins(0, 0, 0, 0);

        // Show polaroid frame
        polaroidLabel->show();

        // Load video into the player (adjust path as needed)
        mediaPlayer->setMedia(QUrl::fromLocalFile(directoryPath + "/" + videoFiles[i]));

        // Connect signals and slots for video playback (if needed)
        connect(ui->videoListWidget, &QListWidget::currentItemChanged, mediaPlayer, [&directoryPath, mediaPlayer, videoFiles, i](QListWidgetItem* item) {
            Q_UNUSED(item);
            mediaPlayer->setMedia(QUrl::fromLocalFile(directoryPath + "/" + videoFiles[i]));
            mediaPlayer->play();
        });
    }

    connect(ui->videoListWidget, &QListWidget::currentItemChanged, this, &HomePage::showVideo);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::showVideo(QListWidgetItem* item)
{
    // Handle the selected video item, update the video display, etc.
    // You may need to load and play the corresponding video based on the selected item.
    Q_UNUSED(item);
}

QStringList HomePage::getVideoFilesInDirectory(const QString &directoryPath)
{
    QDir directory(directoryPath);
    QStringList nameFilters;
    nameFilters << "*.mp4" << "*.avi" << "*.mkv"; // Add more video formats if needed
    QStringList videoFiles = directory.entryList(nameFilters, QDir::Files);
    return videoFiles;
}
