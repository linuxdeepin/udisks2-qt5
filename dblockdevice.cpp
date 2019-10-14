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
#include "dblockdevice.h"
#include "private/dblockdevice_p.h"
#include "udisks2_interface.h"
#include "objectmanager_interface.h"

DBlockDevicePrivate::DBlockDevicePrivate(DBlockDevice *qq)
    : q_ptr(qq)
{

}

void DBlockDevice::onInterfacesAdded(const QDBusObjectPath &object_path, const QMap<QString, QVariantMap> &interfaces_and_properties)
{
    Q_D(DBlockDevice);

    const QString &path = object_path.path();

    if (path != d->dbus->path())
        return;

    if (interfaces_and_properties.contains(QStringLiteral(UDISKS2_SERVICE ".Filesystem"))) {
        Q_EMIT hasFileSystemChanged(true);
    }

    if (interfaces_and_properties.contains(QStringLiteral(UDISKS2_SERVICE ".Partition"))) {
        Q_EMIT hasPartitionChanged(true);
    }

    if (interfaces_and_properties.contains(QStringLiteral(UDISKS2_SERVICE ".Encrypted"))) {
        Q_EMIT isEncryptedChanged(true);
    }
}

void DBlockDevice::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces)
{
    Q_D(DBlockDevice);

    const QString &path = object_path.path();

    if (path != d->dbus->path())
        return;

    for (const QString &i : interfaces) {
        if (i == QStringLiteral(UDISKS2_SERVICE ".Filesystem")) {
            Q_EMIT hasFileSystemChanged(false);
        } else if (i == QStringLiteral(UDISKS2_SERVICE ".Partition")) {
            Q_EMIT hasPartitionChanged(false);
        } else if (i == QStringLiteral(UDISKS2_SERVICE ".Encrypted")) {
            Q_EMIT isEncryptedChanged(false);
        }
    }
}

