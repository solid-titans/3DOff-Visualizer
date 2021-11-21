#include "mainwindow.h"

#include <QApplication>
#include <QGLFormat>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers(true);
    format.setSamples(8);

    if(!format.sampleBuffers())
        qWarning("Multisample buffer is not supported");

    QGLFormat::setDefaultFormat(format);

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "3Dvisualizer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
