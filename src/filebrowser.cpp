
#include <QProcess>
#include <QFileInfo>
#include <QDir>

void selectInFileBrowser(const QString& path){
#if defined(Q_OS_WIN)
    const QString explorer = "explorer";
        QStringList param;
        if (!QFileInfo(path).isDir())
            param << QLatin1String("/select,");
        param << QDir::toNativeSeparators(path);
        QProcess::startDetached(explorer, param);
#endif



#ifdef Q_OS_LINUX
        const QString explorer = "dolphin";
            QStringList param;
            if (!QFileInfo(path).isDir())
                param << QLatin1String("--select");
            param << QDir::toNativeSeparators(path);
            QProcess::startDetached(explorer, param);
#endif
}


