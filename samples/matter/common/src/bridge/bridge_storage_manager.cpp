/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "bridge_storage_manager.h"

namespace
{
template <class T> bool LoadDataToObject(Nrf::PersistentStorageNode *node, T &data)
{
	size_t readSize = 0;

	bool result = Nrf::PersistentStorage::Instance().Load(node, &data, sizeof(T), readSize);

	return result;
}

Nrf::PersistentStorageNode CreateIndexNode(uint8_t bridgedDeviceIndex, Nrf::PersistentStorageNode *parent)
{
	char index[Nrf::BridgeStorageManager::kMaxIndexLength + 1] = { 0 };

	snprintf(index, sizeof(index), "%d", bridgedDeviceIndex);

	return Nrf::PersistentStorageNode(index, strlen(index), parent);
}

} /* namespace */

namespace Nrf {

bool BridgeStorageManager::StoreBridgedDevicesCount(uint8_t count)
{
	return Nrf::PersistentStorage::Instance().Store(&mBridgedDevicesCount, &count, sizeof(count));
}

bool BridgeStorageManager::LoadBridgedDevicesCount(uint8_t &count)
{
	return LoadDataToObject(&mBridgedDevicesCount, count);
}

bool BridgeStorageManager::StoreBridgedDevicesIndexes(uint8_t *indexes, uint8_t count)
{
	if (!indexes) {
		return false;
	}

	return Nrf::PersistentStorage::Instance().Store(&mBridgedDevicesIndexes, indexes, count);
}

bool BridgeStorageManager::LoadBridgedDevicesIndexes(uint8_t *indexes, uint8_t maxCount, size_t &count)
{
	if (!indexes) {
		return false;
	}

	return Nrf::PersistentStorage::Instance().Load(&mBridgedDevicesIndexes, indexes, maxCount, count);
}

bool BridgeStorageManager::StoreBridgedDeviceEndpointId(uint16_t endpointId, uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceEndpointId);

	return Nrf::PersistentStorage::Instance().Store(&id, &endpointId, sizeof(endpointId));
}

bool BridgeStorageManager::LoadBridgedDeviceEndpointId(uint16_t &endpointId, uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceEndpointId);

	return LoadDataToObject(&id, endpointId);
}

bool BridgeStorageManager::RemoveBridgedDeviceEndpointId(uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceEndpointId);

	return Nrf::PersistentStorage::Instance().Remove(&id);
}

bool BridgeStorageManager::StoreBridgedDeviceNodeLabel(const char *label, size_t labelLength,
						       uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceNodeLabel);

	return Nrf::PersistentStorage::Instance().Store(&id, label, labelLength);
}

bool BridgeStorageManager::LoadBridgedDeviceNodeLabel(char *label, size_t labelMaxLength, size_t &labelLength,
						      uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceNodeLabel);

	return Nrf::PersistentStorage::Instance().Load(&id, label, labelMaxLength, labelLength);
}

bool BridgeStorageManager::RemoveBridgedDeviceNodeLabel(uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceNodeLabel);

	return Nrf::PersistentStorage::Instance().Remove(&id);
}

bool BridgeStorageManager::StoreBridgedDeviceType(uint16_t deviceType, uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceType);

	return Nrf::PersistentStorage::Instance().Store(&id, &deviceType, sizeof(deviceType));
}

bool BridgeStorageManager::LoadBridgedDeviceType(uint16_t &deviceType, uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceType);

	return LoadDataToObject(&id, deviceType);
}

bool BridgeStorageManager::RemoveBridgedDeviceType(uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBridgedDeviceType);

	return Nrf::PersistentStorage::Instance().Remove(&id);
}

bool BridgeStorageManager::StoreBridgedDevice(const MatterBridgedDevice *device, uint8_t index)
{
	if (!device) {
		return false;
	}

	if (!StoreBridgedDeviceEndpointId(device->GetEndpointId(), index)) {
		return false;
	}

	if (!StoreBridgedDeviceNodeLabel(device->GetNodeLabel(), strlen(device->GetNodeLabel()), index)) {
		return false;
	}

	return StoreBridgedDeviceType(device->GetDeviceType(), index);
}

#ifdef CONFIG_BRIDGED_DEVICE_BT
bool BridgeStorageManager::StoreBtAddress(bt_addr_le_t addr, uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBtAddress);

	return Nrf::PersistentStorage::Instance().Store(&id, &addr, sizeof(addr));
}

bool BridgeStorageManager::LoadBtAddress(bt_addr_le_t &addr, uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBtAddress);

	return LoadDataToObject(&id, addr);
}

bool BridgeStorageManager::RemoveBtAddress(uint8_t bridgedDeviceIndex)
{
	Nrf::PersistentStorageNode id = CreateIndexNode(bridgedDeviceIndex, &mBtAddress);

	return Nrf::PersistentStorage::Instance().Remove(&id);
}
#endif

} /* namespace Nrf */
