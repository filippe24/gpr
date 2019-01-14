/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Quit;
    QAction *action_Open;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    GLWidget *openGLWidget;
    QTabWidget *tabWidget;
    QWidget *tabCurvature;
    QVBoxLayout *verticalLayout;
    QPushButton *defaultButton;
    QCheckBox *checkBoxFill;
    QCheckBox *checkBoxRefl;
    QLabel *Curvature;
    QVBoxLayout *Curvatures;
    QPushButton *ComputeCurvature;
    QRadioButton *Gaussian;
    QSlider *gaussianWeight;
    QRadioButton *Mean;
    QSlider *meanWeight;
    QSpacerItem *verticalSpacer;
    QWidget *tabSmooth;
    QVBoxLayout *verticalLayout_2;
    QPushButton *defaultButton_2;
    QFrame *line_2;
    QLabel *iterativeLabel;
    QPushButton *LaplacianPush;
    QPushButton *LaplacianCotgPush;
    QHBoxLayout *LmbdaLayout;
    QLabel *lambdaLabel;
    QSlider *lambdaSlider;
    QComboBox *iterativeSelection;
    QFrame *line;
    QLabel *globalLabel;
    QPushButton *globalSmoothPush;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QMenuBar *menuBar;
    QMenu *menu_File;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName(QStringLiteral("action_Quit"));
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openGLWidget = new GLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(openGLWidget);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabWidget->setMaximumSize(QSize(16777215, 16777215));
        tabWidget->setTabBarAutoHide(true);
        tabCurvature = new QWidget();
        tabCurvature->setObjectName(QStringLiteral("tabCurvature"));
        verticalLayout = new QVBoxLayout(tabCurvature);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        defaultButton = new QPushButton(tabCurvature);
        defaultButton->setObjectName(QStringLiteral("defaultButton"));

        verticalLayout->addWidget(defaultButton);

        checkBoxFill = new QCheckBox(tabCurvature);
        checkBoxFill->setObjectName(QStringLiteral("checkBoxFill"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(checkBoxFill->sizePolicy().hasHeightForWidth());
        checkBoxFill->setSizePolicy(sizePolicy2);
        checkBoxFill->setChecked(true);

        verticalLayout->addWidget(checkBoxFill);

        checkBoxRefl = new QCheckBox(tabCurvature);
        checkBoxRefl->setObjectName(QStringLiteral("checkBoxRefl"));
        checkBoxRefl->setEnabled(true);

        verticalLayout->addWidget(checkBoxRefl);

        Curvature = new QLabel(tabCurvature);
        Curvature->setObjectName(QStringLiteral("Curvature"));

        verticalLayout->addWidget(Curvature);

        Curvatures = new QVBoxLayout();
        Curvatures->setSpacing(6);
        Curvatures->setObjectName(QStringLiteral("Curvatures"));
        ComputeCurvature = new QPushButton(tabCurvature);
        ComputeCurvature->setObjectName(QStringLiteral("ComputeCurvature"));

        Curvatures->addWidget(ComputeCurvature);

        Gaussian = new QRadioButton(tabCurvature);
        Gaussian->setObjectName(QStringLiteral("Gaussian"));
        Gaussian->setChecked(true);

        Curvatures->addWidget(Gaussian);

        gaussianWeight = new QSlider(tabCurvature);
        gaussianWeight->setObjectName(QStringLiteral("gaussianWeight"));
        gaussianWeight->setMinimum(1);
        gaussianWeight->setValue(2);
        gaussianWeight->setOrientation(Qt::Horizontal);

        Curvatures->addWidget(gaussianWeight);

        Mean = new QRadioButton(tabCurvature);
        Mean->setObjectName(QStringLiteral("Mean"));
        Mean->setEnabled(true);

        Curvatures->addWidget(Mean);

        meanWeight = new QSlider(tabCurvature);
        meanWeight->setObjectName(QStringLiteral("meanWeight"));
        meanWeight->setMinimum(1);
        meanWeight->setValue(1);
        meanWeight->setOrientation(Qt::Horizontal);

        Curvatures->addWidget(meanWeight);


        verticalLayout->addLayout(Curvatures);

        verticalSpacer = new QSpacerItem(20, 475, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(tabCurvature, QString());
        tabSmooth = new QWidget();
        tabSmooth->setObjectName(QStringLiteral("tabSmooth"));
        verticalLayout_2 = new QVBoxLayout(tabSmooth);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        defaultButton_2 = new QPushButton(tabSmooth);
        defaultButton_2->setObjectName(QStringLiteral("defaultButton_2"));

        verticalLayout_2->addWidget(defaultButton_2);

        line_2 = new QFrame(tabSmooth);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        iterativeLabel = new QLabel(tabSmooth);
        iterativeLabel->setObjectName(QStringLiteral("iterativeLabel"));

        verticalLayout_2->addWidget(iterativeLabel);

        LaplacianPush = new QPushButton(tabSmooth);
        LaplacianPush->setObjectName(QStringLiteral("LaplacianPush"));

        verticalLayout_2->addWidget(LaplacianPush);

        LaplacianCotgPush = new QPushButton(tabSmooth);
        LaplacianCotgPush->setObjectName(QStringLiteral("LaplacianCotgPush"));

        verticalLayout_2->addWidget(LaplacianCotgPush);

        LmbdaLayout = new QHBoxLayout();
        LmbdaLayout->setSpacing(6);
        LmbdaLayout->setObjectName(QStringLiteral("LmbdaLayout"));
        lambdaLabel = new QLabel(tabSmooth);
        lambdaLabel->setObjectName(QStringLiteral("lambdaLabel"));

        LmbdaLayout->addWidget(lambdaLabel);

        lambdaSlider = new QSlider(tabSmooth);
        lambdaSlider->setObjectName(QStringLiteral("lambdaSlider"));
        lambdaSlider->setMaximum(100);
        lambdaSlider->setValue(100);
        lambdaSlider->setOrientation(Qt::Horizontal);

        LmbdaLayout->addWidget(lambdaSlider);


        verticalLayout_2->addLayout(LmbdaLayout);

        iterativeSelection = new QComboBox(tabSmooth);
        iterativeSelection->setObjectName(QStringLiteral("iterativeSelection"));

        verticalLayout_2->addWidget(iterativeSelection);

        line = new QFrame(tabSmooth);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        globalLabel = new QLabel(tabSmooth);
        globalLabel->setObjectName(QStringLiteral("globalLabel"));

        verticalLayout_2->addWidget(globalLabel);

        globalSmoothPush = new QPushButton(tabSmooth);
        globalSmoothPush->setObjectName(QStringLiteral("globalSmoothPush"));

        verticalLayout_2->addWidget(globalSmoothPush);

        verticalSpacer_2 = new QSpacerItem(20, 421, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        tabWidget->addTab(tabSmooth, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));

        verticalLayout_4->addLayout(verticalLayout_3);

        tabWidget->addTab(tab, QString());

        horizontalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());
        menu_File->addAction(action_Open);
        menu_File->addAction(action_Quit);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        action_Quit->setText(QApplication::translate("MainWindow", "&Quit", Q_NULLPTR));
        action_Open->setText(QApplication::translate("MainWindow", "&Open", Q_NULLPTR));
        defaultButton->setText(QApplication::translate("MainWindow", "Default", Q_NULLPTR));
        checkBoxFill->setText(QApplication::translate("MainWindow", "Fill Triangles", Q_NULLPTR));
        checkBoxRefl->setText(QApplication::translate("MainWindow", "Reflection Lines", Q_NULLPTR));
        Curvature->setText(QApplication::translate("MainWindow", "Curvatures", Q_NULLPTR));
        ComputeCurvature->setText(QApplication::translate("MainWindow", "Curvatures", Q_NULLPTR));
        Gaussian->setText(QApplication::translate("MainWindow", "Gaussian", Q_NULLPTR));
        Mean->setText(QApplication::translate("MainWindow", "Mean", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabCurvature), QApplication::translate("MainWindow", "Render", Q_NULLPTR));
        defaultButton_2->setText(QApplication::translate("MainWindow", "Default", Q_NULLPTR));
        iterativeLabel->setText(QApplication::translate("MainWindow", "Iterative Smoothing", Q_NULLPTR));
        LaplacianPush->setText(QApplication::translate("MainWindow", "Laplacian Uniform", Q_NULLPTR));
        LaplacianCotgPush->setText(QApplication::translate("MainWindow", "Laplacian Cotang", Q_NULLPTR));
        lambdaLabel->setText(QApplication::translate("MainWindow", "Lambda", Q_NULLPTR));
        iterativeSelection->clear();
        iterativeSelection->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Normal", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Bilaplacian", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Taubin", Q_NULLPTR)
        );
        globalLabel->setText(QApplication::translate("MainWindow", "Global Smoothing", Q_NULLPTR));
        globalSmoothPush->setText(QApplication::translate("MainWindow", "Smooth", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSmooth), QApplication::translate("MainWindow", "Smooth", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Page", Q_NULLPTR));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
