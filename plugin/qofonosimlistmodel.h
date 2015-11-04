/****************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: slava.monich@jolla.com
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

#ifndef QOFONOSIMLISTMODEL_H
#define QOFONOSIMLISTMODEL_H

#include "qofonosimwatcher.h"

class QOfonoSimListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool requireSubscriberIdentity READ requireSubscriberIdentity
        WRITE setRequireSubscriberIdentity NOTIFY requireSubscriberIdentityChanged)

public:
    enum Role {
        PathRole = Qt::UserRole + 1,
        SubscriberIdentityRole,
        MobileCountryCodeRole,
        MobileNetworkCodeRole,
        SubscriberNumbersRole,
        ServiceNumbersRole,
        PinRequiredRole,
        LockedPinsRole,
        CardIdentifierRole,
        PreferredLanguagesRole,
        PinRetriesRole,
        FixedDialingRole,
        BarredDialingRole
    };

    explicit QOfonoSimListModel(QObject* parent = NULL);

    bool valid() const;
    int count() const;
    bool requireSubscriberIdentity() const;
    void setRequireSubscriberIdentity(bool require);

protected:
    QHash<int,QByteArray> roleNames() const;
    int rowCount(const QModelIndex& aParent) const;
    QVariant data(const QModelIndex& aIndex, int aRole) const;
    bool setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole);

Q_SIGNALS:
    void validChanged();
    void countChanged();
    void requireSubscriberIdentityChanged();
    void simAdded(QOfonoSimManager* sim);
    void simRemoved(QOfonoSimManager* sim);

private Q_SLOTS:
    void onPresentSimListChanged();
    void onSubscriberIdentityChanged();
    void onMobileCountryCodeChanged();
    void onMobileNetworkCodeChanged();
    void onSubscriberNumbersChanged();
    void onServiceNumbersChanged();
    void onPinRequiredChanged();
    void onLockedPinsChanged();
    void onCardIdentifierChanged();
    void onPreferredLanguagesChanged();
    void onPinRetriesChanged();
    void onFixedDialingChanged();
    void onBarredDialingChanged();

private:
    void simPropertyChanged(Role role);
    int indexOf(QOfonoSimManager* sim);

private:
    QOfonoSimWatcher *simWatcher;
    QList<QOfonoSimManager::SharedPointer> simList;
};

#endif // QOFONOSIMLISTMODEL_H
