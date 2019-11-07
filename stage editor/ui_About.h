#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

class Ui_aboutDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QSpacerItem *spacerItem1;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_5;

    void setupUi(QDialog *aboutDialog)
    {
    aboutDialog->setObjectName(QString::fromUtf8("aboutDialog"));
    aboutDialog->resize(QSize(395, 300).expandedTo(aboutDialog->minimumSizeHint()));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(aboutDialog->sizePolicy().hasHeightForWidth());
    aboutDialog->setSizePolicy(sizePolicy);
    aboutDialog->setMinimumSize(QSize(395, 300));
    aboutDialog->setMaximumSize(QSize(395, 300));
    aboutDialog->setAcceptDrops(false);
    aboutDialog->setSizeGripEnabled(false);
    aboutDialog->setModal(true);
    layoutWidget = new QWidget(aboutDialog);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(20, 250, 351, 33));
    hboxLayout = new QHBoxLayout(layoutWidget);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    okButton = new QPushButton(layoutWidget);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    hboxLayout->addWidget(okButton);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);

    label_2 = new QLabel(aboutDialog);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(140, 90, 121, 16));
    label_2->setAlignment(Qt::AlignCenter);
    label = new QLabel(aboutDialog);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(130, 20, 151, 47));
    QFont font;
    font.setFamily(QString::fromUtf8("Arial Black"));
    font.setPointSize(20);
    font.setBold(false);
    font.setItalic(false);
    font.setUnderline(false);
    font.setWeight(50);
    font.setStrikeOut(false);
    label->setFont(font);
    label->setTextFormat(Qt::RichText);
    label->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
    label_4 = new QLabel(aboutDialog);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 140, 341, 21));
    label_3 = new QLabel(aboutDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(160, 110, 81, 16));
    label_3->setAlignment(Qt::AlignCenter);
    label_5 = new QLabel(aboutDialog);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 170, 362, 71));
    retranslateUi(aboutDialog);
    QObject::connect(okButton, SIGNAL(clicked()), aboutDialog, SLOT(accept()));

    QMetaObject::connectSlotsByName(aboutDialog);
    } // setupUi

    void retranslateUi(QDialog *aboutDialog)
    {
    aboutDialog->setWindowTitle(QApplication::translate("aboutDialog", "About", 0, QApplication::UnicodeUTF8));
    okButton->setText(QApplication::translate("aboutDialog", "OK", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("aboutDialog", "Stage Builder", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("aboutDialog", "CalcKing", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("aboutDialog", "Lee Ungju <a href=#>ehei0001@gmail.com</a> 2006, July", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("aboutDialog", "0.90a", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("aboutDialog", "License Information<br><br>OGRE3D 1.2.1 Dagon present by <a href=http://www.ogre3d.org target=_blank>The OGRE Team</a> under LGPL<br>Qt 4.1.3 Open Source Edition present by <a href=http://www.trolltech.com>Trolltech</a> under GPL", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(aboutDialog);
    } // retranslateUi

};

namespace Ui {
    class aboutDialog: public Ui_aboutDialog {};
} // namespace Ui

#endif // UI_ABOUT_H
