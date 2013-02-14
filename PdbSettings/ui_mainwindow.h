/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Dec 4 11:04:30 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTreeWidget *m_TreeOfSettings;
    QGroupBox *m_groupBoxDB;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *m_AcceptButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(827, 524);
        MainWindow->setMinimumSize(QSize(827, 524));
        MainWindow->setMaximumSize(QSize(827, 524));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        m_TreeOfSettings = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("Settings"));
        m_TreeOfSettings->setHeaderItem(__qtreewidgetitem);
        m_TreeOfSettings->setObjectName(QString::fromUtf8("m_TreeOfSettings"));
        m_TreeOfSettings->setGeometry(QRect(0, 0, 281, 461));
        m_groupBoxDB = new QGroupBox(centralWidget);
        m_groupBoxDB->setObjectName(QString::fromUtf8("m_groupBoxDB"));
        m_groupBoxDB->setGeometry(QRect(290, 0, 531, 411));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(290, 430, 521, 29));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        m_AcceptButton = new QPushButton(layoutWidget);
        m_AcceptButton->setObjectName(QString::fromUtf8("m_AcceptButton"));

        horizontalLayout->addWidget(m_AcceptButton);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 827, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Pdb settings", 0, QApplication::UnicodeUTF8));
        m_groupBoxDB->setTitle(QString());
        m_AcceptButton->setText(QApplication::translate("MainWindow", "Apply changes", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
