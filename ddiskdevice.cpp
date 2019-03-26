/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "ddiskdevice.h"
#include "udisks2_interface.h"

class DDiskDevicePrivate
{
public:
    OrgFreedesktopUDisks2DriveInterface *dbus = nullptr;
};

DDiskDevice::DDiskDevice(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new DDiskDevicePrivate())
{
    d_ptr->dbus = new OrgFreedesktopUDisks2DriveInterface(UDISKS2_SERVICE, path, QDBusConnection::systemBus(), this);
}

DDiskDevice::~DDiskDevice()
{

}

QString DDiskDevice::path() const
{
    Q_D(const DDiskDevice);

    return d->dbus->path();
}

bool DDiskDevice::canPowerOff() const
{
    return d_ptr->dbus->canPowerOff();
}

QVariantMap DDiskDevice::configuration() const
{
    return d_ptr->dbus->configuration();
}

QString DDiskDevice::connectionBus() const
{
    return d_ptr->dbus->connectionBus();
}

bool DDiskDevice::ejectable() const
{
    return d_ptr->dbus->ejectable();
}

QString DDiskDevice::id() const
{
    return d_ptr->dbus->id();
}

QString DDiskDevice::media() const
{
    return d_ptr->dbus->media();
}

bool DDiskDevice::mediaAvailable() const
{
    return d_ptr->dbus->mediaAvailable();
}

bool DDiskDevice::mediaChangeDetected() const
{
    return d_ptr->dbus->mediaChangeDetected();
}

QStringList DDiskDevice::mediaCompatibility() const
{
    return d_ptr->dbus->mediaCompatibility();
}

bool DDiskDevice::mediaRemovable() const
{
    return d_ptr->dbus->mediaRemovable();
}

QString DDiskDevice::model() const
{
    return d_ptr->dbus->model();
}

bool DDiskDevice::optical() const
{
    return d_ptr->dbus->optical();
}

bool DDiskDevice::opticalBlank() const
{
    return d_ptr->dbus->opticalBlank();
}

uint DDiskDevice::opticalNumAudioTracks() const
{
    return d_ptr->dbus->opticalNumAudioTracks();
}

uint DDiskDevice::opticalNumDataTracks() const
{
    return d_ptr->dbus->opticalNumDataTracks();
}

uint DDiskDevice::opticalNumSessions() const
{
    return d_ptr->dbus->opticalNumSessions();
}

uint DDiskDevice::opticalNumTracks() const
{
    return d_ptr->dbus->opticalNumTracks();
}

bool DDiskDevice::removable() const
{
    return d_ptr->dbus->removable();
}

QString DDiskDevice::revision() const
{
    return d_ptr->dbus->revision();
}

int DDiskDevice::rotationRate() const
{
    return d_ptr->dbus->rotationRate();
}

QString DDiskDevice::seat() const
{
    return d_ptr->dbus->seat();
}

QString DDiskDevice::serial() const
{
    return d_ptr->dbus->serial();
}

QString DDiskDevice::siblingId() const
{
    return d_ptr->dbus->siblingId();
}

qulonglong DDiskDevice::size() const
{
    return d_ptr->dbus->size();
}

QString DDiskDevice::sortKey() const
{
    return d_ptr->dbus->sortKey();
}

qulonglong DDiskDevice::timeDetected() const
{
    return d_ptr->dbus->timeDetected();
}

qulonglong DDiskDevice::timeMediaDetected() const
{
    return d_ptr->dbus->timeMediaDetected();
}

QString DDiskDevice::vendor() const
{
    return d_ptr->dbus->vendor();
}

QString DDiskDevice::WWN() const
{
    return d_ptr->dbus->wWN();
}

void DDiskDevice::eject(const QVariantMap &options)
{
    d_ptr->dbus->Eject(options);
}

void DDiskDevice::powerOff(const QVariantMap &options)
{
    d_ptr->dbus->PowerOff(options);
}

void DDiskDevice::setConfiguration(const QVariantMap &value, const QVariantMap &options)
{
    d_ptr->dbus->SetConfiguration(value, options);
}
