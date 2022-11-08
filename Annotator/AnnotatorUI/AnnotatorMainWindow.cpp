//
// Created by Daniel Pietsch on 08.08.22.
//

#include "AnnotatorMainWindow.h"
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QStatusBar>

AnnotatorMainWindow::AnnotatorMainWindow(QObject *parent)
{
    this->resize(1037, 786);
    this->setTabPosition(Qt::DockWidgetArea::LeftDockWidgetArea, QTabWidget::East);
    this->setTabPosition(Qt::DockWidgetArea::RightDockWidgetArea, QTabWidget::West);

    _currentFileName = "";

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
    //_horizontalLayout->setContentsMargins(150, 0, 150, 0);

    _view = std::make_shared<AnnotatorViewer>(_centralWidget);
    _horizontalLayout->addWidget(_view.get());

    this->setCentralWidget(_centralWidget);

    _leftDockwidget = new DockWidget(this);
    _leftDockwidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    _leftDockwidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    _leftDockwidget->setFixedWidth(150);
    _leftDockwidget->setMaximumWidth(200);


    QWidget *widget = new QWidget(this);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _leftLayout = new QVBoxLayout(widget);
    _bar = new Toolbar(this);
    _bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    _bar->resize(40, this->height() / 2);


    _leftLayout->addWidget(_bar, 0, Qt::AlignRight | Qt::AlignTop);



    _leftDockwidget->setWidget(widget);


    this->addDockWidget(Qt::LeftDockWidgetArea, _leftDockwidget);

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
    _currentFileName = QFileDialog::getOpenFileName(this, "Open File", "/Users/danielpietsch/Documents/Bilder/WholeSlideImages", "");

    if(_currentFileName.isEmpty())
    {
        this->statusBar()->showMessage("No suitable Image found");
        return;
    } else
    {
        //this->statusBar()->showMessage("Opening: " + _currentFileName);
    }

    emit initializeImage(_currentFileName.toStdString());


}

std::shared_ptr<AnnotatorViewer> AnnotatorMainWindow::getView()
{
    if(_view)
    {
        return _view;
    }

    return nullptr;
}

Toolbar* AnnotatorMainWindow::getToolBar()
{
    return _bar;
}

void AnnotatorMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    _bar->resize(40, this->height() / 2);

}

