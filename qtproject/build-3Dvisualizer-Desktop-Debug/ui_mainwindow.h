/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpenFile;
    QAction *actionPredefinida;
    QAction *actionGurro;
    QAction *actionPhong;
    QAction *actionPhong_with_Normal;
    QAction *action_Screenshot;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    GLWidget *widget;
    QTabWidget *tabWidget;
    QWidget *Shaders;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QComboBox *ShaderOptions;
    QWidget *Misc;
    QPushButton *ChangeBackground;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menuArquivo;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(908, 600);
        MainWindow->setMinimumSize(QSize(800, 600));
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 56, 70);\n"
"color: rgb(255, 255, 255);"));
        actionOpenFile = new QAction(MainWindow);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionPredefinida = new QAction(MainWindow);
        actionPredefinida->setObjectName(QString::fromUtf8("actionPredefinida"));
        actionGurro = new QAction(MainWindow);
        actionGurro->setObjectName(QString::fromUtf8("actionGurro"));
        actionPhong = new QAction(MainWindow);
        actionPhong->setObjectName(QString::fromUtf8("actionPhong"));
        actionPhong_with_Normal = new QAction(MainWindow);
        actionPhong_with_Normal->setObjectName(QString::fromUtf8("actionPhong_with_Normal"));
        action_Screenshot = new QAction(MainWindow);
        action_Screenshot->setObjectName(QString::fromUtf8("action_Screenshot"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        widget = new GLWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(600, 0));
        widget->setFocusPolicy(Qt::StrongFocus);

        horizontalLayout->addWidget(widget);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(0, 0));
        Shaders = new QWidget();
        Shaders->setObjectName(QString::fromUtf8("Shaders"));
        verticalLayoutWidget = new QWidget(Shaders);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(40, 70, 191, 171));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        ShaderOptions = new QComboBox(verticalLayoutWidget);
        ShaderOptions->addItem(QString());
        ShaderOptions->addItem(QString());
        ShaderOptions->addItem(QString());
        ShaderOptions->addItem(QString());
        ShaderOptions->setObjectName(QString::fromUtf8("ShaderOptions"));

        verticalLayout_2->addWidget(ShaderOptions);

        tabWidget->addTab(Shaders, QString());
        Misc = new QWidget();
        Misc->setObjectName(QString::fromUtf8("Misc"));
        ChangeBackground = new QPushButton(Misc);
        ChangeBackground->setObjectName(QString::fromUtf8("ChangeBackground"));
        ChangeBackground->setGeometry(QRect(20, 180, 251, 27));
        tabWidget->addTab(Misc, QString());

        horizontalLayout->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 908, 24));
        menuArquivo = new QMenu(menubar);
        menuArquivo->setObjectName(QString::fromUtf8("menuArquivo"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuArquivo->menuAction());
        menuArquivo->addAction(actionOpenFile);
        menuArquivo->addAction(action_Screenshot);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), MainWindow, SLOT(close()));
        QObject::connect(actionOpenFile, SIGNAL(triggered()), widget, SLOT(showFileOpenDialog()));
        QObject::connect(widget, SIGNAL(statusBarMessage(QString)), statusbar, SLOT(showMessage(QString)));
        QObject::connect(action_Screenshot, SIGNAL(triggered()), widget, SLOT(takeScreenshot()));
        QObject::connect(ShaderOptions, SIGNAL(currentTextChanged(QString)), widget, SLOT(changeShader(QString)));
        QObject::connect(ChangeBackground, SIGNAL(pressed()), widget, SLOT(chooseBackgroundColor()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpenFile->setText(QCoreApplication::translate("MainWindow", "&Open", nullptr));
        actionPredefinida->setText(QCoreApplication::translate("MainWindow", "Predefinida 1", nullptr));
        actionGurro->setText(QCoreApplication::translate("MainWindow", "&Gurro (Default)", nullptr));
#if QT_CONFIG(tooltip)
        actionGurro->setToolTip(QCoreApplication::translate("MainWindow", "Gurro (Default)", nullptr));
#endif // QT_CONFIG(tooltip)
        actionPhong->setText(QCoreApplication::translate("MainWindow", "&Phong", nullptr));
        actionPhong_with_Normal->setText(QCoreApplication::translate("MainWindow", "&Normal Texture", nullptr));
#if QT_CONFIG(tooltip)
        actionPhong_with_Normal->setToolTip(QCoreApplication::translate("MainWindow", "Phong Normal", nullptr));
#endif // QT_CONFIG(tooltip)
        action_Screenshot->setText(QCoreApplication::translate("MainWindow", "&Screenshot", nullptr));
        ShaderOptions->setItemText(0, QCoreApplication::translate("MainWindow", "Gouraud", nullptr));
        ShaderOptions->setItemText(1, QCoreApplication::translate("MainWindow", "Texture", nullptr));
        ShaderOptions->setItemText(2, QCoreApplication::translate("MainWindow", "Normal", nullptr));
        ShaderOptions->setItemText(3, QCoreApplication::translate("MainWindow", "Phong", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(Shaders), QCoreApplication::translate("MainWindow", "Shaders", nullptr));
        ChangeBackground->setText(QCoreApplication::translate("MainWindow", "Change background color", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Misc), QCoreApplication::translate("MainWindow", "Misc", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        menuArquivo->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
