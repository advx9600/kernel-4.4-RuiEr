export PATH=$PATH:/home/abc/Public/tools/lame/_install/bin:/home/abc/Public/android/android4.4/ruier/android/prebuilts/gcc/linux-x86/arm/arm-eabi-4.7/bin:/home/abc/Public/android/android4.4/ruier/android/u-boot/tools
make -j16 ARCH=arm uImage 
./s5p4418_boot/make-boot.sh
