Tested on Ubuntu 14.04 (kernel 4.4)

1. Insert module:  
`sudo insmod test-module.ko`

2. Create partition table:  
`sudo fdisk /dev/ramdisk0`

3. Create file system:  
`sudo mkfs /dev/ramdisk0p1`

4. Mount to folder /foldername:  
`sudo mount /dev/ramdisk0p1 /foldername`

5. Now you can use disk normally with `sudo`

5. Unmount:  
`sudo umount /dev/ramdisk0p1`

6. Remove module:  
`sudo rmmod test-module.ko`