void DBlockDevice::onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties)
{
    if (interface.endsWith(".PartitionTable")) {
        auto begin = changed_properties.begin();

        while (begin != changed_properties.constEnd()) {
            if (begin.key() == "Type") {
                Q_EMIT ptTypeChanged();
                break;
            }

            ++begin;
        }
    } else {
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
}

DBlockDevice::~DBlockDevice()
{

}

bool DBlockDevice::isValid() const
{
    Q_D(const DBlockDevice);

    return d->dbus->isValid();
}

bool DBlockDevice::watchChanges() const
{
    Q_D(const DBlockDevice);

    return d->watchChanges;
}

/*!
 * \brief Get the block device (patition) DBus path.
 *
 * \return the DBus path
 */
QString DBlockDevice::path() const
{
    Q_D(const DBlockDevice);

    return d->dbus->path();
}

QList<QPair<QString, QVariantMap> > DBlockDevice::configuration() const
{
    Q_D(const DBlockDevice);

    return d->dbus->configuration();
}

QString DBlockDevice::cryptoBackingDevice() const
{
    Q_D(const DBlockDevice);

    return d->dbus->cryptoBackingDevice().path();
}

/*!
 * \brief Get the device path, like `/dev/sda1`
 *
 * \return Device path in QByteArray.
 */
QByteArray DBlockDevice::device() const
{
    Q_D(const DBlockDevice);

    return d->dbus->device();
}

qulonglong DBlockDevice::deviceNumber() const
{
    Q_D(const DBlockDevice);

    return d->dbus->deviceNumber();
}

/*!
 * \brief Get the disk device dbus path
 *
 * \return a dbus path in QString
 */
QString DBlockDevice::drive() const
{
    Q_D(const DBlockDevice);

    return d->dbus->drive().path();
}

bool DBlockDevice::hintAuto() const
{
    Q_D(const DBlockDevice);

    return d->dbus->hintAuto();
}

QString DBlockDevice::hintIconName() const
{
    Q_D(const DBlockDevice);

    return d->dbus->hintIconName();
}

bool DBlockDevice::hintIgnore() const
{
    Q_D(const DBlockDevice);

    return d->dbus->hintIgnore();
}

QString DBlockDevice::hintName() const
{
    Q_D(const DBlockDevice);

    return d->dbus->hintName();
}

bool DBlockDevice::hintPartitionable() const
{
    Q_D(const DBlockDevice);

    return d->dbus->hintPartitionable();
}

QString DBlockDevice::hintSymbolicIconName() const
{
    Q_D(const DBlockDevice);

    return d->dbus->hintSymbolicIconName();
}

bool DBlockDevice::hintSystem() const
{
    Q_D(const DBlockDevice);

    return d->dbus->hintSystem();
}

QString DBlockDevice::id() const
{
    Q_D(const DBlockDevice);

    return d->dbus->id();
}

QString DBlockDevice::idLabel() const
{
    Q_D(const DBlockDevice);

    return d->dbus->idLabel();
}

QString DBlockDevice::idType() const
{
    Q_D(const DBlockDevice);

    return d->dbus->idType();
}

DBlockDevice::FSType DBlockDevice::fsType() const
{
    const QString &fs_type = idType();

    if (fs_type.isEmpty())
        return InvalidFS;

    if (fs_type == "hfs+")
        return hfs_plus;

    bool ok = false;
    const QMetaEnum me = QMetaEnum::fromType<FSType>();

    int value = me.keyToValue(fs_type.toLatin1().constData(), &ok);

    if (!ok) {
        return UnknowFS;
    }

    return static_cast<FSType>(value);
}

QString DBlockDevice::idUUID() const
{
    Q_D(const DBlockDevice);

    return d->dbus->idUUID();
}

QString DBlockDevice::idUsage() const
{
    Q_D(const DBlockDevice);

    return d->dbus->idUsage();
}

QString DBlockDevice::idVersion() const
{
    Q_D(const DBlockDevice);

    return d->dbus->idVersion();
}

QString DBlockDevice::mDRaid() const
{
    Q_D(const DBlockDevice);

    return d->dbus->mDRaid().path();
}

QString DBlockDevice::mDRaidMember() const
{
    Q_D(const DBlockDevice);

    return d->dbus->mDRaidMember().path();
}

QByteArray DBlockDevice::preferredDevice() const
{
    Q_D(const DBlockDevice);

    return d->dbus->preferredDevice();
}

bool DBlockDevice::readOnly() const
{
    Q_D(const DBlockDevice);

    return d->dbus->readOnly();
}

qulonglong DBlockDevice::size() const
{
    Q_D(const DBlockDevice);

    return d->dbus->size();
}

QByteArrayList DBlockDevice::symlinks() const
{
    Q_D(const DBlockDevice);

    return d->dbus->symlinks();
}

QStringList DBlockDevice::userspaceMountOptions() const
{
    Q_D(const DBlockDevice);

    return d->dbus->userspaceMountOptions();
}

bool DBlockDevice::hasFileSystem() const
{
    Q_D(const DBlockDevice);

    return hasFileSystem(d->dbus->path());
}

bool DBlockDevice::hasPartition() const
{
    Q_D(const DBlockDevice);

    return hasPartition(d->dbus->path());
}

bool DBlockDevice::isEncrypted() const
{
    Q_D(const DBlockDevice);

    return isEncrypted(d->dbus->path());
}

bool DBlockDevice::isLoopDevice() const
{
    Q_D(const DBlockDevice);

    return UDisks2::interfaceExists(d->dbus->path(), UDISKS2_SERVICE ".Loop");
}

bool DBlockDevice::hasFileSystem(const QString &path)
{
    return UDisks2::interfaceExists(path, UDISKS2_SERVICE ".Filesystem");
}

bool DBlockDevice::hasPartition(const QString &path)
{
    return UDisks2::interfaceExists(path, UDISKS2_SERVICE ".Partition");
}

bool DBlockDevice::isEncrypted(const QString &path)
{
    return UDisks2::interfaceExists(path, UDISKS2_SERVICE ".Encrypted");
}

QByteArrayList DBlockDevice::mountPoints() const
{
    if (!hasFileSystem())
        return QByteArrayList();

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2FilesystemInterface fsif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());
    return fsif.mountPoints();
}

