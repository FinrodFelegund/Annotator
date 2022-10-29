//
// Created by Daniel Pietsch on 08.08.22.
//

#include "AnnotatorMainWindow.h"
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>

AnnotatorMainWindow::AnnotatorMainWindow(QObject *parent)
{
    this->resize(1037, 786);
    this->setTabPosition(Qt::DockWidgetArea::LeftDockWidgetArea, QTabWidget::East);
    this->setTabPosition(Qt::DockWidgetArea::RightDockWidgetArea, QTabWidget::West);

    _img = std::make_shared<WholeSlideImageReader>();

    setUpUi();
    setUpGraphics();


}

AnnotatorMainWindow::~AnnotatorMainWindow() noexcept
{

}

void AnnotatorMainWindow::setUpGraphics()
{
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    policy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(policy);

    _centralWidget = new QWidget(this);
    _centralWidget->setObjectName("centralWidget");
    policy.setHeightForWidth(_centralWidget->sizePolicy().hasHeightForWidth());
    _centralWidget->setSizePolicy(policy);

    _horizontalLayout = new QHBoxLayout(_centralWidget);
    _horizontalLayout->setSpacing(6);
    _horizontalLayout->setContentsMargins(150, 0, 150, 0);

    _view = std::make_shared<AnnotatorViewer>(_centralWidget);
    _horizontalLayout->addWidget(_view.get());

    this->setCentralWidget(_centralWidget);
}

void AnnotatorMainWindow::setUpUi()
{
    QMenu* bar = menuBar()->addMenu(tr("File"));

    _openImage = new QAction(this);
    _openImage->setObjectName(QStringLiteral("actionOpen"));
    _openImage->setText("Open File");
    bar->addAction(_openImage);
    connect(_openImage, &QAction::triggered, this, &AnnotatorMainWindow::openImage);
}

void AnnotatorMainWindow::openImage()
{

    if(!_img) //do some internal stuff of img and check if error pop up
        return;


    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "/Users/danielpietsch/Documents/Bilder/WholeSlideImages", "");

    if(fileName.isEmpty())
    {
        //do some outputs to status bar
        return;
    }

    //qDebug() << "Test: " << fileName;
    _img->initializeImage(fileName.toStdString());
    _view->initialize(_img);
}

