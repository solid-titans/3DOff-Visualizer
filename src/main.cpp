#include <QApplication>
#include <QLabel>
#include "ui_dialog.cpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QDialog widget;

    Ui::Dialog ui;
    ui.setupUi(&widget);

    widget.show();

    return app.exec();
}