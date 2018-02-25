/*
 *
 * Copyright (C) 2014 Canonical Ltd.
 * Contact: Jonas G. Drange <jonas.drange@canonical.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <QtTest/QtTest>
#include <QtCore/QObject>

#include "../../../src/qofonomtksettings.h"

#include <QtDebug>

class TestQOfonoMtkSettings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        m = new QOfonoMtkSettings(this);
        m->setModemPath("/phonesim");
        QCOMPARE(m->isValid(), true);

        QEXPECT_FAIL("", "mtk settings is not supported by phonesim", Abort);
        QVERIFY(false);
    }

    void testQOfonoMtkSettings()
    {
        QSignalSpy has3G(m, SIGNAL(has3GChanged(bool)));

        qDebug() << "has3G():" << m->has3G();
    }

    void cleanupTestCase()
    {

    }

private:
    QOfonoMtkSettings *m;
};

QTEST_MAIN(TestQOfonoMtkSettings)
#include "tst_qofonomtksettings.moc"
