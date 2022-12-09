kmodule=kmod
user=user
path=/home/vladislav/Downloads/lab2os/src/dfs/kmod.ko
obj-m += $(kmodule).o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
ins:
	sudo insmod $(kmodule).ko
rm:
	sudo rmmod $(kmodule).ko
user:
	gcc -o $(user) $(user).c
test:
	sudo ./$(user) $(path)