DBlockDevice::PTType DBlockDevice::ptType() const
{
    Q_D(const DBlockDevice);

    if (!UDisks2::interfaceExists(d->dbus->path(), UDISKS2_SERVICE ".PartitionTable")) {
        return InvalidPT;
    }

    OrgFreedesktopUDisks2PartitionTableInterface ptif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());

    const QString &type = ptif.type();

    if (type.isEmpty()) {
        return InvalidPT;
    }

    if (type == "dos") {
        return MBR;
    }

    if (type == "gpt") {
        return GPT;
    }

    return UnknowPT;
}

QList<QPair<QString, QVariantMap> > DBlockDevice::childConfiguration() const
{
    Q_D(const DBlockDevice);

    if (!isEncrypted()) {
        return QList<QPair<QString, QVariantMap>>();
    }

    OrgFreedesktopUDisks2EncryptedInterface eif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());
    return eif.childConfiguration();
}

void DBlockDevice::setWatchChanges(bool watchChanges)
{
    Q_D(DBlockDevice);

    if (d->watchChanges == watchChanges)
        return;

    d->watchChanges = watchChanges;

    OrgFreedesktopDBusObjectManagerInterface *object_manager = UDisks2::objectManager();
    auto sb = QDBusConnection::systemBus();

    if (watchChanges) {
        connect(object_manager, &OrgFreedesktopDBusObjectManagerInterface::InterfacesAdded,
                this, &DBlockDevice::onInterfacesAdded);
        connect(object_manager, &OrgFreedesktopDBusObjectManagerInterface::InterfacesRemoved,
                this, &DBlockDevice::onInterfacesRemoved);

        sb.connect(UDISKS2_SERVICE, d->dbus->path(), "org.freedesktop.DBus.Properties",
                   "PropertiesChanged", this, SLOT(onPropertiesChanged(const QString &, const QVariantMap &)));
    } else {
        disconnect(object_manager, &OrgFreedesktopDBusObjectManagerInterface::InterfacesAdded,
                   this, &DBlockDevice::onInterfacesAdded);
        disconnect(object_manager, &OrgFreedesktopDBusObjectManagerInterface::InterfacesRemoved,
                   this, &DBlockDevice::onInterfacesRemoved);

        sb.disconnect(UDISKS2_SERVICE, d->dbus->path(), "org.freedesktop.DBus.Properties",
                      "PropertiesChanged", this, SLOT(onPropertiesChanged(const QString &, const QVariantMap &)));
    }
}

void DBlockDevice::addConfigurationItem(const QPair<QString, QVariantMap> &item, const QVariantMap &options)
{
    Q_D(DBlockDevice);

    d->dbus->AddConfigurationItem(item, options);
}

void DBlockDevice::format(const QString &type, const QVariantMap &options)
{
    Q_D(DBlockDevice);

    d->dbus->Format(type, options);
}

void DBlockDevice::format(const DBlockDevice::FSType &type, const QVariantMap &options)
{
    if (type < ext2)
        return;

    format(QString::fromLatin1(QMetaEnum::fromType<FSType>().valueToKey(type)), options);
}

QList<QPair<QString, QVariantMap> > DBlockDevice::getSecretConfiguration(const QVariantMap &options)
{
    Q_D(DBlockDevice);

    return d->dbus->GetSecretConfiguration(options);
}

QDBusUnixFileDescriptor DBlockDevice::openDevice(const QString &mode, const QVariantMap &options)
{
    Q_D(DBlockDevice);

    return d->dbus->OpenDevice(mode, options);
}

QDBusUnixFileDescriptor DBlockDevice::openForBackup(const QVariantMap &options)
{
    Q_D(DBlockDevice);

    return d->dbus->OpenForBackup(options);
}

QDBusUnixFileDescriptor DBlockDevice::openForBenchmark(const QVariantMap &options)
{
    Q_D(DBlockDevice);

    return d->dbus->OpenForBenchmark(options);
}

QDBusUnixFileDescriptor DBlockDevice::openForRestore(const QVariantMap &options)
{
    Q_D(DBlockDevice);

    return d->dbus->OpenForRestore(options);
}

