#ifndef COMPARISON_H
#define COMPARISON_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>
#include "video.h"
#include "prefs.h"

namespace Ui { class Comparison; }

class Comparison : public QDialog
{
    Q_OBJECT

public:
    Comparison(const QVector<Video *> &userVideos, const Prefs &userPrefs, QWidget &parent);
    ~Comparison();
    void reportMatchingVideos();

private:
    Ui::Comparison *ui;

    QVector<Video *> _videos;
    Prefs _prefs;
    int _leftVideo = 0;
    int _rightVideo = 0;
    uint _videosDeleted = 0;
    qint64 _spaceSaved = 0;
    bool _seekForwards = true;

    short _durationModifier = 0;
    short _phashSimilarity = 0;
    double _ssimSimilarity = 0.0;

    int _zoomLevel = 0;
    QPixmap _leftZoomed;
    int _leftW = 0;
    int _leftH = 0;
    QPixmap _rightZoomed;
    int _rightW = 0;
    int _rightH = 0;

private slots:
    void on_prevVideo_clicked();
    void on_nextVideo_clicked();
    bool bothVideosMatch(const Video *left, const Video *right);
    short phashSimilarity(const Video *left, const Video *right);

    void showVideo(const QString &side) const;
    QString readableDuration(const qint64 &milliseconds) const;
    QString readableFileSize(const qint64 &filesize) const;
    QString readableBitRate(const double &kbps) const;
    void highlightBetterProperties() const;
    void updateUI();
    int comparisonsSoFar() const;

    void on_selectPhash_clicked ( const bool &checked) { if(checked) _prefs._ComparisonMode = _prefs._PHASH;
                                                         emit switchComparisonMode(_prefs._ComparisonMode); }
    void on_selectSSIM_clicked ( const bool &checked) { if(checked) _prefs._ComparisonMode = _prefs._SSIM;
                                                        emit switchComparisonMode(_prefs._ComparisonMode); }
    void on_leftImage_clicked() { QDesktopServices::openUrl(QUrl::fromLocalFile(_videos[_leftVideo]->filename)); }
    void on_rightImage_clicked() { QDesktopServices::openUrl(QUrl::fromLocalFile(_videos[_rightVideo]->filename)); }
    void on_leftFileName_clicked() const;
    void on_rightFileName_clicked() const;
    void on_leftDelete_clicked();
    void on_rightDelete_clicked();
    void on_leftMove_clicked();
    void on_rightMove_clicked();
    void on_swapFilenames_clicked() const;

    void on_thresholdSlider_valueChanged(const int &value);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

    double sigma(const cv::Mat &m, const int &i, const int &j, const int &block_size) const;
    double covariance(const cv::Mat &m0, const cv::Mat &m1, const int &i, const int &j, const int &block_size) const;
    double ssim(const cv::Mat &m0, const cv::Mat &m1, const int &block_size) const;

signals:
    void sendStatusMessage(const QString &message) const;
    void switchComparisonMode(const short &mode) const;
    void adjustThresholdSlider(const int &value) const;
};


class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent) { Q_UNUSED (parent); }
protected:
    void mousePressEvent(QMouseEvent *event) { Q_UNUSED (event); emit clicked(); }
signals:
    void clicked();
};

#endif // COMPARISON_H
