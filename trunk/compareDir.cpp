
#include <QApplication>

#include "CDMainWindow.h"
#include "CDMainWindowNew.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CDMainWindowNew cd;
    app.setActiveWindow(&cd);
    cd.show(); /// 终于知道show和exec的区别了

    return app.exec();
}
