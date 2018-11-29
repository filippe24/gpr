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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
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
    QWidget *tabRender;
    QVBoxLayout *verticalLayout;
    QPushButton *defaultButton;
    QCheckBox *checkBoxFill;
    QCheckBox *checkBoxRefl;
    QLabel *label;
    QVBoxLayout *Curvatures;
    QPushButton *ComputeCurvature;
    QRadioButton *Gaussian;
    QRadioButton *Mean;
    QPushButton *LaplacianPush;
    QSpacerItem *verticalSpacer;
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
        tabRender = new QWidget();
        tabRender->setObjectName(QStringLiteral("tabRender"));
        verticalLayout = new QVBoxLayout(tabRender);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        defaultButton = new QPushButton(tabRender);
        defaultButton->setObjectName(QStringLiteral("defaultButton"));

        verticalLayout->addWidget(defaultButton);

        checkBoxFill = new QCheckBox(tabRender);
        checkBoxFill->setObjectName(QStringLiteral("checkBoxFill"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(checkBoxFill->sizePolicy().hasHeightForWidth());
        checkBoxFill->setSizePolicy(sizePolicy2);
        checkBoxFill->setChecked(true);

        verticalLayout->addWidget(checkBoxFill);

        checkBoxRefl = new QCheckBox(tabRender);
        checkBoxRefl->setObjectName(QStringLiteral("checkBoxRefl"));
        checkBoxRefl->setEnabled(true);

        verticalLayout->addWidget(checkBoxRefl);

        label = new QLabel(tabRender);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        Curvatures = new QVBoxLayout();
        Curvatures->setSpacing(6);
        Curvatures->setObjectName(QStringLiteral("Curvatures"));
        ComputeCurvature = new QPushButton(tabRender);
        ComputeCurvature->setObjectName(QStringLiteral("ComputeCurvature"));

        Curvatures->addWidget(ComputeCurvature);

        Gaussian = new QRadioButton(tabRender);
        Gaussian->setObjectName(QStringLiteral("Gaussian"));
        Gaussian->setChecked(true);

        Curvatures->addWidget(Gaussian);

        Mean = new QRadioButton(tabRender);
        Mean->setObjectName(QStringLiteral("Mean"));
        Mean->setEnabled(true);

        Curvatures->addWidget(Mean);


        verticalLayout->addLayout(Curvatures);

        LaplacianPush = new QPushButton(tabRender);
        LaplacianPush->setObjectName(QStringLiteral("LaplacianPush"));

        verticalLayout->addWidget(LaplacianPush);

        verticalSpacer = new QSpacerItem(20, 475, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(tabRender, QString());

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

        tabWidget->setCurrentIndex(0);


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
        label->setText(QApplication::translate("MainWindow", "Curvatures", Q_NULLPTR));
        ComputeCurvature->setText(QApplication::translate("MainWindow", "Curvatures", Q_NULLPTR));
        Gaussian->setText(QApplication::translate("MainWindow", "Gaussian", Q_NULLPTR));
        Mean->setText(QApplication::translate("MainWindow", "Mean", Q_NULLPTR));
        LaplacianPush->setText(QApplication::translate("MainWindow", "Laplacian", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabRender), QApplication::translate("MainWindow", "Render", Q_NULLPTR));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
