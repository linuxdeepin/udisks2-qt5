/*
 * Copyright (C) 2020 UOS Technology Co., Ltd.
 *
 * Author:     zccrs <zhangjide@deepin.com>
 *
 * Maintainer: xushitong<xushitong@uniontech.com>
 *             max-lv<lvwujun@uniontech.com>
 *             zhangsheng<zhangsheng@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dudisksjob.h"
#include "udisks2_interface.h"

#include <QDBusConnection>

class DUDisksJobPrivate
{
    DUDisksJobPrivate(DUDisksJob *)
    {

    }
    DUDisksJob *q_ptr;
    OrgFreedesktopUDisks2JobInterface *dbusif;

    Q_DECLARE_PUBLIC(DUDisksJob)
};

DUDisksJob::~DUDisksJob()
{
}

QString DUDisksJob::path() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->path();
}

QStringList DUDisksJob::objects() const
{
    Q_D(const DUDisksJob);
    QStringList ret;
    for (auto &o : d->dbusif->objects()) {
        ret.push_back(o.path());
    }
    return ret;
}

bool DUDisksJob::cancelable() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->cancelable();
}

bool DUDisksJob::progressValid() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->progressValid();
}

double DUDisksJob::progress() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->progress();
}

QString DUDisksJob::operation() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->operation();
}

quint32 DUDisksJob::startedByUid() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->startedByUID();
}

quint64 DUDisksJob::bytes() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->bytes();
}

quint64 DUDisksJob::expectedEndTime() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->expectedEndTime();
}

quint64 DUDisksJob::rate() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->rate();
}

quint64 DUDisksJob::startTime() const
{
    Q_D(const DUDisksJob);
    return d->dbusif->startTime();
}

void DUDisksJob::cancel(const QVariantMap &options)
{
    Q_D(DUDisksJob);
    d->dbusif->Cancel(options).waitForFinished();
}

DUDisksJob::DUDisksJob(QString path, QObject *parent)
    : QObject(parent)
    , d_ptr(new DUDisksJobPrivate(this))
{
    Q_D(DUDisksJob);
    d->dbusif = new OrgFreedesktopUDisks2JobInterface(UDISKS2_SERVICE, path, QDBusConnection::systemBus());
    QDBusConnection::systemBus().connect(UDISKS2_SERVICE, d->dbusif->path(), "org.freedesktop.DBus.Properties",
                   "PropertiesChanged", this, SLOT(onPropertiesChanged(const QString &, const QVariantMap &)));
    connect(d->dbusif, &OrgFreedesktopUDisks2JobInterface::Completed, this, &DUDisksJob::completed);
}

void DUDisksJob::onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties)
{
    Q_UNUSED(interface)

    auto begin = changed_properties.begin();

    for (; begin != changed_properties.constEnd(); ++begin) {
        QString property_name = begin.key();

        int pindex = this->metaObject()->indexOfProperty(property_name.toLatin1().constData());

        if (pindex < 0) {
            property_name[0] = property_name.at(0).toLower();
            pindex = this->metaObject()->indexOfProperty(property_name.toLatin1().constData());
        }

        if (pindex < 0)
            continue;

        const QMetaProperty &mp = this->metaObject()->property(pindex);

        if (!mp.hasNotifySignal())
            continue;

        mp.notifySignal().invoke(this, QGenericArgument(begin.value().typeName(), begin.value().constData()));
    }
}
