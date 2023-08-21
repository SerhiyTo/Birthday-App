#include "stylehelper.h"

StyleHelper::StyleHelper()
{
}

StyleHelper::~StyleHelper()
{
}

QByteArray StyleHelper::fileRead(const QString &fileName)
{
    QFile fileWithStyles(fileName);
    if (!fileWithStyles.open(QFile::ReadOnly))
    {
        return QByteArray();
    }

    QByteArray data = fileWithStyles.readAll();  // Get all data from file with styles
    fileWithStyles.close();

    return data;
}

QByteArray StyleHelper::inputStyles()
{
    return fileRead(":/styles/input-styles.css");
}

QByteArray StyleHelper::listStyles()
{
    return fileRead(":/styles/list-styles.css");
}

QByteArray StyleHelper::mainStyles()
{
    return fileRead(":/styles/main-styles.css");
}
