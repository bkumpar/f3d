#include "optionswidget.h"

OptionsWidget::OptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    strengthLevel[0] = tr("Low") ;
    strengthLevel[1] = tr("Optimal") ;
    strengthLevel[2]=  tr("Strong but slow") ;
    setUpUI();
    makeConnections();
    strengthSlider->setValue(1);

    #ifdef DEBUG
    this->minSizeSpinBox->setValue(100);
    this->maxSizeSpinBox->setValue(1000);;
    #endif
}

OptionsWidget::~OptionsWidget()
{
}

QString OptionsWidget::pattern()
{
    return searchPatternCombo->currentText();
}
int OptionsWidget::minSize()
{
    return minSizeSpinBox->fileSize();
}

int OptionsWidget::maxSize()
{
    return maxSizeSpinBox->fileSize();
}
int OptionsWidget::strength()
{
    return strengthSlider->value();
}

void OptionsWidget::setUpUI()
{
    searchPatternLabel =  new QLabel(tr("Search pattern:"), this);
    searchPatternLabel->setAlignment(Qt::AlignRight);

    minSizeLabel = new QLabel(tr("Min file size:"), this);
    minSizeLabel->setAlignment(Qt::AlignRight);

    maxSizeLabel = new QLabel(tr("Max file size:"), this);
    maxSizeLabel->setAlignment(Qt::AlignRight);

    searchPatternCombo = new QComboBox(this);
    searchPatternCombo->setEnabled(true);
    searchPatternCombo->setInsertPolicy(QComboBox::InsertPolicy::InsertAtBottom);
    searchPatternCombo->setEditable(true);
//    searchPatternCombo->setEditText(DEFAULT_SEARCH_PATTERN);
    searchPatternCombo->addItem("*.jpg");
    searchPatternCombo->addItem("*.bmp");
    searchPatternCombo->addItem("*.png");
    searchPatternCombo->addItem("*.mpg");
    searchPatternCombo->addItem("*.mov");
    searchPatternCombo->addItem("*.mp4");
    searchPatternCombo->addItem("*.avi");
    searchPatternCombo->addItem("*.mp3");
    searchPatternCombo->addItem("*.pdf");

    minSizeSpinBox = new SpinBoxEx(this);
    minSizeSpinBox->setValue(DEFAULT_MIN_SIZE);

    maxSizeSpinBox = new SpinBoxEx(this);
    maxSizeSpinBox->setValue(DEFAULT_MAX_SIZE);

    strengthSlider = new QSlider(Qt::Horizontal, this);
    strengthSlider->setPageStep( 1 );
    strengthSlider->setSingleStep( 1 );
    strengthSlider->setRange ( 0, 2);

    strengthSlider->setTickPosition(QSlider::TicksBelow);
    strengthSlider->setMinimumWidth(128) ;

    strengthCaptionLabel = new QLabel(tr("Strength:"), this);
    strengthCaptionLabel->setAlignment(Qt::AlignLeft);

    strengthLevelLabel = new QLabel(tr("(Strength Level)"), this);
    strengthLevelLabel->setAlignment(Qt::AlignLeft );

    optionsGrid = new QGridLayout(this);

    optionsGrid->addWidget(strengthCaptionLabel, 0, 0);
    optionsGrid->addWidget(strengthSlider, 1, 0);
    optionsGrid->addWidget(strengthLevelLabel, 2, 0 );

    optionsGrid->addWidget(searchPatternLabel, 0, 2);
    optionsGrid->addWidget(minSizeLabel, 1, 2);
    optionsGrid->addWidget(maxSizeLabel, 2, 2);

    optionsGrid->addWidget(searchPatternCombo, 0, 3);
    optionsGrid->addWidget(minSizeSpinBox, 1, 3);
    optionsGrid->addWidget(maxSizeSpinBox, 2, 3);

    optionsGrid->setColumnStretch(1,4);
    optionsGrid->setColumnStretch(1,0);
    optionsGrid->setColumnStretch(1,4);
    optionsGrid->setColumnStretch(1,3);

}
void  OptionsWidget::makeConnections()
{
//    qDebug(">>OptionsWidget::makeConnections()");
    connect(strengthSlider,SIGNAL(valueChanged(int)), this, SLOT(showStrengthLevel(int)));
//    qDebug("<<OptionsWidget::makeConnections()");
}

void OptionsWidget::showStrengthLevel(int level)
{
//    qDebug(">>OptionsWidget::showStrengthLevel()");
    strengthLevelLabel->setText(strengthLevel[level]);
//    qDebug("<<OptionsWidget::showStrengthLevel()");

}
