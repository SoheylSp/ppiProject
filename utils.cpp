#include "utils.h"


Utils::Utils()
{

}

QVariantMap Utils::readArguments(QStringList arguments)
{
    QVariantMap argumentsMap;

    for (int i = 1; i < arguments.length(); ++i) {
        QString argument = arguments[i];

        if (argument.startsWith("--")) {
            QString key = argument.mid(2);
            if (i + 1 < arguments.length()) {
                QString value = arguments[i + 1];
                if (value.startsWith("--")) {
                    argumentsMap[key] = true;
                } else {
                    argumentsMap[key] = value;
                    i++;
                }
            } else {
                argumentsMap[key] = true;
            }
        }
    }

    return argumentsMap;
}
