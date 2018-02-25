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

#include "qofonomtksettings.h"
#include "dbus/ofonomtksettings.h"

class QOfonoMtkSettingsPrivate
{
public:
    QOfonoMtkSettingsPrivate();
    QString modemPath;
    OfonoMtkSettings *mtkSettings;
    QVariantMap properties;

};

QOfonoMtkSettingsPrivate::QOfonoMtkSettingsPrivate() :
    modemPath(QString())
  , mtkSettings(0)
{
}

QOfonoMtkSettings::QOfonoMtkSettings(QObject *parent) :
    QObject(parent)
  , d_ptr(new QOfonoMtkSettingsPrivate)
{
}

QOfonoMtkSettings::~QOfonoMtkSettings()
{
    delete d_ptr;
}

void QOfonoMtkSettings::setModemPath(const QString &path)
{
    if (path == d_ptr->modemPath ||
            path.isEmpty())
        return;

    QStringList removedProperties = d_ptr->properties.keys();

    delete d_ptr->mtkSettings;
    d_ptr->mtkSettings = new OfonoMtkSettings("org.ofono", path, QDBusConnection::systemBus(),this);

    if (d_ptr->mtkSettings->isValid()) {
        d_ptr->modemPath = path;

        connect(d_ptr->mtkSettings,SIGNAL(PropertyChanged(QString,QDBusVariant)),
                this,SLOT(propertyChanged(QString,QDBusVariant)));

        QVariantMap properties = d_ptr->mtkSettings->GetProperties().value();
        for (QVariantMap::ConstIterator it = properties.constBegin();
             it != properties.constEnd(); ++it) {
            updateProperty(it.key(), it.value());
            removedProperties.removeOne(it.key());
        }

        Q_EMIT modemPathChanged(path);
    }

    foreach (const QString &p, removedProperties)
        updateProperty(p, QVariant());
}

QString QOfonoMtkSettings::modemPath() const
{
    return d_ptr->modemPath;
}


void QOfonoMtkSettings::propertyChanged(const QString& property, const QDBusVariant& dbusvalue)
{
    updateProperty(property, dbusvalue.variant());
}

void QOfonoMtkSettings::updateProperty(const QString &property, const QVariant &value)
{
    if (d_ptr->properties.value(property) == value)
        return;

    if (value.isValid())
        d_ptr->properties.insert(property, value);
    else
        d_ptr->properties.remove(property);

    if (property == QLatin1String("Has3G")) {
        Q_EMIT has3GChanged(value.value<bool>());
    }
}

bool QOfonoMtkSettings::has3G() const
{
    return d_ptr->properties["Has3G"].value<bool>();
}

void QOfonoMtkSettings::setHas3G(bool has3G)
{
    // We do not allow this property to be set to false
    if (!has3G)
        return;

    if (d_ptr->mtkSettings)
        d_ptr->mtkSettings->SetProperty("Has3G", QDBusVariant(has3G));
}

bool QOfonoMtkSettings::isValid() const
{
    return d_ptr->mtkSettings->isValid();
}
