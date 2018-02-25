/****************************************************************************
**
** Copyright (C) 2014 Canonical Ltd.
** Contact: jonas.drange@canonical.com
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#ifndef QOFONOMtkSettings_H
#define QOFONOMtkSettings_H

#include <QObject>
#include <QDBusVariant>

#include "qofono_global.h"
//! This class is used to access ofono mtk (MediaTek) settings API
/*!
 * The API is documented in
 * https://github.com/rilmodem/ofono/blob/master/doc/mtk-settings-api.txt
 */

class QOfonoMtkSettingsPrivate;
class QOFONOSHARED_EXPORT QOfonoMtkSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString modemPath READ modemPath WRITE setModemPath NOTIFY modemPathChanged)
    Q_PROPERTY(bool has3G READ has3G WRITE setHas3G NOTIFY has3GChanged)

public:
    explicit QOfonoMtkSettings(QObject *parent = 0);
    ~QOfonoMtkSettings();

    QString modemPath() const;
    void setModemPath(const QString &path);

    bool has3G() const;
    void setHas3G(bool has3G);

    bool isValid() const;
Q_SIGNALS:
    void has3GChanged(bool has3G);
    void modemPathChanged(const QString &path);

public slots:

private:
    void updateProperty(const QString &property, const QVariant &value);

    QOfonoMtkSettingsPrivate *d_ptr;
private slots:
    void propertyChanged(const QString &property,const QDBusVariant &value);
};

#endif // QOFONOMtkSettings_H
