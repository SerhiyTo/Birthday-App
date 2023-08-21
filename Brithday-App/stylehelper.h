#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QByteArray>
#include <QFile>

class StyleHelper
{
public:
    StyleHelper();
    ~StyleHelper();

    static QByteArray fileRead(const QString& fileName);  // Main function, that get styles from file
    static QByteArray inputStyles();  // Function to get styles from input-styles.css
    static QByteArray listStyles();  // Function to get styles from list-styles.css
    static QByteArray mainStyles();  // Function to get styles from main-styles.css
};

#endif // STYLEHELPER_H
