#ifndef UTILS_H
#define UTILS_H

#include <QVariantMap>
#include <QString>
#include <QStringList>

class Utils
{

public:
    Utils();

    static QVariantMap readArguments(QStringList arguments);
};

#endif // UTILS_H
