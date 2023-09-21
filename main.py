import requests
import socket
import pywifi
import time
import re
import os

from pywifi import const

# 使用方法：
account = "114514"  # 在双引号内输入学工号
password = "1919810"  # 在双引号内输入密码，默认身份证后六位
isp = "unicom"  # 在双引号内输入运营商，联通为unicom，电信为telcom，移动忘了，留空为校园内网
# 截止到上面，下面不用动

# TODO CET4/6考试期间学校无差别断网会断开认证，最好能有办法检测到认证切断便重新认证（但是一直断线带来的丢包也挺头疼的，开摆
# TODO 读取配置文件甚至是GUI来设置账号（pip的Yaml用不了的样子，麻了

time.sleep(5)
# 获取网卡接口
wifi = pywifi.PyWiFi()
# 使用第一个无线网卡
ifaces = wifi.interfaces()[0]
# 获取wifi的连接状态
wifi_status = ifaces.status()
# 检查WiFi是否处于切断状态
if wifi_status == const.IFACE_DISCONNECTED:
    while True:
        print("未连接到WiFi，等待3秒")
        time.sleep(3)
        wifi_status = ifaces.status()
        if wifi_status == const.IFACE_CONNECTED:
            print("连接到WiFi，开始登录")
            break

command = "netsh wlan show network"
# 获取命令执行结果
result = os.popen(command)
# 读取返回值
text = result.read()
result.close()
# 匹配Wifi
wifi_list = re.findall(r"SSID [\d] : (.*?)\n", text, re.MULTILINE | re.DOTALL)
# 检测是否有空名称
wifi_list = [i for i in wifi_list if i]

# 循环打印
for i in wifi_list:
    if i == "i-wxxy":
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect(('10.1.99.100', 80))
            ip = s.getsockname()[0]  # 拿到本机IP
            print("套接字操作成功!本机IP为:" + ip)
        finally:
            s.close()

            res = requests.get(url=
                               "http://10.1.99.100:801/eportal/portal/login?callback=dr1003&login_method=1&user_account=%2C0%2C"
                               + account + "%40" + isp + "&user_password=" + password + "&wlan_user_ip=" + ip + "&wlan_user_ipv6=&wlan_user_mac"
                               "=000000000000&wlan_ac_ip=&wlan_ac_name=&jsVersion=4.1.3&terminal_type=1&lang=zh-cn&v=7751&lang=zh")
            print(res.text)
            break
    else:
        print("未检测到校园网，已退出")
