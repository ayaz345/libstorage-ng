/*
 * Copyright (c) 2018 SUSE LLC
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#include "storage/Utils/StorageDefines.h"
#include "storage/Devices/StrayBlkDeviceImpl.h"
#include "storage/Devicegraph.h"
#include "storage/Utils/Enum.h"


namespace storage
{

    using namespace std;


    StrayBlkDevice*
    StrayBlkDevice::create(Devicegraph* devicegraph, const string& name)
    {
	StrayBlkDevice* ret = new StrayBlkDevice(new StrayBlkDevice::Impl(name));
	ret->Device::create(devicegraph);
	return ret;
    }


    StrayBlkDevice*
    StrayBlkDevice::create(Devicegraph* devicegraph, const string& name, const Region& region)
    {
	StrayBlkDevice* ret = new StrayBlkDevice(new StrayBlkDevice::Impl(name, region));
	ret->Device::create(devicegraph);
	return ret;
    }


    StrayBlkDevice*
    StrayBlkDevice::create(Devicegraph* devicegraph, const string& name, unsigned long long size)
    {
	StrayBlkDevice* ret = new StrayBlkDevice(new StrayBlkDevice::Impl(name, Region(0, size / 512, 512)));
	ret->Device::create(devicegraph);
	return ret;
    }


    StrayBlkDevice*
    StrayBlkDevice::load(Devicegraph* devicegraph, const xmlNode* node)
    {
	StrayBlkDevice* ret = new StrayBlkDevice(new StrayBlkDevice::Impl(node));
	ret->Device::load(devicegraph);
	return ret;
    }


    StrayBlkDevice::StrayBlkDevice(Impl* impl)
	: BlkDevice(impl)
    {
    }


    StrayBlkDevice*
    StrayBlkDevice::clone() const
    {
	return new StrayBlkDevice(get_impl().clone());
    }


    StrayBlkDevice::Impl&
    StrayBlkDevice::get_impl()
    {
	return dynamic_cast<Impl&>(Device::get_impl());
    }


    const StrayBlkDevice::Impl&
    StrayBlkDevice::get_impl() const
    {
	return dynamic_cast<const Impl&>(Device::get_impl());
    }


    StrayBlkDevice*
    StrayBlkDevice::find_by_name(Devicegraph* devicegraph, const string& name)
    {
	return to_stray_blk_device(BlkDevice::find_by_name(devicegraph, name));
    }


    const StrayBlkDevice*
    StrayBlkDevice::find_by_name(const Devicegraph* devicegraph, const string& name)
    {
	return to_stray_blk_device(BlkDevice::find_by_name(devicegraph, name));
    }


    vector<StrayBlkDevice*>
    StrayBlkDevice::get_all(Devicegraph* devicegraph)
    {
	return devicegraph->get_impl().get_devices_of_type<StrayBlkDevice>();
    }


    vector<const StrayBlkDevice*>
    StrayBlkDevice::get_all(const Devicegraph* devicegraph)
    {
	return devicegraph->get_impl().get_devices_of_type<const StrayBlkDevice>();
    }


    bool
    is_stray_blk_device(const Device* device)
    {
	return is_device_of_type<const StrayBlkDevice>(device);
    }


    StrayBlkDevice*
    to_stray_blk_device(Device* device)
    {
	return to_device_of_type<StrayBlkDevice>(device);
    }


    const StrayBlkDevice*
    to_stray_blk_device(const Device* device)
    {
	return to_device_of_type<const StrayBlkDevice>(device);
    }

}
