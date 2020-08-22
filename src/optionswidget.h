#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QSlider>
#include <QComboBox>

#include "spinboxex.h"

#define DEFAULT_SEARCH_PATTERN "*.pdf"
#define DEFAULT_MIN_SIZE 100
#define DEFAULT_MAX_SIZE 1000

class OptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsWidget(QWidget *parent = 0);
    ~OptionsWidget();
    QString pattern();
    int minSize();
    int maxSize();
    int strength();

signals:
    
public slots:
    void showStrengthLevel(int level);

private:

    QLabel * searchPatternLabel;
    QLabel * minSizeLabel;
    QLabel * maxSizeLabel;
    QLabel * strengthCaptionLabel;
    QLabel * strengthLevelLabel;

    QComboBox * searchPatternCombo;

    SpinBoxEx  * minSizeSpinBox;
    SpinBoxEx * maxSizeSpinBox;

    QGridLayout * optionsGrid;
    QSlider * strengthSlider;

    QString strengthLevel[3];

    void setUpUI();
    void makeConnections();
    
};

#endif // OPTIONSWIDGET_H
