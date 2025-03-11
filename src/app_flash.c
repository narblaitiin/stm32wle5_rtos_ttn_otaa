/*
 * Copyright (c) 2024
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include "app_flash.h"

//  ======== app_flash_init =====================================
int8_t app_flash_init(struct nvs_fs *fs)
{
	struct flash_pages_info info;
	int8_t ret;

	// configuration of storage partition in flash memory
	fs->flash_device = NVS_PARTITION_DEVICE;
	if (!device_is_ready(fs->flash_device)) {
		printk("flash device \"%s\" is not ready\n", fs->flash_device->name);
		return 0;
	} else {
		printk("- found device: \"%s\", getting nvs memory\n", fs->flash_device->name);
	}

	fs->offset = NVS_PARTITION_OFFSET;
	ret = flash_get_page_info_by_offs(fs->flash_device, fs->offset, &info);
	if (ret) {
		printk("unable to get page info. error: %d\n", ret);
		return 0;
	}
	
	fs->sector_size = info.size;
	if (!fs->sector_size || fs->sector_size % info.size) {
		printk("invalid sector size\n");
		return 0;
	}

	// mounting data storage partition in flash memory
	fs->sector_count = 4U;
	ret = nvs_mount(fs);
	if (ret) {
		printk("flash init failed. error: %d\n", ret);
		return 0;
	}

	// cleaning data storage partition
//	(void)nvs_delete(fs, NVS_DEVNONCE_ID);
}

//  ======== app_flash_init_param ===============================
int8_t app_flash_init_param(struct nvs_fs *fs, uint16_t id, void *data)
{
	int8_t ret;

	ret = nvs_read(fs, id, data, sizeof(data));
	if (ret > 0) {
		printk("ID: %d, address: %s\n", id, data);
	} else {
		printk("no address found, adding %s at id %d\n", data, id);
		(void)nvs_write(fs, id, data, sizeof(data));	
	}
	return 0;
}

