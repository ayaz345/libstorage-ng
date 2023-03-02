/*
 * Copyright (c) [2017-2023] SUSE LLC
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


#include "storage/Actions/UpdateInEtcFstab.h"
#include "storage/Filesystems/MountPointImpl.h"


namespace storage
{

    namespace Action
    {

	Text
	UpdateInEtcFstab::text(const CommitData& commit_data) const
	{
	    const MountPoint* mount_point_lhs = to_mount_point(get_device(commit_data.actiongraph, LHS));
	    const MountPoint* mount_point_rhs = to_mount_point(get_device(commit_data.actiongraph, RHS));
	    return mount_point_rhs->get_impl().do_update_in_etc_fstab_text(mount_point_lhs, commit_data.tense);
	}


	void
	UpdateInEtcFstab::commit(CommitData& commit_data, const CommitOptions& commit_options) const
	{
	    const MountPoint* mount_point_lhs = to_mount_point(get_device(commit_data.actiongraph, LHS));
	    const MountPoint* mount_point_rhs = to_mount_point(get_device(commit_data.actiongraph, RHS));
	    mount_point_rhs->get_impl().do_update_in_etc_fstab(commit_data, mount_point_lhs);
	}

    }

}
