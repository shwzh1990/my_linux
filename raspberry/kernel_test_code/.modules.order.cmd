cmd_/home/shwzh1990/Documents/learn/linux/my_linux/raspberry/kernel_test_code/modules.order := {   echo /home/shwzh1990/Documents/learn/linux/my_linux/raspberry/kernel_test_code/kernel_test_code.ko; :; } | awk '!x[$$0]++' - > /home/shwzh1990/Documents/learn/linux/my_linux/raspberry/kernel_test_code/modules.order