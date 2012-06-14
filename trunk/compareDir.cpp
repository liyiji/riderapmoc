
#include <QApplication>

#include <CDMainWindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CDMainWindow cd;
    app.setActiveWindow(&cd);
    cd.show(); /// 终于知道show和exec的区别了

    return app.exec();
}