void DBlockDevice::removeConfigurationItem(const QPair<QString, QVariantMap> &item, const QVariantMap &options)
{
    Q_D(DBlockDevice);

    d->dbus->RemoveConfigurationItem(item, options);
}

void DBlockDevice::rescan(const QVariantMap &options)
{
    Q_D(DBlockDevice);

    d->dbus->Rescan(options);
}

void DBlockDevice::updateConfigurationItem(const QPair<QString, QVariantMap> &old_item, const QPair<QString, QVariantMap> &new_item, const QVariantMap &options)
{
    Q_D(DBlockDevice);

    d->dbus->UpdateConfigurationItem(old_item, new_item, options);
}

/*!
 * \brief Mount the block device.
 *
 * \param options please refer to \l {http://storaged.org/doc/udisks2-api/latest/gdbus-org.freedesktop.UDisks2.Filesystem.html#gdbus-method-org-freedesktop-UDisks2-Filesystem.Mount} {UDisks2.Filesystem Mount()}
 *
 * \return the mount point path.
 *
 * \sa unmount()
 */
QString DBlockDevice::mount(const QVariantMap &options)
{
    if (!hasFileSystem()) {
        return QString();
    }

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2FilesystemInterface fsif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());

    auto r = fsif.Mount(options);
    r.waitForFinished();
    return r.value();
}

void DBlockDevice::unmount(const QVariantMap &options)
{
    if (!hasFileSystem()) {
        return;
    }

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2FilesystemInterface fsif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());
    fsif.Unmount(options).waitForFinished();
}

/*!
 * \brief Check if we can set the filesystem label.
 *
 * \return
 */
bool DBlockDevice::canSetLabel() const
{
    if (!hasFileSystem()) {
        return false;
    }

    // blumia: Since gvfs can't correctly mount a label-renamed patition
    //         we simply disable rename support if we don't unmount the
    //         patition. Will be add back when we switch to udisks2.
    if (/*fsType() == ntfs && */!mountPoints().isEmpty()) {
        return false;
    }

    return true;
}

/*!
 * \brief Sets the filesystem label.
 *
 * \param options Options (currently unused except for standard options).
 */
void DBlockDevice::setLabel(const QString &label, const QVariantMap &options)
{
    if (!hasFileSystem()) {
        return;
    }

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2FilesystemInterface fsif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());
    fsif.SetLabel(label, options).waitForFinished();
}

void DBlockDevice::changePassphrase(const QString &passphrase, const QString &new_passphrase, const QVariantMap &options)
{
    if (!isEncrypted()) {
        return;
    }

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2EncryptedInterface eif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());
    eif.ChangePassphrase(passphrase, new_passphrase, options).waitForFinished();
}

void DBlockDevice::lock(const QVariantMap &options)
{
    if (!isEncrypted()) {
        return;
    }

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2EncryptedInterface eif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());
    eif.Lock(options).waitForFinished();
}

QString DBlockDevice::unlock(const QString &passphrase, const QVariantMap &options)
{
    if (!isEncrypted()) {
        return QString();
    }

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2EncryptedInterface eif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());

    auto r = eif.Unlock(passphrase, options);
    r.waitForFinished();
    return r.value().path();
}

QString DBlockDevice::cleartextDevice()
{
    if (!isEncrypted()) {
        return QString();
    }

    Q_D(const DBlockDevice);

    OrgFreedesktopUDisks2EncryptedInterface eif(UDISKS2_SERVICE, d->dbus->path(), QDBusConnection::systemBus());
    return eif.cleartextDevice().path();
}

DBlockDevice::DBlockDevice(const QString &path, QObject *parent)
    : DBlockDevice(*new DBlockDevicePrivate(this), path, parent)
{

}

DBlockDevice::DBlockDevice(DBlockDevicePrivate &dd, const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    dd.dbus = new OrgFreedesktopUDisks2BlockInterface(UDISKS2_SERVICE, path, QDBusConnection::systemBus(), this);

    connect(this, &DBlockDevice::idTypeChanged, this, &DBlockDevice::fsTypeChanged);
}
