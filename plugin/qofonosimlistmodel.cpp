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

#include "qofonosimlistmodel.h"
#include <QQmlEngine>

QOfonoSimListModel::QOfonoSimListModel(QObject *parent) :
    QAbstractListModel(parent),
    simWatcher(new QOfonoSimWatcher(this))
{
    simWatcher->setRequireSubscriberIdentity(true);
    simList = simWatcher->presentSimList();
    connect(simWatcher,
        SIGNAL(validChanged()),
        SIGNAL(validChanged()));
    connect(simWatcher,
        SIGNAL(presentSimListChanged()),
        SLOT(onPresentSimListChanged()));
}

bool QOfonoSimListModel::valid() const
{
    return simWatcher->isValid();
}

int QOfonoSimListModel::count() const
{
    return simList.count();
}

bool QOfonoSimListModel::requireSubscriberIdentity() const
{
    return simWatcher->requireSubscriberIdentity();
}

void QOfonoSimListModel::setRequireSubscriberIdentity(bool require)
{
    if (simWatcher->requireSubscriberIdentity() != require) {
        simWatcher->setRequireSubscriberIdentity(require);
        Q_EMIT requireSubscriberIdentityChanged();
    }
}

QHash<int,QByteArray> QOfonoSimListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PathRole]                = "path";
    roles[SubscriberIdentityRole]  = "subscriberIdentity";
    roles[MobileCountryCodeRole]   = "mobileCountryCode";
    roles[MobileNetworkCodeRole]   = "mobileNetworkCode";
    roles[ServiceProviderNameRole] = "serviceProviderName";
    roles[SubscriberNumbersRole]   = "subscriberNumbers";
    roles[ServiceNumbersRole]      = "serviceNumbers";
    roles[PinRequiredRole]         = "pinRequired";
    roles[LockedPinsRole]          = "lockedPins";
    roles[CardIdentifierRole]      = "cardIdentifier";
    roles[PreferredLanguagesRole]  = "preferredLanguages";
    roles[PinRetriesRole]          = "pinRetries";
    roles[FixedDialingRole]        = "fixedDialing";
    roles[BarredDialingRole]       = "barredDialing";
    return roles;
}

int QOfonoSimListModel::rowCount(const QModelIndex&) const
{
    return simList.count();
}

QVariant QOfonoSimListModel::data(const QModelIndex& index, int role) const
{
    const int row = index.row();
    if (row >= 0 && row < simList.count()) {
        const QOfonoSimManager* sim = simList.at(row).data();
        switch (role) {
        case PathRole:                return sim->modemPath();
        case SubscriberIdentityRole:  return sim->subscriberIdentity();
        case MobileCountryCodeRole:   return sim->mobileCountryCode();
        case MobileNetworkCodeRole:   return sim->mobileNetworkCode();
        case ServiceProviderNameRole: return sim->serviceProviderName();
        case SubscriberNumbersRole:   return sim->subscriberNumbers();
        case ServiceNumbersRole:      return sim->serviceNumbers();
        case PinRequiredRole:         return sim->pinRequired();
        case LockedPinsRole:          return sim->lockedPins();
        case CardIdentifierRole:      return sim->cardIdentifier();
        case PreferredLanguagesRole:  return sim->preferredLanguages();
        case PinRetriesRole:          return sim->pinRetries();
        case FixedDialingRole:        return sim->fixedDialing();
        case BarredDialingRole:       return sim->barredDialing();
        }
    } else {
        qWarning() << index << role;
    }
    return QVariant();
}

bool QOfonoSimListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    const int row = index.row();
    if (row >= 0 && row < simList.count() && role == SubscriberNumbersRole) {
        simList.at(row)->setSubscriberNumbers(value.toStringList());
        return true;
    } else {
        qWarning() << index << role;
    }
    return false;
}

int QOfonoSimListModel::indexOf(QOfonoSimManager* sim)
{
    // We cann't just do simList.indexOf(sim) because that would create temporary
    // QOfonoSimManager::SharedPointer on stack, pass it to simList.indexOf, and
    // eventually destroy QOfonoSimManager.
    const int n = simList.count();
    for (int i=0; i<n; i++) {
        if (simList.at(i).data() == sim) {
            return i;
        }
    }
    return -1;
}

void QOfonoSimListModel::simPropertyChanged(Role role)
{
    int row = indexOf((QOfonoSimManager*)sender());
    if (row >= 0) {
        QModelIndex modelIndex = index(row);
        QVector<int> roles;
        roles.append(role);
        Q_EMIT dataChanged(modelIndex, modelIndex, roles);
    }
}

