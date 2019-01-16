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
#include <QtWidgets/QDoubleSpinBox>
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
#include <QtWidgets/QSpinBox>
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
    QPushButton *globalSmoothCotangPush;
    QHBoxLayout *percentageFixed;
    QLabel *percentageLabel;
    QSpinBox *percentageSpinBox;
    QLabel *label;
    QFrame *line_3;
    QLabel *magnifyLabel;
    QPushButton *magnifyUniform;
    QPushButton *magnifyCotangent;
    QHBoxLayout *iterations1;
    QLabel *iteration1Label;
    QSpinBox *iteration1Spin;
    QHBoxLayout *iterations2;
    QLabel *iteration2Label;
    QSpinBox *iteration2Spin;
    QHBoxLayout *lambdaMag;
    QLabel *lambdaMagLabel;
    QDoubleSpinBox *lambdaMagSpin;
    QSpacerItem *verticalSpacer_2;
    QTabWidget *tabWidget_2;
    QWidget *tabParametrization;
    QVBoxLayout *verticalLayout_4;
    QPushButton *parametrizeButton;
    QSpacerItem *verticalSpacer_3;
    QMenuBar *menuBar;
    QMenu *menu_File;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 820);
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
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
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

        globalSmoothCotangPush = new QPushButton(tabSmooth);
        globalSmoothCotangPush->setObjectName(QStringLiteral("globalSmoothCotangPush"));

        verticalLayout_2->addWidget(globalSmoothCotangPush);

        percentageFixed = new QHBoxLayout();
        percentageFixed->setSpacing(6);
        percentageFixed->setObjectName(QStringLiteral("percentageFixed"));
        percentageLabel = new QLabel(tabSmooth);
        percentageLabel->setObjectName(QStringLiteral("percentageLabel"));

        percentageFixed->addWidget(percentageLabel);

        percentageSpinBox = new QSpinBox(tabSmooth);
        percentageSpinBox->setObjectName(QStringLiteral("percentageSpinBox"));
        percentageSpinBox->setValue(30);

        percentageFixed->addWidget(percentageSpinBox);

        label = new QLabel(tabSmooth);
        label->setObjectName(QStringLiteral("label"));

        percentageFixed->addWidget(label);


        verticalLayout_2->addLayout(percentageFixed);

        line_3 = new QFrame(tabSmooth);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_3);

        magnifyLabel = new QLabel(tabSmooth);
        magnifyLabel->setObjectName(QStringLiteral("magnifyLabel"));

        verticalLayout_2->addWidget(magnifyLabel);

        magnifyUniform = new QPushButton(tabSmooth);
        magnifyUniform->setObjectName(QStringLiteral("magnifyUniform"));

        verticalLayout_2->addWidget(magnifyUniform);

        magnifyCotangent = new QPushButton(tabSmooth);
        magnifyCotangent->setObjectName(QStringLiteral("magnifyCotangent"));

        verticalLayout_2->addWidget(magnifyCotangent);

        iterations1 = new QHBoxLayout();
        iterations1->setSpacing(6);
        iterations1->setObjectName(QStringLiteral("iterations1"));
        iteration1Label = new QLabel(tabSmooth);
        iteration1Label->setObjectName(QStringLiteral("iteration1Label"));

        iterations1->addWidget(iteration1Label);

        iteration1Spin = new QSpinBox(tabSmooth);
        iteration1Spin->setObjectName(QStringLiteral("iteration1Spin"));
        iteration1Spin->setMinimum(1);
        iteration1Spin->setMaximum(20);
        iteration1Spin->setValue(15);

        iterations1->addWidget(iteration1Spin);


        verticalLayout_2->addLayout(iterations1);

        iterations2 = new QHBoxLayout();
        iterations2->setSpacing(6);
        iterations2->setObjectName(QStringLiteral("iterations2"));
        iteration2Label = new QLabel(tabSmooth);
        iteration2Label->setObjectName(QStringLiteral("iteration2Label"));

        iterations2->addWidget(iteration2Label);

        iteration2Spin = new QSpinBox(tabSmooth);
        iteration2Spin->setObjectName(QStringLiteral("iteration2Spin"));
        iteration2Spin->setMinimum(1);
        iteration2Spin->setMaximum(5);
        iteration2Spin->setValue(3);

        iterations2->addWidget(iteration2Spin);


        verticalLayout_2->addLayout(iterations2);

        lambdaMag = new QHBoxLayout();
        lambdaMag->setSpacing(6);
        lambdaMag->setObjectName(QStringLiteral("lambdaMag"));
        lambdaMagLabel = new QLabel(tabSmooth);
        lambdaMagLabel->setObjectName(QStringLiteral("lambdaMagLabel"));

        lambdaMag->addWidget(lambdaMagLabel);

        lambdaMagSpin = new QDoubleSpinBox(tabSmooth);
        lambdaMagSpin->setObjectName(QStringLiteral("lambdaMagSpin"));
        lambdaMagSpin->setDecimals(1);
        lambdaMagSpin->setMinimum(-1);
        lambdaMagSpin->setMaximum(3);
        lambdaMagSpin->setValue(0.5);

        lambdaMag->addWidget(lambdaMagSpin);


        verticalLayout_2->addLayout(lambdaMag);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        tabWidget_2 = new QTabWidget(tabSmooth);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tabWidget_2->sizePolicy().hasHeightForWidth());
        tabWidget_2->setSizePolicy(sizePolicy3);
        tabParametrization = new QWidget();
        tabParametrization->setObjectName(QStringLiteral("tabParametrization"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(tabParametrization->sizePolicy().hasHeightForWidth());
        tabParametrization->setSizePolicy(sizePolicy4);
        verticalLayout_4 = new QVBoxLayout(tabParametrization);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
        parametrizeButton = new QPushButton(tabParametrization);
        parametrizeButton->setObjectName(QStringLiteral("parametrizeButton"));

        verticalLayout_4->addWidget(parametrizeButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        tabWidget_2->addTab(tabParametrization, QString());

        verticalLayout_2->addWidget(tabWidget_2);

        tabWidget->addTab(tabSmooth, QString());

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

        tabWidget->setCurrentIndex(1);
        tabWidget_2->setCurrentIndex(0);


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
        globalSmoothPush->setText(QApplication::translate("MainWindow", "Uniform", Q_NULLPTR));
        globalSmoothCotangPush->setText(QApplication::translate("MainWindow", "Cotangent", Q_NULLPTR));
        percentageLabel->setText(QApplication::translate("MainWindow", "Fixed", Q_NULLPTR));
        percentageSpinBox->setSuffix(QString());
        label->setText(QApplication::translate("MainWindow", "%", Q_NULLPTR));
        magnifyLabel->setText(QApplication::translate("MainWindow", "Magnify Details", Q_NULLPTR));
        magnifyUniform->setText(QApplication::translate("MainWindow", "Uniform", Q_NULLPTR));
        magnifyCotangent->setText(QApplication::translate("MainWindow", "Cotangent", Q_NULLPTR));
        iteration1Label->setText(QApplication::translate("MainWindow", "1)  Iterations", Q_NULLPTR));
        iteration2Label->setText(QApplication::translate("MainWindow", "2)  Times x", Q_NULLPTR));
        lambdaMagLabel->setText(QApplication::translate("MainWindow", "Lambda", Q_NULLPTR));
        parametrizeButton->setText(QApplication::translate("MainWindow", "Parametrize", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabParametrization), QApplication::translate("MainWindow", "Parametrization", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSmooth), QApplication::translate("MainWindow", "Smooth", Q_NULLPTR));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