void QOfonoSimListModel::onPresentSimListChanged()
{
    QList<QOfonoSimManager::SharedPointer> newList = simWatcher->presentSimList();
    const bool countHasChanged = simList.count() != simWatcher->presentSimCount();
    int i;
    for (i=simList.count()-1; i>=0; i--) {
        QOfonoSimManager::SharedPointer sim = simList.at(i);
        if (!newList.contains(sim)) {
            beginRemoveRows(QModelIndex(), i, i);
            sim->disconnect(this);
            simList.removeAt(i);
            endRemoveRows();
            Q_EMIT simRemoved(sim.data());
        }
    }

    QList<QOfonoSimManager::SharedPointer> oldList = simList;
    simList = newList;
    for (i=0; i<newList.count(); i++) {
        if (!oldList.contains(newList.at(i))) {
            QOfonoSimManager* sim = newList.at(i).data();
            QQmlEngine::setObjectOwnership(sim, QQmlEngine::CppOwnership);
            beginInsertRows(QModelIndex(), i, i);
            sim->disconnect(this);
            connect(sim,
                SIGNAL(subscriberIdentityChanged(QString)),
                SLOT(onSubscriberIdentityChanged()));
            connect(sim,
                SIGNAL(mobileCountryCodeChanged(QString)),
                SLOT(onMobileCountryCodeChanged()));
            connect(sim,
                SIGNAL(mobileNetworkCodeChanged(QString)),
                SLOT(onMobileNetworkCodeChanged()));
            connect(sim,
                SIGNAL(serviceProviderNameChanged(QString)),
                SLOT(onServiceProviderNameChanged()));
            connect(sim,
                SIGNAL(subscriberNumbersChanged(QStringList)),
                SLOT(onSubscriberNumbersChanged()));
            connect(sim,
                SIGNAL(serviceNumbersChanged(QVariantMap)),
                SLOT(onServiceNumbersChanged()));
            connect(sim,
                SIGNAL(pinRequiredChanged(int)),
                SLOT(onPinRequiredChanged()));
            connect(sim,
                SIGNAL(lockedPinsChanged(QVariantList)),
                SLOT(onLockedPinsChanged()));
            connect(sim,
                SIGNAL(cardIdentifierChanged(QString)),
                SLOT(onCardIdentifierChanged()));
            connect(sim,
                SIGNAL(preferredLanguagesChanged(QStringList)),
                SLOT(onPreferredLanguagesChanged()));
            connect(sim,
                SIGNAL(pinRetriesChanged(QVariantMap)),
                SLOT(onPinRetriesChanged()));
            connect(sim,
                SIGNAL(fixedDialingChanged(bool)),
                SLOT(onFixedDialingChanged()));
            connect(sim,
                SIGNAL(barredDialingChanged(bool)),
                SLOT(onBarredDialingChanged()));
            endInsertRows();
            Q_EMIT simAdded(sim);
        }
    }

    if (countHasChanged) {
        Q_EMIT countChanged();
    }
}

void QOfonoSimListModel::onSubscriberIdentityChanged()
{
    simPropertyChanged(SubscriberIdentityRole);
}

void QOfonoSimListModel::onMobileCountryCodeChanged()
{
    simPropertyChanged(MobileCountryCodeRole);
}

void QOfonoSimListModel::onMobileNetworkCodeChanged()
{
    simPropertyChanged(MobileNetworkCodeRole);
}

void QOfonoSimListModel::onServiceProviderNameChanged()
{
    simPropertyChanged(ServiceProviderNameRole);
}

void QOfonoSimListModel::onSubscriberNumbersChanged()
{
    simPropertyChanged(SubscriberNumbersRole);
}

void QOfonoSimListModel::onServiceNumbersChanged()
{
    simPropertyChanged(ServiceNumbersRole);
}

void QOfonoSimListModel::onPinRequiredChanged()
{
    simPropertyChanged(PinRequiredRole);
}

void QOfonoSimListModel::onLockedPinsChanged()
{
    simPropertyChanged(LockedPinsRole);
}

void QOfonoSimListModel::onCardIdentifierChanged()
{
    simPropertyChanged(CardIdentifierRole);
}

void QOfonoSimListModel::onPreferredLanguagesChanged()
{
    simPropertyChanged(PreferredLanguagesRole);
}

void QOfonoSimListModel::onPinRetriesChanged()
{
    simPropertyChanged(PinRetriesRole);
}

void QOfonoSimListModel::onFixedDialingChanged()
{
    simPropertyChanged(FixedDialingRole);
}

void QOfonoSimListModel::onBarredDialingChanged()
{
    simPropertyChanged(BarredDialingRole);
}
